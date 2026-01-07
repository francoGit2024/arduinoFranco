// ======================== FUNCIONES DE UTILIDAD ========================
#include <ArduinoJson.h>

void resetWatchdog() {
  ESP.wdtFeed();
}

uint32_t calculateChecksum(const ConfigData* data) {
  uint32_t crc = 0xFFFFFFFF;
  const uint8_t* ptr = (const uint8_t*)data;
  size_t len = sizeof(ConfigData) - sizeof(uint32_t);

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
  cuentaErroresTotales = cuentaErroresTotales + 1;
  if (cuentaErroresTotales > 10) {
    resetDispositivo();
  }
  DEBUG_PRINT("Cuenta error total: ");
  DEBUG_PRINTLN(cuentaErroresTotales);
}

void actLicencia(const char* value){
  strncpy(config.licenciaActual, value, sizeof(config.licenciaActual)-1);
  config.licenciaActual[sizeof(config.licenciaActual)-1] = '\0';
  saveConfigToEEPROM();
  DEBUG_PRINT("Se actualizó la licencia a: ");
  DEBUG_PRINTLN(value);
}

void actTiempoIntervalo(const char* value){
  strncpy(config.tiempoIntervalo, value, sizeof(config.tiempoIntervalo)-1);
  config.tiempoIntervalo[sizeof(config.tiempoIntervalo)-1] = '\0';
  saveConfigToEEPROM();
  MQTT_SEND_INTERVAL = strtoul(config.tiempoIntervalo, nullptr, 10);
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

  strncpy(config.tokenDispositivo, tokenDefecto, sizeof(config.tokenDispositivo) - 1);
  config.tokenDispositivo[sizeof(config.tokenDispositivo) - 1] = '\0';
  strncpy(config.stored_ssid, ssidDefecto, sizeof(config.stored_ssid) - 1);
  config.stored_ssid[sizeof(config.stored_ssid) - 1] = '\0';
  strncpy(config.stored_password, passWIFI, sizeof(config.stored_password) - 1);
  config.stored_password[sizeof(config.stored_password) - 1] = '\0';
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
  config.mqtt_port = mqtt_port_default;
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
  config.addr_BajoValorPb1 = addr_BajoValorPb1_default;
  config.addr_Compressor = addr_Compressor_default;
  config.addr_w_SetPoint = addr_w_SetPoint_default;
  config.addr_w_Status = addr_w_Status_default;
  strncpy(config.passDiag, passDiag_default, sizeof(config.passDiag) - 1);
  config.passDiag[sizeof(config.passDiag) - 1] = '\0';
  config.useStaticIP = useStaticIP_default;
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
  config.func_BajoValorPb1 = func_BajoValorPb1_default;
  config.func_Compressor = func_Compressor_default;
  config.func_w_SetPoint = func_w_SetPoint_default;
  config.func_w_Status = func_w_Status_default;
  saveConfigToEEPROM();

  DEBUG_PRINTLN("Reset completado. Reiniciando...");
  delay(1000);
  ESP.restart();
}

static inline String trimSafe(const String& s){ String t=s; t.trim(); return t; }

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

static inline bool isEmailBasic(const String& e){
  int at = e.indexOf('@'); int dot = e.lastIndexOf('.');
  return at>0 && dot>at+1 && dot < (int)e.length()-1;
}

static inline String keepDigitsPlus(const String& in){
  String o; o.reserve(in.length());
  for (size_t i=0;i<in.length();++i){
    char c=in[i];
    if ((c>='0'&&c<='9') || c=='+') o+=c;
  }
  return o;
}

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
  int eepromSize = 512;
  for (int i = 0; i < eepromSize; i++) {
    EEPROM.write(i, 0xFF);
  }
  EEPROM.commit();
  Serial.println("EEPROM borrada");
}

bool isSSIDAvailable(const char* ssid) {
  int n = WiFi.scanNetworks(false, true);
  if (n <= 0) return false;
  for (int i = 0; i < n; i++) {
    if (WiFi.SSID(i) == ssid) return true;
  }
  return false;
}

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

bool publishBootPacketMQTT() {
  DixellRawFingerprint fp;
  bool dixellOk = getDixellFingerprintCached(fp);

  int fc = dixellOk ? fp.familyCode : -1;
  int fi = dixellOk ? fp.firmware   : -1;

  uint8_t result;
  uint16_t data;

  DEBUG_PRINT("Leyendo Probe1 en mant.: ");
  if (readHoldingRegister(config.addr_Probe1, data)){
    snprintf(config.Probe1, sizeof(config.Probe1), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    DEBUG_PRINTLN("ERROR");
    snprintf(config.Probe1, sizeof(config.Probe1), "Error");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo SetPoint en mant.: ");
  if (readHoldingRegister(config.addr_SetPoint, data)){
    snprintf(config.SetPoint, sizeof(config.SetPoint), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.SetPoint, sizeof(config.SetPoint), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo StatusOn en mant.: ");
  if (readCoil(config.addr_StatusOn, data)){
    snprintf(config.StatusOn, sizeof(config.StatusOn), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.StatusOn, sizeof(config.StatusOn), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo Defrost en mant.: ");
  if (readCoil(config.addr_Defrost, data)){
    snprintf(config.Defrost, sizeof(config.Defrost), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.Defrost, sizeof(config.Defrost), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo PtaAbierta en mant.: ");
  if (readCoil(config.addr_PtaAbierta, data)){
    snprintf(config.PtaAbierta, sizeof(config.PtaAbierta), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.PtaAbierta, sizeof(config.PtaAbierta), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo ErrorPb1 en mant.: ");
  if (readCoil(config.addr_ErrorPb1, data)){
    snprintf(config.ErrorPb1, sizeof(config.ErrorPb1), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.ErrorPb1, sizeof(config.ErrorPb1), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo ErrorPb2 en mant.: ");
  if (readCoil(config.addr_ErrorPb2, data)){
    snprintf(config.ErrorPb2, sizeof(config.ErrorPb2), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.ErrorPb2, sizeof(config.ErrorPb2), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo AltoValorPb1 en mant.: ");
  if (readCoil(config.addr_AltoValorPb1, data)){
    snprintf(config.AltoValorPb1, sizeof(config.AltoValorPb1), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.AltoValorPb1, sizeof(config.AltoValorPb1), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo BajoValorPb1 en mant.: ");
  if (readCoil(config.addr_BajoValorPb1, data)){
    snprintf(config.BajoValorPb1, sizeof(config.BajoValorPb1), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.BajoValorPb1, sizeof(config.BajoValorPb1), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

  DEBUG_PRINT("Leyendo Compressor en mant.: ");
  if (readCoil(config.addr_Compressor, data)){
    snprintf(config.Compressor, sizeof(config.Compressor), "%u", data);
    DEBUG_PRINTLN("CORRECTO");
  } else {
    snprintf(config.Compressor, sizeof(config.Compressor), "Error");
    DEBUG_PRINTLN("ERROR");
  }
  delay(msEntreSolicitudes);

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
    HVERSION,
    fc,
    fi,
    VERSION
  );

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

    mqttClient.setServer(
      (config.mqtt_server[0] ? config.mqtt_server : mqtt_server_default),
      (config.mqtt_port > 0  ? config.mqtt_port  : mqtt_port_default)
    );
    mqttClient.setCallback(callbackMQTTMant);

    mantenimiento = true;
    currentState = STATE_MAINTENANCE;

    if (!mqttClient.connect(ssidFL)) {
      DEBUG_PRINT("MQTT connect FALLO. state="); DEBUG_PRINTLN(mqttClient.state());
      return false;
    }

    if (mqttClient.connected()) {
      suscribeBootPacketMQTT();
    }
  }

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

String getFormattedUptime() {
  unsigned long currentTime = millis();
  unsigned long seconds = currentTime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;

  seconds %= 60;
  minutes %= 60;
  hours %= 24;

  char buf[128];
  buf[0] = '\0';
  size_t pos = 0;

  if (days > 0) {
    pos += snprintf(buf + pos, sizeof(buf) - pos, "%lu %s, ", days, (days == 1) ? "día" : "días");
  }
  if (hours > 0) {
    pos += snprintf(buf + pos, sizeof(buf) - pos, "%lu %s, ", hours, (hours == 1) ? "hora" : "horas");
  }
  if (minutes > 0) {
    pos += snprintf(buf + pos, sizeof(buf) - pos, "%lu %s y ", minutes, (minutes == 1) ? "minuto" : "minutos");
  }

  if (pos == 0) {
    snprintf(buf + pos, sizeof(buf) - pos, "%lu %s", seconds, (seconds == 1) ? "segundo" : "segundos");
  } else {
    snprintf(buf + pos, sizeof(buf) - pos, "%lu %s", seconds, (seconds == 1) ? "segundo" : "segundos");
  }

  return String(buf);
}

String getDixellFingerprintAsJson() {
  DixellRawFingerprint fp;

  if (!getDixellFingerprintCached(fp)) {
    return String("{\"status\":\"unknown\"}");
  }

  StaticJsonDocument<256> doc;
  doc["status"] = "ok";
  doc["familyCode"] = fp.familyCode;
  doc["firmware"] = fp.firmware;
  doc["reg0Readable"] = fp.reg0Readable ? true : false;
  doc["setpointReadable"] = fp.setpointReadable ? true : false;
  doc["inputRegReadable"] = fp.inputRegReadable ? true : false;
  doc["responseTimeMs"] = fp.responseTimeMs;

  char buf[256];
  size_t n = serializeJson(doc, buf, sizeof(buf));
  (void)n;
  return String(buf);
}

String getDiagnosticDataAsJson() {
  StaticJsonDocument<2048> doc;

  JsonObject info = doc.createNestedObject("info");
  info["UT"] = getFormattedUptime();
  info["Last"] = (millis() - lastMqttSend) / 1000;
  info["State"] = (currentState == STATE_NORMAL_OPERATION) ? "Operación Normal" : "Otro";

  JsonObject net = doc.createNestedObject("network");
  net["cIP"] = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : String("No conectado");
  net["aIP"] = WiFi.softAPIP().toString();
  net["wName"] = config.stored_ssid;
  net["wRSSI"] = WiFi.RSSI();

  JsonArray sensors = doc.createNestedArray("sensors");

  auto addSensor = [&](const char* name, uint16_t addr, ModbusFunc f, const char* val, const char* estado){
    JsonObject s = sensors.createNestedObject();
    s["n"] = name;
    s["a"] = addr;
    s["f"] = (int)f;
    s["v"] = val ? val : "Error";
    s["e"] = estado ? estado : "ok";
  };

  addSensor("Probe1", config.addr_Probe1, config.func_Probe1, config.Probe1[0] ? config.Probe1 : "Error", (config.Probe1[0] && strcmp(config.Probe1, "Error")!=0) ? "ok" : "error");
  addSensor("SetPoint", config.addr_SetPoint, config.func_SetPoint, config.SetPoint[0] ? config.SetPoint : "Error", (config.SetPoint[0] && strcmp(config.SetPoint, "Error")!=0) ? "ok" : "error");
  addSensor("StatusOn", config.addr_StatusOn, config.func_StatusOn, config.StatusOn[0] ? config.StatusOn : "Error", (config.StatusOn[0] && strcmp(config.StatusOn, "Error")!=0) ? "ok" : "error");
  addSensor("Defrost", config.addr_Defrost, config.func_Defrost, config.Defrost[0] ? config.Defrost : "Error", (config.Defrost[0] && strcmp(config.Defrost, "Error")!=0) ? "ok" : "error");
  addSensor("PtaAbierta", config.addr_PtaAbierta, config.func_PtaAbierta, config.PtaAbierta[0] ? config.PtaAbierta : "Error", (config.PtaAbierta[0] && strcmp(config.PtaAbierta, "Error")!=0) ? "ok" : "error");
  addSensor("Compressor", config.addr_Compressor, config.func_Compressor, config.Compressor[0] ? config.Compressor : "Error", (config.Compressor[0] && strcmp(config.Compressor, "Error")!=0) ? "ok" : "error");
  addSensor("ErrorPb1", config.addr_ErrorPb1, config.func_ErrorPb1, config.ErrorPb1[0] ? config.ErrorPb1 : "Error", (config.ErrorPb1[0] && strcmp(config.ErrorPb1, "Error")!=0) ? "ok" : "error");
  addSensor("ErrorPb2", config.addr_ErrorPb2, config.func_ErrorPb2, config.ErrorPb2[0] ? config.ErrorPb2 : "Error", (config.ErrorPb2[0] && strcmp(config.ErrorPb2, "Error")!=0) ? "ok" : "error");
  addSensor("AltoValorPb1", config.addr_AltoValorPb1, config.func_AltoValorPb1, config.AltoValorPb1[0] ? config.AltoValorPb1 : "Error", (config.AltoValorPb1[0] && strcmp(config.AltoValorPb1, "Error")!=0) ? "ok" : "error");
  addSensor("BajoValorPb1", config.addr_BajoValorPb1, config.func_BajoValorPb1, config.BajoValorPb1[0] ? config.BajoValorPb1 : "Error", (config.BajoValorPb1[0] && strcmp(config.BajoValorPb1, "Error")!=0) ? "ok" : "error");
  addSensor("w_SetPoint", config.addr_w_SetPoint, config.func_w_SetPoint, "N/A", "ok");
  addSensor("w_Status", config.addr_w_Status, config.func_w_Status, "N/A", "ok");

  JsonObject lic = doc.createNestedObject("lic");
  lic["lic_activa"] = config.licenciaActual;

  static char outBuf[1536];
  size_t n = serializeJson(doc, outBuf, sizeof(outBuf));
  (void)n;
  return String(outBuf);
}

