// ======================== GESTIÓN DE CONECTIVIDAD ========================

void setupWiFi() {
  WiFi.setOutputPower(20.5); //Maxima potencia
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false); // No guardar en flash para reducir desgaste
}

bool connectToWiFi(const char* ssid, const char* password, unsigned long timeout) {
  DEBUG_PRINT("Conectando a WiFi: ");
  DEBUG_PRINTLN(ssid);
  DEBUG_PRINT("Pass: ");
  DEBUG_PRINTLN(password);

  // Si la configuración de IP estática está habilitada, la aplicamos
  if (config.useStaticIP) {
    IPAddress ip, gateway, subnet, dns;
    if (ip.fromString(config.staticIP) && 
        gateway.fromString(config.staticGateway) && 
        subnet.fromString(config.staticSubnet) && 
        dns.fromString(config.staticDNS)) {
      
      DEBUG_PRINTLN("Aplicando configuración de IP estática:");
      DEBUG_PRINT("  IP: "); DEBUG_PRINTLN(ip);
      DEBUG_PRINT("  Gateway: "); DEBUG_PRINTLN(gateway);
      DEBUG_PRINT("  Subnet: "); DEBUG_PRINTLN(subnet);
      DEBUG_PRINT("  DNS: "); DEBUG_PRINTLN(dns);
      
      if (WiFi.config(ip, gateway, subnet, dns)) {
        DEBUG_PRINTLN("Configuración de IP estática aplicada con éxito.");
      } else {
        DEBUG_PRINTLN("ERROR: No se pudo aplicar la configuración de IP estática. Usando DHCP.");
      }
    } else {
      DEBUG_PRINTLN("ERROR: Las direcciones IP estáticas guardadas son inválidas. Usando DHCP.");
    }
  }
  
  unsigned long timeout_ms = 20000;
   
  //WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(150);
  
  WiFi.setOutputPower(20.5);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  WiFi.begin(ssid, password);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout) {
    delay(1000);
    DEBUG_PRINT(".");
  }

  unsigned long start = millis();
  wl_status_t last = WL_IDLE_STATUS;

  while (millis() - start < timeout_ms) {
    wl_status_t st = WiFi.status();
    if (st != last) {
      last = st;
      Serial.printf("[WiFi] Status=%d\n", st);
    }
    if (st == WL_CONNECTED) break;
    delay(300);
    yield(); // evita WDT
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("[WiFi] ✅ Conectado. IP: %s  RSSI: %d dBm  Canal: %d\n",
                  WiFi.localIP().toString().c_str(), WiFi.RSSI(), WiFi.channel());
    // Si usaste AP_STA solo para config y ya no querés AP:
    // WiFi.softAPdisconnect(true); WiFi.mode(WIFI_STA);
    return true;
  } else {
    Serial.println("[WiFi] ❌ No conectó. Diagnóstico:");
    WiFi.printDiag(Serial);
    Serial.println("Posibles causas: 5GHz/WPA3, canal 12/13 sin pista, SSID duplicado, clave con caracteres raros, filtro MAC.");
    cuentaErroresTotales=cuentaErroresTotales+1;
    if (cuentaErroresTotales>10){
      resetDispositivo();
    }
    DEBUG_PRINT("Cuenta error total: ");
    DEBUG_PRINTLN(cuentaErroresTotales);
    return false;
  }
}

bool checkWifiConnection(unsigned long currentTime) {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  
  if (currentTime - lastWifiRetry >= wifiRetryDelay) {
    DEBUG_PRINTLN("Reintentando WiFi...");
    WiFi.reconnect();
    lastWifiRetry = currentTime;
    wifiRetryCount++;
    
    // Exponential backoff (máximo 60 segundos)
    wifiRetryDelay = min(wifiRetryDelay * 2, 60000UL);
    
    if (wifiRetryCount > 10) {
      currentState = STATE_CONFIG_MODE;
      return false;
    }
  }
  
  return false;
}

// ======================== MODO CONFIGURACIÓN ========================

void startConfigMode() {
  CONFIG_MODE_TIMEOUT = 120000; //2min
  config_mode = true;
  currentState = STATE_CONFIG_MODE;
  lastConfigModeStart = millis();
  
  DEBUG_PRINTLN("Iniciando modo configuración...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, "");
  
  DEBUG_PRINT("AP creado: ");
  DEBUG_PRINTLN(ap_ssid);
  DEBUG_PRINT("IP: ");
  DEBUG_PRINTLN(WiFi.softAPIP());
  
  // Configurar rutas del servidor web
  setupWebServer();
  server.begin();
}

void startNormalMode() {
  config_mode = false;
  
  // Establecemos el estado optimistamente, pero lo cambiaremos si falla
  currentState = STATE_NORMAL_OPERATION;

  DEBUG_PRINTLN("=== INICIANDO MODO NORMAL (AP+STA) ===");
  
  // PASO 1: Desconectar temporalmente el cliente WiFi
  WiFi.disconnect();
  delay(100);
  
  // PASO 2: Establecer modo AP+STA
  WiFi.mode(WIFI_AP_STA);
  delay(100);
  
  // PASO 3: Configurar el AP de diagnóstico
  String diagSSID = String(ssidFL) + "_DIAG";
  diagSSID.toCharArray(ap_diag_ssid, sizeof(ap_diag_ssid));
  
  DEBUG_PRINT("Iniciando AP de diagnóstico en canal 1: ");
  DEBUG_PRINTLN(ap_diag_ssid);
  
  bool apResult = WiFi.softAP(ap_diag_ssid, "123456", 1, false);
  
  if (!apResult) {
    DEBUG_PRINTLN("Falló AP con contraseña guardada, intentando sin contraseña");
    apResult = WiFi.softAP(ap_diag_ssid);
  }
  
  if (apResult) {
    DEBUG_PRINT("✅ AP de diagnóstico iniciado - Canal: ");
    DEBUG_PRINT(WiFi.channel());
    DEBUG_PRINT(" IP: ");
    DEBUG_PRINTLN(WiFi.softAPIP());
  } else {
    DEBUG_PRINTLN("❌ No se pudo iniciar el AP de diagnóstico");
  }
  
  // PASO 4: Intentar conectar el cliente WiFi
  DEBUG_PRINTLN("Conectando cliente WiFi...");
  if (connectToWiFi(config.stored_ssid, config.stored_password)) {
    DEBUG_PRINT("✅ Cliente WiFi conectado - Canal: ");
    DEBUG_PRINT(WiFi.channel());
    DEBUG_PRINT(" IP: ");
    DEBUG_PRINTLN(WiFi.localIP());
    
    // Solo si todo fue bien, inicializamos MQTT
    initializeMqtt();
    // --- INICIO DE LA MODIFICACIÓN CLAVE ---
    // Reiniciar el servidor web para que escuche en AMBAS interfaces (AP y STA)
    DEBUG_PRINTLN("Reiniciando el servidor web para el modo dual...");
    server.stop(); // Detenemos el servidor antiguo
    setupWebServer(); // Lo volvemos a configurar (ahora cargará las rutas correctas)
    server.begin();  // Lo iniciamos de nuevo. Ahora escuchará en 192.168.4.1 y 192.168.1.37
    DEBUG_PRINTLN("Servidor web reiniciado y escuchando en ambas interfaces.");

    // currentState ya es STATE_NORMAL_OPERATION, así que continuamos
  } else {
    DEBUG_PRINTLN("❌ Falló conexión WiFi. Cambiando a modo de reconexión.");
    
    // Cambiamos el estado para que el loop maneje la reconexión
    currentState = STATE_WIFI_RECONNECT;
    
    // NO inicializamos MQTT si no hay WiFi
  }

  DEBUG_PRINTLN("=== Inicio de modo normal completado ===");
}

// ======================== MÁQUINA DE ESTADOS PRINCIPAL ========================
void stopConfigAP() {
  DEBUG_PRINTLN("Cerrando portal/AP...");
  server.stop();
  // Si usas MDNS: MDNS.end();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
}

void handleConfigMode() {
  static bool verificationStarted = false;
  static SystemState lastState = STATE_NORMAL_OPERATION; // Estado inicial
  if (lastState != STATE_CONFIG_MODE) {
    DEBUG_PRINTLN("=== Entrando en MODO CONFIGURACIÓN ===");
    DEBUG_PRINTLN("Iniciando verificación Modbus única para esta sesión.");
    
    // Reiniciar el flag de verificación solo al entrar al estado
    verificationStarted = false; 
    
    // Actualizar el último estado para no volver a entrar aquí hasta que cambiemos de estado
    lastState = STATE_CONFIG_MODE;
  }
  
  server.handleClient();
  
  static uint8_t lastStationCount = 0;
  uint8_t currentStationCount = WiFi.softAPgetStationNum(); // Número de clientes conectados

  if (currentStationCount != lastStationCount) {
    if (currentStationCount > lastStationCount) {
      DEBUG_PRINTLN("Se conecto un cliente dando 10min para configuracion...");
      lastConfigModeStart = millis();
      CONFIG_MODE_TIMEOUT = 600000; //10min
    }
    lastStationCount = currentStationCount;
  }

  // Ejecuta la verificación Modbus (solo cuando está en estado de configuración)
  if (!verificationStarted) {
    // Iniciar la verificación solo si no se ha completado ya en esta sesión
    if (!isVerificationComplete()) {
      DEBUG_PRINTLN("Iniciando verificación Modbus en segundo plano...");
      startConnectionVerification(); // Prepara la máquina de estados
      verificationStarted = true;    // Marca que el proceso ha iniciado
    } else {
      DEBUG_PRINTLN("La verificación ya fue completada previamente en esta sesión.");
    }
  } else {
    // Si la verificación ya inició, procesar un paso de la máquina de estados
    if (checkModbusConnectionNonBlocking()) {
      // La verificación ha finalizado
      DEBUG_PRINTLN("Verificación Modbus completada. El resultado está disponible.");
      seguirLeyendoEnConfig=true;
      // Importante: NO reiniciamos 'verificationStarted' aquí.
      // Permanecerá en 'true' hasta que se salga y vuelva a entrar a CONFIG_MODE.
    }
  }
  
  if (seguirLeyendoEnConfig){
    DEBUG_PRINTLN("Sigue leyendo en config");
    uint16_t dataLeerConfig;
    readModbus(config.func_Probe1, config.addr_Probe1, dataLeerConfig);
  }
  
  // Chequear timeout sin bloquear (overflow-safe)
  if ((millis() - lastConfigModeStart) >= CONFIG_MODE_TIMEOUT) {
    DEBUG_PRINTLN("Timeout de configuración alcanzado.");

    // Cerrar portal/AP antes de intentar STA
    stopConfigAP();
    config_mode = false;

    // Siempre recargo por si el usuario guardó algo durante el portal
    loadConfigFromEEPROM();

    if (strlen(config.stored_ssid) > 0) {
      DEBUG_PRINTLN("Intentando conexión con credenciales guardadas...");
      if (connectToWiFi(config.stored_ssid, config.stored_password)) {
        MQTT_SEND_INTERVAL = String(config.tiempoIntervalo).toInt();
        DEBUG_PRINTLN("Conectado. Iniciando modo normal...");
        startNormalMode();
        return;
      } else {
        DEBUG_PRINTLN("Fallo conexión. Paso a STATE_WIFI_RECONNECT.");
        currentState = STATE_WIFI_RECONNECT;
        return;
      }
    } else {
      // No hay credenciales: decide tu estrategia.
      // O bien reabrir portal por otro ciclo breve, o reiniciar, o dormir.
      DEBUG_PRINTLN("No hay credenciales guardadas. Reabro portal por 2 min.");
      ESP.restart(); // reset limpio para reintentar boot normal
    }
  }
}

void handleNormalOperation(unsigned long currentTime) {
  // Verificar conexión WiFi
  if (currentTime - lastWifiCheck >= WIFI_CHECK_INTERVAL) {
    if (!checkWifiConnection(currentTime)) {
      currentState = STATE_WIFI_RECONNECT;
      return;
    }
    lastWifiCheck = currentTime;
  }
  
  // Gestionar conexión MQTT
  if (!connectToMqtt()) {
    // Si falla MQTT, continuar con operación local
  }

  // Enviar datos por MQTT solo si es tiempo Y si todas las lecturas están completas
  if (currentTime - lastMqttSend >= MQTT_SEND_INTERVAL && mqttClient.connected()) {
    // Llamar a la función de lectura no bloqueante en CADA iteración del loop
    readSensorDataNonBlocking();
    if (sensorStatus.cycleComplete) {
      DEBUG_PRINTLN("Enviando datos por MQTT...");
      sendMqttData(); // Llamamos a la función que solo envía
      lastMqttSend = currentTime;
    }
  }
  
  // Enviar datos por MQTT
  //if (currentTime - lastMqttSend >= MQTT_SEND_INTERVAL && mqttClient.connected()) {
  //  DEBUG_PRINTLN("Preparando transmision...");
  //  sendBatchMqttData();
  //  lastMqttSend = currentTime;
  //  //MQTT_SEND_INTERVAL=10000;
  //}
}

void handleWifiReconnect() {
  DEBUG_PRINTLN("Intentando reconectar al WiFi (Intento: "+String(wifiRetryCount)+")");
  if (connectToWiFi(config.stored_ssid, config.stored_password)) {
    currentState = STATE_NORMAL_OPERATION;
    DEBUG_PRINTLN("WiFi reconectado");
  } else {
    wifiRetryCount++;
    if (wifiRetryCount > 5) { // Después de 5 intentos fallidos
      DEBUG_PRINTLN("Demasiados fallos de WiFi, yendo a modo configuración");
      startConfigMode();
    }
  }
}

void handleSaveWifiClient() {
  DEBUG_PRINTLN("Guardando configuración del cliente WiFi...");
  String responseJson;
  
  bool useStatic = server.hasArg("useStaticIP");
  String ip = server.arg("staticIP");
  String gateway = server.arg("staticGateway");
  String subnet = server.arg("staticSubnet");
  String dns = server.arg("staticDNS");

  // Validación simple de formato de IP
  auto isValidIP = [](const String& s) {
    int dots = 0;
    for (int i = 0; i < s.length(); ++i) {
      char c = s.charAt(i);
      if (c == '.') {
        dots++;
      }else if (c < '0' || c > '9') {
        return false;
      }
    }
    return dots == 3;
  };

  if (useStatic && (!isValidIP(ip) || !isValidIP(gateway) || !isValidIP(subnet) || !isValidIP(dns))) {
    if (!isValidIP(ip)){
      responseJson = "{\"status\":\"error\",\"message\":\"La dirección IP tiene un formato inválido.\"}";
    }else if (!isValidIP(gateway)){
      responseJson = "{\"status\":\"error\",\"message\":\"La puerta de enlace tiene un formato inválido.\"}";
    } else if (!isValidIP(subnet)){
      responseJson = "{\"status\":\"error\",\"message\":\"La mascara de subred tiene un formato inválido.\"}";
    } else if (!isValidIP(dns)){
      responseJson = "{\"status\":\"error\",\"message\":\"El DNS tiene un formato inválido.\"}";
    }
    server.send(400, "application/json", responseJson);
    return;
  }

  // Guardar en la estructura de configuración
  config.useStaticIP = useStatic;
  
  if (String(useStatic)=="0"){
    DEBUG_PRINTLN("Guradando configuracion DHCP de WiFi..."+String(useStatic));
    strncpy(config.staticIP, "0.0.0.0", sizeof(config.staticIP) - 1);
    config.staticIP[sizeof(config.staticIP) - 1] = '\0';
    strncpy(config.staticGateway, "0.0.0.0", sizeof(config.staticGateway) - 1);
    config.staticGateway[sizeof(config.staticGateway) - 1] = '\0';
    strncpy(config.staticSubnet, "0.0.0.0", sizeof(config.staticSubnet) - 1);
    config.staticSubnet[sizeof(config.staticSubnet) - 1] = '\0';
    strncpy(config.staticDNS, "0.0.0.0", sizeof(config.staticDNS) - 1);
    config.staticDNS[sizeof(config.staticDNS) - 1] = '\0';
  }else{
    DEBUG_PRINTLN("Guradando configuracion IP estatica de WiFi..."+String(useStatic));
    strncpy(config.staticIP, ip.c_str(), sizeof(config.staticIP) - 1);
    config.staticIP[sizeof(config.staticIP) - 1] = '\0';
    strncpy(config.staticGateway, gateway.c_str(), sizeof(config.staticGateway) - 1);
    config.staticGateway[sizeof(config.staticGateway) - 1] = '\0';
    strncpy(config.staticSubnet, subnet.c_str(), sizeof(config.staticSubnet) - 1);
    config.staticSubnet[sizeof(config.staticSubnet) - 1] = '\0';
    strncpy(config.staticDNS, dns.c_str(), sizeof(config.staticDNS) - 1);
    config.staticDNS[sizeof(config.staticDNS) - 1] = '\0';
  }
  
  // Guardar en EEPROM
  saveConfigToEEPROM();

  // Responder al usuario
  responseJson = "{\"status\":\"success\",\"message\":\"Configuración guardada correctamente.\"}";
  server.send(200, "application/json", responseJson);
  
  DEBUG_PRINTLN("Configuración del cliente WiFi guardada. Reiniciando dispositivo en 2 segundos...");
  delay(2000);
  ESP.restart();
}

void handleSaveApConfig() {
  DEBUG_PRINTLN("Guardando configuración del AP...");

  String apPassword = server.arg("apPassword");
  int apChannel = server.arg("apChannel").toInt();

  // Validar canal
  if (apChannel < 1 || apChannel > 11) {
    server.send(400, "text/html", "<h1>Error</h1><p>El canal del AP debe estar entre 1 y 11.</p><a href='/network-config'>Volver</a>");
    return;
  }

  // Solo actualizar la contraseña si se proporcionó una nueva
  if (apPassword.length() > 0) {
    strncpy(config.passDiag, apPassword.c_str(), sizeof(config.passDiag) - 1);
    config.passDiag[sizeof(config.passDiag) - 1] = '\0';
  }

  // Guardar en EEPROM
  saveConfigToEEPROM();

  // Responder al usuario
  server.send(200, "text/html", "<h1>Configuración Guardada</h1><p>El dispositivo se reiniciará para aplicar los cambios. Por favor, espere...</p>");

  DEBUG_PRINTLN("Configuración del AP guardada. Reiniciando dispositivo en 2 segundos...");
  delay(2000);
  ESP.restart();
}

// Función de utilidad para enviar una alerta y volver atrás
void sendAlertAndGoBack(String message) {
  String page = R"(
<!DOCTYPE html>
<html><head><title>Error</title></head><body>
<script>alert(')" + message + R"('); window.history.back();</script>
</body></html>
)";
  server.send(400, "text/html", page);
}
