// ======================== FUNCIONES DE UTILIDAD ========================

void resetWatchdog() {
  ESP.wdtFeed();
}

uint32_t calculateChecksum(const ConfigData* data) {
  // CRC32 simple
  uint32_t crc = 0xFFFFFFFF;
  const uint8_t* ptr = (const uint8_t*)data;
  size_t len = sizeof(ConfigData) - sizeof(uint32_t); // Excluir el checksum
  
  for (size_t i = 0; i < len; i++) {
    crc ^= ptr[i];
    for (int j = 0; j < 8; j++) {
      crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
    }
  }
  return ~crc;
}

void handleError(ErrorCode error) {
  lastError = error;
  DEBUG_PRINT("Error: ");
  DEBUG_PRINTLN(error);
  cuentaErroresTotales=cuentaErroresTotales+1;
  if (cuentaErroresTotales>10){
    resetDispositivo();
  }
  DEBUG_PRINT("Cuenta error total: ");
  DEBUG_PRINTLN(cuentaErroresTotales);
}

void actLicencia(const char* value){
  strncpy(config.licenciaActual, value, sizeof(config.licenciaActual));
  config.licenciaActual[sizeof(config.licenciaActual) - 1] = '\0';  // Asegura que termine en null
  saveConfigToEEPROM();
  DEBUG_PRINT("Se actualizó la licencia a: ");
  DEBUG_PRINTLN(value);
}

void actTiempoIntervalo(const char* value){
  strncpy(config.tiempoIntervalo, value, sizeof(config.tiempoIntervalo));
  config.tiempoIntervalo[sizeof(config.tiempoIntervalo) - 1] = '\0';  // Asegura que termine en null
  saveConfigToEEPROM();
  MQTT_SEND_INTERVAL=String(value).toInt();
  DEBUG_PRINT("Se actualizó el tiempo de intervalo de envio de datos a: ");
  DEBUG_PRINTLN(value);
}

void resetDispositivo() {
  DEBUG_PRINTLN("Reseteando el dispositivo...");
  delay(1000);
  ESP.restart();
}

void resetToFactory() {
  DEBUG_PRINTLN("Restableciendo a fábrica...");
  
  // Valores por defecto
  strncpy(config.tokenDispositivo, tokenDefecto, sizeof(config.tokenDispositivo) - 1);
  config.tokenDispositivo[sizeof(config.tokenDispositivo) - 1] = '\0';
  strncpy(config.stored_ssid, ssidDefecto, sizeof(config.stored_ssid) - 1);
  config.stored_ssid[sizeof(config.stored_ssid) - 1] = '\0';
  strncpy(config.stored_password, passWIFI, sizeof(config.stored_password) - 1);
  config.stored_password[sizeof(config.stored_password) - 1] = '\0';
  strncpy(config.tokenDispositivo, tokenDefecto, sizeof(config.tokenDispositivo) - 1);
  config.tokenDispositivo[sizeof(config.tokenDispositivo) - 1] = '\0';
  strncpy(config.mqtt_server, mqtt_server_default, sizeof(config.mqtt_server) - 1);
  config.mqtt_server[sizeof(config.mqtt_server) - 1] = '\0';
  strncpy(config.Probe1, "", sizeof(config.Probe1) - 1);
  config.Probe1[sizeof(config.Probe1) - 1] = '\0';
  strncpy(config.SetPoint, "", sizeof(config.SetPoint) - 1);
  config.SetPoint[sizeof(config.SetPoint) - 1] = '\0';
  strncpy(config.StatusOn, "", sizeof(config.StatusOn) - 1);
  config.StatusOn[sizeof(config.StatusOn) - 1] = '\0';
  strncpy(config.Defrost, "", sizeof(config.Defrost) - 1);
  config.Defrost[sizeof(config.Defrost) - 1] = '\0';
  strncpy(config.PtaAbierta, "", sizeof(config.PtaAbierta) - 1);
  config.PtaAbierta[sizeof(config.PtaAbierta) - 1] = '\0';
  strncpy(config.ErrorPb1, "", sizeof(config.ErrorPb1) - 1);
  config.ErrorPb1[sizeof(config.ErrorPb1) - 1] = '\0';
  strncpy(config.ErrorPb2, "", sizeof(config.ErrorPb2) - 1);
  config.ErrorPb2[sizeof(config.ErrorPb2) - 1] = '\0';
  strncpy(config.AltoValorPb1, "", sizeof(config.AltoValorPb1) - 1);
  config.AltoValorPb1[sizeof(config.AltoValorPb1) - 1] = '\0';
  strncpy(config.BajoValorPb1, "", sizeof(config.BajoValorPb1) - 1);
  config.BajoValorPb1[sizeof(config.BajoValorPb1) - 1] = '\0';
  strncpy(config.Compressor, "", sizeof(config.Compressor) - 1);
  config.Compressor[sizeof(config.Compressor) - 1] = '\0';
  strncpy(config.mqtt_server, mqtt_server_default, sizeof(config.mqtt_server) - 1);
  config.mqtt_server[sizeof(config.mqtt_server) - 1] = '\0';
  config.mqtt_port=mqtt_port_default;
  strncpy(config.licenciaActual, licActual_default, sizeof(config.licenciaActual) - 1);
  config.licenciaActual[sizeof(config.licenciaActual) - 1] = '\0';
  strncpy(config.tiempoIntervalo, tiempoIntervalo_default, sizeof(config.tiempoIntervalo) - 1);
  config.tiempoIntervalo[sizeof(config.tiempoIntervalo) - 1] = '\0';
  config.addr_Probe1 = addr_Probe1_default;
  config.addr_SetPoint = addr_SetPoint_default;
  config.addr_StatusOn = addr_StatusOn_default;
  config.addr_Defrost = addr_Defrost_default;
  config.addr_PtaAbierta = addr_PtaAbierta_default;
  config.addr_ErrorPb1 = addr_ErrorPb1_default;
  config.addr_ErrorPb2 = addr_ErrorPb2_default;
  config.addr_AltoValorPb1 = addr_AltoValorPb1_default;
  config.addr_BajoValorPb1= addr_BajoValorPb1_default;
  config.addr_Compressor=addr_Compressor_default;
  config.addr_w_SetPoint=addr_w_SetPoint_default;
  config.addr_w_Status=addr_w_Status_default;
  strncpy(config.passDiag, passDiag_default, sizeof(config.passDiag) - 1);
  config.passDiag[sizeof(config.passDiag) - 1] = '\0';
  config.useStaticIP =  useStaticIP_default;
  strncpy(config.staticIP, staticIP_default, sizeof(config.staticIP) - 1);
  config.staticIP[sizeof(config.staticIP) - 1] = '\0';
  strncpy(config.staticGateway, staticGateway_default, sizeof(config.staticGateway) - 1);
  config.staticGateway[sizeof(config.staticGateway) - 1] = '\0';
  strncpy(config.staticSubnet, staticSubnet_default, sizeof(config.staticSubnet) - 1);
  config.staticSubnet[sizeof(config.staticSubnet) - 1] = '\0';
  strncpy(config.staticDNS, staticDNS_default, sizeof(config.staticDNS) - 1);
  config.staticDNS[sizeof(config.staticDNS) - 1] = '\0';
  config.func_Probe1 = func_Probe1_default;
  config.func_SetPoint = func_SetPoint_default;
  config.func_StatusOn = func_StatusOn_default;
  config.func_Defrost = func_Defrost_default;
  config.func_PtaAbierta = func_PtaAbierta_default;
  config.func_ErrorPb1 = func_ErrorPb1_default;
  config.func_ErrorPb2 = func_ErrorPb2_default;
  config.func_AltoValorPb1 = func_AltoValorPb1_default;
  config.func_BajoValorPb1= func_BajoValorPb1_default;
  config.func_Compressor=func_Compressor_default;
  config.func_w_SetPoint=func_w_SetPoint_default;
  config.func_w_Status=func_w_Status_default;
  saveConfigToEEPROM();
  
  DEBUG_PRINTLN("Reset completado. Reiniciando...");
  delay(1000);
  ESP.restart();
}

static inline String trimSafe(const String& s){ String t=s; t.trim(); return t; }

// Contrae espacios internos (más de uno -> uno)
static inline String collapseSpaces(const String& in){
  String o; o.reserve(in.length());
  bool prevSpace=false;
  for (size_t i=0;i<in.length();++i){
    char c=in[i];
    if (c==' '||c=='\t'||c=='\r'||c=='\n'){ if(!prevSpace){ o+=' '; prevSpace=true; } }
    else { o+=c; prevSpace=false; }
  }
  o.trim(); return o;
}

// Email básico: tener "@", algo antes y después, y un punto en la parte de dominio
static inline bool isEmailBasic(const String& e){
  int at = e.indexOf('@'); int dot = e.lastIndexOf('.');
  return at>0 && dot>at+1 && dot < (int)e.length()-1;
}

// Deja sólo dígitos y + (para teléfono)
static inline String keepDigitsPlus(const String& in){
  String o; o.reserve(in.length());
  for (size_t i=0;i<in.length();++i){
    char c=in[i];
    if ((c>='0'&&c<='9') || c=='+') o+=c;
  }
  return o;
}

// Percent-encode para querystring (por si usas GET)
static inline String urlEncode(const String &s) {
  String out; out.reserve(s.length()*3);
  const char *hex="0123456789ABCDEF";
  for (size_t i=0;i<s.length();++i){
    unsigned char c=(unsigned char)s[i];
    if ((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_'||c=='-'||c=='.'||c=='~')
      out+=char(c);
    else if (c==' ') out+="%20";
    else { out+='%'; out+=hex[(c>>4)&0x0F]; out+=hex[c&0x0F]; }
  }
  return out;
}

void clearEEPROM() {
  int eepromSize = 512; // tamaño que usaste en EEPROM.begin()
  for (int i = 0; i < eepromSize; i++) {
    EEPROM.write(i, 0xFF);   // o 0 si preferís dejarla en 0
  }
  EEPROM.commit(); // guarda cambios en la flash
  Serial.println("EEPROM borrada");
}

/*mantenimiento*/
// Sonda si el SSID existe en el aire (scan rápido)
bool isSSIDAvailable(const char* ssid) {
  // WiFi.scanNetworks(async=false, show_hidden=true)
  int n = WiFi.scanNetworks(false, true);
  if (n <= 0) return false;
  for (int i = 0; i < n; i++) {
    if (WiFi.SSID(i) == ssid) return true;
  }
  return false;
}

// Conecta a WiFi con timeout corto (reutiliza tu connectToWiFi si querés)
bool connectQuick(const char* ssid, const char* pass, unsigned long timeoutMs = 10000) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs) {
    delay(100);
    yield();
  }
  return WiFi.status() == WL_CONNECTED;
}

bool suscribeBootPacketMQTT(){
  snprintf(topicBuffer, sizeof(topicBuffer), "friolinkBMant/FLB/ack/%s", ssidFL);
  bool ok = mqttClient.subscribe(topicBuffer);
  DEBUG_PRINT("Suscribiendo a: "); 
  DEBUG_PRINT(topicBuffer);
  DEBUG_PRINT(" -> "); 
  DEBUG_PRINTLN(ok ? "OK" : "FALLO");
  return ok;
}
  
    
// Inicializa MQTT y publica un "boot packet"
bool publishBootPacketMQTT() {
  /*Para asegurarnos que la familia ya este cacheada*/
  DixellRawFingerprint fp;
  bool dixellOk = getDixellFingerprintCached(fp);

  int fc = dixellOk ? fp.familyCode : -1;
  int fi = dixellOk ? fp.firmware   : -1;
  
  uint8_t result;
  uint16_t data;

  DEBUG_PRINT("Leyendo Probe1 en mant.: ");
  if (readHoldingRegister(config.addr_Probe1, data)){
    strncpy(config.Probe1, String(data).c_str(), sizeof(config.Probe1) - 1);
    config.Probe1[sizeof(config.Probe1) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    DEBUG_PRINTLN("ERROR");
    strncpy(config.Probe1, "Error", sizeof(config.Probe1) - 1);
    config.Probe1[sizeof(config.Probe1) - 1] = '\0';
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo SetPoint en mant.: ");
  if (readHoldingRegister(config.addr_SetPoint, data)){
    strncpy(config.SetPoint, String(data).c_str(), sizeof(config.SetPoint) - 1);
    config.SetPoint[sizeof(config.SetPoint) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.SetPoint, "Error", sizeof(config.SetPoint) - 1);
    config.SetPoint[sizeof(config.SetPoint) - 1] = '\0';
    DEBUG_PRINTLN("ERROR");       
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo StatusOn en mant.: ");
  if (readCoil(config.addr_StatusOn, data)){
    strncpy(config.StatusOn, String(data).c_str(), sizeof(config.StatusOn) - 1);
    config.StatusOn[sizeof(config.StatusOn) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.StatusOn, "Error", sizeof(config.StatusOn) - 1);
    config.StatusOn[sizeof(config.StatusOn) - 1] = '\0';
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo Defrost en mant.: ");
  if (readCoil(config.addr_Defrost, data)){
    strncpy(config.Defrost, String(data).c_str(), sizeof(config.Defrost) - 1);
    config.Defrost[sizeof(config.Defrost) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.Defrost, "Error", sizeof(config.Defrost) - 1);
    config.Defrost[sizeof(config.Defrost) - 1] = '\0';
    DEBUG_PRINTLN("ERROR");  
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo PtaAbierta en mant.: ");
  if (readCoil(config.addr_PtaAbierta, data)){
    strncpy(config.PtaAbierta, String(data).c_str(), sizeof(config.PtaAbierta) - 1);
    config.PtaAbierta[sizeof(config.PtaAbierta) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.PtaAbierta, "Error", sizeof(config.PtaAbierta) - 1);
    config.PtaAbierta[sizeof(config.PtaAbierta) - 1] = '\0';
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo ErrorPb1 en mant.: ");
  if (readCoil(config.addr_ErrorPb1, data)){
    strncpy(config.ErrorPb1, String(data).c_str(), sizeof(config.ErrorPb1) - 1);
    config.ErrorPb1[sizeof(config.ErrorPb1) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.ErrorPb1, "Error", sizeof(config.ErrorPb1) - 1);
    config.ErrorPb1[sizeof(config.ErrorPb1) - 1] = '\0';
    DEBUG_PRINTLN("ERROR"); 
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo ErrorPb2 en mant.: ");
  if (readCoil(config.addr_ErrorPb2, data)){
    strncpy(config.ErrorPb2, String(data).c_str(), sizeof(config.ErrorPb2) - 1);
    config.ErrorPb2[sizeof(config.ErrorPb2) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.ErrorPb2, "Error", sizeof(config.ErrorPb2) - 1);
    config.ErrorPb2[sizeof(config.ErrorPb2) - 1] = '\0';
    DEBUG_PRINTLN("ERROR");      
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo AltoValorPb1 en mant.: ");
  if (readCoil(config.addr_AltoValorPb1, data)){
    strncpy(config.AltoValorPb1, String(data).c_str(), sizeof(config.AltoValorPb1) - 1);
    config.AltoValorPb1[sizeof(config.AltoValorPb1) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.AltoValorPb1, "Error", sizeof(config.AltoValorPb1) - 1);
    config.AltoValorPb1[sizeof(config.AltoValorPb1) - 1] = '\0';
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo BajoValorPb1 en mant.: ");
  if (readCoil(config.addr_BajoValorPb1, data)){
    strncpy(config.BajoValorPb1, String(data).c_str(), sizeof(config.BajoValorPb1) - 1);
    config.BajoValorPb1[sizeof(config.BajoValorPb1) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.BajoValorPb1, "Error", sizeof(config.BajoValorPb1) - 1);
    config.BajoValorPb1[sizeof(config.BajoValorPb1) - 1] = '\0';
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo Compressor en mant.: ");
  if (readCoil(config.addr_Compressor, data)){
    strncpy(config.Compressor, String(data).c_str(), sizeof(config.Compressor) - 1);
    config.Compressor[sizeof(config.Compressor) - 1] = '\0';
    DEBUG_PRINTLN("CORRECTO");
  } else {
    strncpy(config.Compressor, "Error", sizeof(config.Compressor) - 1);
    config.Compressor[sizeof(config.Compressor) - 1] = '\0';
    DEBUG_PRINTLN("ERROR");      
  }
  delay(msEntreSolicitudes);

  // JSON compacto con todos los datos
  snprintf(jsonBuffer, sizeof(jsonBuffer),
    "{\"t\":\"%s\",\"d\":{"
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"%s\":\"%s\","
    "\"hv\":\"%s\","
    "\"Fc\":%d,"
    "\"Fi\":%d"
    "},\"v\":\"%s\"}",
    ssidFL,
    "Probe1", config.Probe1,
    "SetPoint", config.SetPoint,
    "StatusOn", config.StatusOn,
    "Defrost", config.Defrost,
    "PtaAbierta", config.PtaAbierta,
    "ErrorPb1", config.ErrorPb1,
    "ErrorPb2", config.ErrorPb2,
    "AltoValorPb1", config.AltoValorPb1,
    "BajoValorPb1", config.BajoValorPb1,
    "Compressor", config.Compressor,
    HVERSION,          // 👈 nuevo
    fc,                // 👈 familyCode
    fi,                // 👈 firmware
    VERSION
  );

  // Asegurar MQTT conectado
  if (!mqttClient.connected()) {
    DEBUG_PRINTLN("MQTT NO Conectado en manenimiento, intentando reconectar.");
    String errA, errB;
    const char* chosenServer = nullptr;
    int chosenPort = 0;
  
    DEBUG_PRINT("Verificando broker MQTT (puerto por defecto)...");
    DEBUG_PRINT(mqtt_server_default);
    DEBUG_PRINT(":");
    DEBUG_PRINTLN(mqtt_port_default);
    if (canReachMqtt(mqtt_server_default, mqtt_port_default, 5000, &errA)) {
      chosenServer = mqtt_server_default;
      chosenPort   = mqtt_port_default;
      DEBUG_PRINTLN("MQTT OK por puerto default.");
    } else {
      DEBUG_PRINT("Default fallo: "); DEBUG_PRINTLN(errA);
      DEBUG_PRINTLN("Probando broker MQTT por puerto secundario...");
      if (canReachMqtt(mqtt_server_default, mqtt_port_secondary, 5000, &errB)) {
        chosenServer = mqtt_server_default;
        chosenPort   = mqtt_port_secondary;
        DEBUG_PRINTLN("MQTT OK por puerto secundario.");
      } else {
        DEBUG_PRINT("Secondary fallo: "); DEBUG_PRINTLN(errB);
        return false;
      }
    }
    
    // Configurar broker y callback
    mqttClient.setServer(
      (config.mqtt_server[0] ? config.mqtt_server : mqtt_server_default),
      (config.mqtt_port > 0  ? config.mqtt_port  : mqtt_port_default)
    );
    mqttClient.setCallback(callbackMQTTMant);
  
    // Forzar estado mantenimiento
    mantenimiento = true;
    currentState = STATE_MAINTENANCE;
    
    // Conectar MQTT y chequear estado
    if (!mqttClient.connect(ssidFL)) {
      DEBUG_PRINT("MQTT connect FALLO. state="); DEBUG_PRINTLN(mqttClient.state());
      return false;
    }
  
    if (mqttClient.connected()) {
      suscribeBootPacketMQTT();
    }
  }

  // Procesar MQTT
  mqttClient.loop();

  DEBUG_PRINT("Se va a enviar el siguiente Json en mantenimiento: ");
  DEBUG_PRINTLN(jsonBuffer);
  snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/FLB/Recepcion/%s", ssidFL);
  bool resultMQTT = mqttClient.publish(topicBuffer, jsonBuffer);
  
  if (resultMQTT) {
    DEBUG_PRINTLN("Datos MQTT de mantenimiento enviados");
  } else {
    DEBUG_PRINT("Error enviando datos de MQTT: ");
    DEBUG_PRINTLN(topicBuffer);
    DEBUG_PRINTLN(jsonBuffer);
    handleError(ERROR_MQTT_FAILED);
  }
  return resultMQTT;
}

void callbackMQTTMant(char* topic, byte* payload, unsigned int length) {
  String message;
  message.reserve(length);
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];

  DEBUG_PRINT("<--------------- MQTT recibido ["); DEBUG_PRINT(topic); DEBUG_PRINT("]: ");
  DEBUG_PRINTLN(message);

  // Si querés, marcá que recibiste ACK y continuá el flujo:
  mantEsperandoAck = false;
  // aquí podrías setear flags segun contenido de message (status OK, etc.)
}

// Intenta la secuencia: scan -> conectar -> publicar
bool tryPreferredAPAndSend() {
  DEBUG_PRINTLN("Buscando SSID de control: APPControlOrionSI");
  if (!isSSIDAvailable(PREFERRED_SSID)) {
    DEBUG_PRINTLN("SSID preferido no encontrado.");
    return false;
  }
  mantenimiento = true;

  DEBUG_PRINTLN("SSID de control encontrado. Conectando...");
  if (!connectQuick(PREFERRED_SSID, PREFERRED_PASSWORD, 10000)) {
    DEBUG_PRINTLN("No se pudo conectar al SSID de control.");
    mantenimiento = false;
    return false;
  }

  DEBUG_PRINT("Conectado. IP: "); DEBUG_PRINTLN(WiFi.localIP());

  String errA, errB;
  const char* chosenServer = nullptr;
  int chosenPort = 0;

  DEBUG_PRINT("Verificando broker MQTT (puerto por defecto)...");
  DEBUG_PRINT(mqtt_server_default);
  DEBUG_PRINT(":");
  DEBUG_PRINTLN(mqtt_port_default);
  if (canReachMqtt(mqtt_server_default, mqtt_port_default, 5000, &errA)) {
    chosenServer = mqtt_server_default;
    chosenPort   = mqtt_port_default;
    DEBUG_PRINTLN("MQTT OK por puerto default.");
  } else {
    DEBUG_PRINT("Default fallo: "); DEBUG_PRINTLN(errA);
    DEBUG_PRINTLN("Probando broker MQTT por puerto secundario...");
    if (canReachMqtt(mqtt_server_default, mqtt_port_secondary, 5000, &errB)) {
      chosenServer = mqtt_server_default;
      chosenPort   = mqtt_port_secondary;
      DEBUG_PRINTLN("MQTT OK por puerto secundario.");
    } else {
      DEBUG_PRINT("Secondary fallo: "); DEBUG_PRINTLN(errB);
      return false;
    }
  }
  
  // Configurar broker y callback
  mqttClient.setServer(
    (config.mqtt_server[0] ? config.mqtt_server : mqtt_server_default),
    (config.mqtt_port > 0  ? config.mqtt_port  : mqtt_port_default)
  );
  mqttClient.setCallback(callbackMQTTMant);

  // Forzar estado mantenimiento
  mantenimiento = true;
  currentState = STATE_MAINTENANCE;
  
  // Conectar MQTT y chequear estado
  if (!mqttClient.connect(ssidFL)) {
    DEBUG_PRINT("MQTT connect FALLO. state="); DEBUG_PRINTLN(mqttClient.state());
    return false;
  }

  if (mqttClient.connected()) {
    suscribeBootPacketMQTT();
    publishBootPacketMQTT();
    mantNextPublish = millis() + MANT_REPUBLISH_MS;
  }
  return true;
}

void handleMaintenance(unsigned long now) {
  // Si se perdió WiFi, reintentar a la preferida
  if (WiFi.status() != WL_CONNECTED) {
    connectQuick(PREFERRED_SSID, PREFERRED_PASSWORD, 8000);
    // si no conecta, te podés quedar intentando aquí; NO salgas al flujo normal
    return;
  }

  // Asegurar MQTT conectado
  if (!mqttClient.connected()) {
    if (!mqttClient.connect(ssidFL)) {
      // reconexión falló, reintentar luego
      return;
    }
    // resuscribir tras reconectar
    suscribeBootPacketMQTT();
    // publicar de nuevo
    publishBootPacketMQTT();
    mantNextPublish = now + MANT_REPUBLISH_MS;
  }

  // Procesar MQTT
  mqttClient.loop();

  // Timeout de espera de ACK (opcional: sólo informativo)
  if (mantEsperandoAck && now > mantDeadline) {
    DEBUG_PRINTLN("⏱️ Timeout esperando ACK (mant). Reintentando luego.");
    mantEsperandoAck = false;
  }

  // Re-publicar cada cierto tiempo si no hay ACK o como latido
  if (!mantEsperandoAck && now >= mantNextPublish) {
    publishBootPacketMQTT();
    mantNextPublish = now + MANT_REPUBLISH_MS;
  }

  // 🔒 Importante: NO cambies currentState aquí; te quedás en mantenimiento
}

//bool checkDixellConnection() {
//  DEBUG_PRINTLN("=== Iniciando verificación Dixell XR06CH ===");
//  struct Signal {
//    const char* nombre;
//    uint16_t direccion;
//    bool esHolding;
//  };
//
//  // Lista de señales que se intentarán leer XR06CH
//  Signal señales[] = {
//    {"Probe1",       257,  true},
//    {"SetPoint",     1539, true},
//    {"StatusOn",     512,  false},
//    {"Defrost",      513,  false},
//    {"PtaAbierta",   521,  false},
//    {"ErrorPb1",     532,  false},
//    {"ErrorPb2",     533,  false},
//    {"AltoValorPb1", 526,  false},
//    {"BajoValorPb1", 527,  false},
//    {"Compressor",   515,  false}
//  };
//
//  const int total = sizeof(señales) / sizeof(Signal);
//  int exitos = 0;
//  uint16_t data;
//
//  señalesOK = "";
//  señalesError = "";
//
//  for (int i = 0; i < total; i++) {
//    bool ok = false;
//    DEBUG_PRINT("=== Leyendo ");
//    DEBUG_PRINT(String(señales[i].nombre));
//    DEBUG_PRINT(": ");
//    
//    if (señales[i].esHolding) {
//      ok = readHoldingRegister(señales[i].direccion, data);
//    } else {
//      ok = readCoil(señales[i].direccion, data);
//    }
//
//    if (ok) {
//      DEBUG_PRINTLN("CORRECTO");
//      exitos++;
//      señalesOK += String(señales[i].nombre) + ",";
//    } else {
//      señalesError += String(señales[i].nombre) + ",";
//      DEBUG_PRINTLN("INCORRECTO");
//    }
//
//    delay(0);  // respetar tu delay definido
//  }
//
//  // Limpieza de las cadenas (remover última coma)
//  if (señalesOK.endsWith(",")) señalesOK.remove(señalesOK.length() - 1);
//  if (señalesError.endsWith(",")) señalesError.remove(señalesError.length() - 1);
//
//  // Determinar estado general
//  if (exitos == total) {
//    conexDixell = "CORRECTO";
//  } else if (exitos == 0) {
//    conexDixell = "INCORRECTO";
//  } else {
//    conexDixell = "PARCIAL";
//  }
//
//  DEBUG_PRINTLN("=== Resultado verificación Dixell XR06CH ===");
//  DEBUG_PRINTLN("Estado conexión: " + conexDixell);
//  DEBUG_PRINTLN("Señales OK: " + señalesOK);
//  DEBUG_PRINTLN("Señales con error: " + señalesError);
//  DEBUG_PRINTLN("=====================================");
//
//  return conexDixell == "CORRECTO";
//}

String getFormattedUptime() {
  unsigned long currentTime = millis();
  unsigned long seconds = currentTime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;

  seconds %= 60;
  minutes %= 60;
  hours %= 24;

  String uptime = "";
  if (days > 0) {
    uptime += String(days) + (days == 1 ? " día, " : " días, ");
  }
  if (hours > 0) {
    uptime += String(hours) + (hours == 1 ? " hora, " : " horas, ");
  }
  if (minutes > 0) {
    uptime += String(minutes) + (minutes == 1 ? " minuto y " : " minutos y ");
  }
  
  // Si no hay días, horas ni minutos, solo mostramos los segundos
  if (uptime.length() == 0) {
      uptime += String(seconds) + (seconds == 1 ? " segundo" : " segundos");
  } else {
      // Si hay algo más, quitamos la última " y " y la reemplazamos por " y " antes de los segundos
      uptime += String(seconds) + (seconds == 1 ? " segundo" : " segundos");
  }

  return uptime;
}

String cleanString(const char* str) {
  String result = "";
  for (int i = 0; i < strlen(str); i++) {
    char c = str[i];
    if (c >= 32 && c <= 126) { // Solo caracteres imprimibles ASCII
      result += c;
    } else {
      result += ' '; // Reemplazar caracteres no imprimibles con espacio
    }
  }
  return result;
}


//En configuracion testea que el dispositivo de frio este bien conectado
void startNewVerification() {
  DEBUG_PRINTLN("=== Iniciando verificación Modbus genérica ===");
  verificationState = VER_PROBE1;
  verificationSuccessCount = 0;
  verificationTotalCount = 0;
  verificationResults = "";
  verificationInProgress = true;
  lastVerificationTime = millis();
  
  // Resetear resultados
  verResult = VerificationResult();
  
  // CORRECCIÓN: Limpiar las cadenas globales para evitar acumulación
  señalesOK = "";
  señalesError = "";
}

void updateVerificationStatus(const char* signalName, bool result) {
  DEBUG_PRINT("Verificando ");
  DEBUG_PRINT(signalName);
  DEBUG_PRINT(": ");
  
  if (result) {
    DEBUG_PRINTLN("CORRECTO");
    verificationSuccessCount++;
  } else {
    DEBUG_PRINTLN("INCORRECTO");
  }
  
  verificationTotalCount++;
  
  // Actualizar cadenas de resultados
  if (result) {
    if (señalesOK.length() > 0) señalesOK += ",";
    señalesOK += signalName;
  } else {
    if (señalesError.length() > 0) señalesError += ",";
    señalesError += signalName;
  }
}

void finalizeVerification() {
  DEBUG_PRINTLN("=== Resultado verificación Modbus ===");
  
  // Determinar estado general
  if (verificationSuccessCount == verificationTotalCount) {
    conexDixell = "CORRECTO";
  } else if (verificationSuccessCount == 0) {
    conexDixell = "INCORRECTO";
  } else {
    conexDixell = "PARCIAL";
  }
  
  DEBUG_PRINTLN("Estado conexión: " + conexDixell);
  DEBUG_PRINTLN("Señales OK: " + señalesOK);
  DEBUG_PRINTLN("Señales con error: " + señalesError);
  DEBUG_PRINTLN("=====================================");
  
  verificationInProgress = false;
  verificationState = VER_IDLE;
}

// Función pública para iniciar verificación
void startConnectionVerification() {
  // Resetear variables globales
  señalesOK = "";
  señalesError = "";
  verificationInProgress = false;
  verificationState = VER_IDLE;

  // Inicia la verificación
  startNewVerification();
}

// Función para verificar si la verificación está completa
bool isVerificationComplete() {
  return !verificationInProgress && (conexDixell != "SINREVISAR");
}

bool checkModbusConnectionNonBlocking() {
  // Si no hay verificación en curso, iniciar una nueva
  if (!verificationInProgress) {
    return false; // Devuelve false porque aún no hay resultado
  }
  
  // Si no ha pasado suficiente tiempo desde la última verificación, salir
  if (millis() - lastVerificationTime < msEntreSolicitudes) {
    return false;
  }
  
  uint16_t data;
  bool currentResult = false;
  
  // Máquina de estados de verificación
  switch (verificationState) {
    case VER_PROBE1:
      currentResult = readModbus(config.func_Probe1, config.addr_Probe1, data);
      verResult.ok_Probe1 = currentResult;
      updateVerificationStatus("Probe1", currentResult);
      verificationState = VER_SETPOINT;
      break;
      
    case VER_SETPOINT:
      currentResult = readModbus(config.func_SetPoint, config.addr_SetPoint, data);
      verResult.ok_SetPoint = currentResult;
      updateVerificationStatus("SetPoint", currentResult);
      verificationState = VER_STATUSON;
      break;
      
    case VER_STATUSON:
      currentResult = readModbus(config.func_StatusOn, config.addr_StatusOn, data);
      verResult.ok_StatusOn = currentResult;
      updateVerificationStatus("StatusOn", currentResult);
      verificationState = VER_DEFROST;
      break;
      
    case VER_DEFROST:
      currentResult = readModbus(config.func_Defrost, config.addr_Defrost, data);
      verResult.ok_Defrost = currentResult;
      updateVerificationStatus("Defrost", currentResult);
      verificationState = VER_PTAABIERTA;
      break;
      
    case VER_PTAABIERTA:
      currentResult = readModbus(config.func_PtaAbierta, config.addr_PtaAbierta, data);
      verResult.ok_PtaAbierta = currentResult;
      updateVerificationStatus("PtaAbierta", currentResult);
      verificationState = VER_ERRORPB1;
      break;
      
    case VER_ERRORPB1:
      currentResult = readModbus(config.func_ErrorPb1, config.addr_ErrorPb1, data);
      verResult.ok_ErrorPb1 = currentResult;
      updateVerificationStatus("ErrorPb1", currentResult);
      verificationState = VER_ERRORPB2;
      break;
      
    case VER_ERRORPB2:
      currentResult = readModbus(config.func_ErrorPb2, config.addr_ErrorPb2, data);
      verResult.ok_ErrorPb2 = currentResult;
      updateVerificationStatus("ErrorPb2", currentResult);
      verificationState = VER_ALTOVALORPB1;
      break;
      
    case VER_ALTOVALORPB1:
      currentResult = readModbus(config.func_AltoValorPb1, config.addr_AltoValorPb1, data);
      verResult.ok_AltoValorPb1 = currentResult;
      updateVerificationStatus("AltoValorPb1", currentResult);
      verificationState = VER_BAJOVALORPB1;
      break;
      
    case VER_BAJOVALORPB1:
      currentResult = readModbus(config.func_BajoValorPb1, config.addr_BajoValorPb1, data);
      verResult.ok_BajoValorPb1 = currentResult;
      updateVerificationStatus("BajoValorPb1", currentResult);
      verificationState = VER_COMPRESSOR;
      break;
      
    case VER_COMPRESSOR:
      currentResult = readModbus(config.func_Compressor, config.addr_Compressor, data);
      verResult.ok_Compressor = currentResult;
      updateVerificationStatus("Compressor", currentResult);
      verificationState = VER_COMPLETE;
      break;
      
    case VER_COMPLETE:
      finalizeVerification();
      return true; // Verificación completada
      
    default:
      verificationState = VER_IDLE;
      return false;
  }
  
  lastVerificationTime = millis();
  return false;
}

String getDiagnosticDataAsJson() {
  char jsonBuffer[1536]; // Un buffer más pequeño y ajustado es suficiente
  
  // Empezamos a construir el JSON
  snprintf(jsonBuffer, sizeof(jsonBuffer), "{");

  DEBUG_PRINTLN("--- Sección 1: Información del Sistema ---");
  int len = strlen(jsonBuffer);
  snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len,
    "\"info\":{"
      "\"UT\":\"%s\","
      "\"Last\":%lu,"
      "\"State\":\"Operación Normal\""
    "},",
    getFormattedUptime().c_str(),
    (millis() - lastMqttSend) / 1000
  );

  DEBUG_PRINTLN("--- Sección 2: Información de Red ---");
  len = strlen(jsonBuffer);
  snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len,
    "\"network\":{"
      "\"cIP\":\"%s\","
      "\"aIP\":\"%s\","
      "\"wName\":\"%s\","
      "\"wRSSI\":%d"
    "},",
    WiFi.localIP().toString().c_str(),
    WiFi.softAPIP().toString().c_str(),
    config.stored_ssid,
    WiFi.RSSI()
  );

  DEBUG_PRINTLN("--- Sección 3: Datos de Sensores (usando una macro más pequeña) ---");
  len = strlen(jsonBuffer);
  snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len, "\"sensors\":[");

  DEBUG_PRINTLN("Empieza a leer las variables");
  #define ADD_SENSOR_JSON(name, addr_ptr, func_enum, val_ptr) \
  do { \
    if (strlen(jsonBuffer) >= (sizeof(jsonBuffer) - 100)) { \
      DEBUG_PRINTLN("ERROR: Buffer JSON casi lleno"); \
      break; \
    } \
    len = strlen(jsonBuffer); \
    snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len, \
      "{\"n\":\"%s\",\"a\":%u,\"f\":%d,\"v\":\"%s\",\"e\":\"%s\"},", \
      (name), \
      (unsigned int)(addr_ptr), \
      (int)(func_enum), \
      (val_ptr) ? (val_ptr) : "Error", \
      ((val_ptr) && strcmp(val_ptr, "Error") == 0) ? "error" : "ok"); \
  } while(0)

  #define ADD_SPECIAL_FIELD(name, value) \
  do { \
    if (strlen(jsonBuffer) >= (sizeof(jsonBuffer) - 100)) { \
      DEBUG_PRINTLN("ERROR: Buffer JSON casi lleno"); \
      break; \
    } \
    len = strlen(jsonBuffer); \
    snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len, \
      "{\"n\":\"%s\",\"a\":0,\"f\":0,\"v\":\"%s\",\"e\":\"ok\"},", \
      (name), (value)); \
  } while(0)

  DEBUG_PRINTLN("--- Sección 4: leyendo datos de los sensores) ---");
  ADD_SENSOR_JSON("Probe1", config.addr_Probe1, config.func_Probe1, config.Probe1);
  ADD_SENSOR_JSON("SetPoint", config.addr_SetPoint, config.func_SetPoint, config.SetPoint);
  ADD_SENSOR_JSON("StatusOn", config.addr_StatusOn, config.func_StatusOn, config.StatusOn);
  ADD_SENSOR_JSON("Defrost", config.addr_Defrost, config.func_Defrost, config.Defrost);
  ADD_SENSOR_JSON("PtaAbierta", config.addr_PtaAbierta, config.func_PtaAbierta, config.PtaAbierta);
  ADD_SENSOR_JSON("Compressor", config.addr_Compressor, config.func_Compressor, config.Compressor);
  ADD_SENSOR_JSON("ErrorPb1", config.addr_ErrorPb1, config.func_ErrorPb1, config.ErrorPb1);
  ADD_SENSOR_JSON("ErrorPb2", config.addr_ErrorPb2, config.func_ErrorPb2, config.ErrorPb2);
  ADD_SENSOR_JSON("AltoValorPb1", config.addr_AltoValorPb1, config.func_AltoValorPb1, config.AltoValorPb1);
  ADD_SENSOR_JSON("BajoValorPb1", config.addr_BajoValorPb1, config.func_BajoValorPb1, config.BajoValorPb1);
  ADD_SENSOR_JSON("w_SetPoint", config.addr_w_SetPoint, config.func_w_SetPoint, "N/A");
  ADD_SENSOR_JSON("w_Status", config.addr_w_Status, config.func_w_Status, "N/A");
  ADD_SPECIAL_FIELD("lic_activa", config.licenciaActual);

  // Eliminar la última coma del array de sensores
  DEBUG_PRINTLN("Elimina la coma");
  len = strlen(jsonBuffer);
  if (jsonBuffer[len - 1] == ',') {
    jsonBuffer[len - 1] = '\0';
  }

  DEBUG_PRINTLN("Cierra el array");
  // Cerrar el array de sensores y el objeto principal
  len = strlen(jsonBuffer);
  snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len, "]}");

  DEBUG_PRINTLN("JSON construido, longitud: " + String(strlen(jsonBuffer)));
  DEBUG_PRINT("JSON A enviar");
  DEBUG_PRINTLN(jsonBuffer);
  return String(jsonBuffer);
}

ModbusPingResult probeDixellRaw(DixellRawFingerprint &fp) {
  uint8_t  result;
  uint16_t raw;
  uint32_t t0;

  // Limpieza inicial
  memset(&fp, 0, sizeof(fp));
  fp.familyCode = 0xFF;
  fp.firmware   = 0xFF;
  fp.baudrate   = 9600;

  modbusSerial.listen();

  // ==================================================
  // PROBE 1: Holding Registers 0..1 (ID principal)
  // ==================================================
  t0 = millis();
  result = node.readHoldingRegisters(0, 1);
  fp.responseTimeMs = millis() - t0;

  if (result == node.ku8MBSuccess) {
    raw = node.getResponseBuffer(0);
    fp.familyCode   = (raw >> 8) & 0xFF;
    fp.firmware     = raw & 0xFF;
    fp.reg0Readable = true;
  } else {
    fp.lastException = result;
  }

  delay(150); // XR75CH necesita más aire

  // ==================================================
  // PROBE 3: Setpoint (registro funcional común)
  // ==================================================
  t0 = millis();
  result = node.readHoldingRegisters(8, 1);
  fp.responseTimeMs = millis() - t0;

  if (result == node.ku8MBSuccess) {
    fp.setpointReadable = true;
  } else {
    fp.lastException = result;
  }

  delay(100);

  // ==================================================
  // PROBE 4: Input Registers genéricos (capacidad)
  // ==================================================
  result = node.readInputRegisters(1, 1);
  if (result == node.ku8MBSuccess) {
    fp.inputRegReadable = true;
  }

  // ==================================================
  // RESULTADO GLOBAL
  // ==================================================
  if (fp.reg0Readable || fp.setpointReadable) {
    return MB_PING_OK;
  }

  if (result == node.ku8MBResponseTimedOut ||
      result == node.ku8MBInvalidCRC) {
    return MB_PING_TIMEOUT;
  }

  if (result == node.ku8MBSlaveDeviceFailure) {
    return MB_PING_SLAVE_ERROR;
  }

  return MB_PING_ERROR;
}

bool getDixellFingerprintCached(DixellRawFingerprint &outFp) {

  // Ya está cacheado
  if (g_dixellCached) {
    outFp = g_dixellFp;
    return true;
  }

  // No cacheado → probe real
  ModbusPingResult r = probeDixellRaw(g_dixellFp);

  if (r == MB_PING_OK) {
    g_dixellCached = true;
    g_dixellCachedAt = millis();
    outFp = g_dixellFp;
    return true;
  }

  return false;
}

String getDixellFingerprintAsJson() {
  DixellRawFingerprint fp;

  if (!getDixellFingerprintCached(fp)) {
    return "{\"status\":\"unknown\"}";
  }

  String json = "{";
  json += "\"status\":\"ok\",";
  json += "\"familyCode\":" + String(fp.familyCode) + ",";
  json += "\"firmware\":" + String(fp.firmware) + ",";
  json += "\"reg0Readable\":" + String(fp.reg0Readable ? "true" : "false") + ",";
  json += "\"setpointReadable\":" + String(fp.setpointReadable ? "true" : "false") + ",";
  json += "\"inputRegReadable\":" + String(fp.inputRegReadable ? "true" : "false") + ",";
  json += "\"responseTimeMs\":" + String(fp.responseTimeMs);
  json += "}";

  return json;
}
