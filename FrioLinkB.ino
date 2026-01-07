#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

// Información del dispositivo
const char* ssidFL="FLB1089";
const char* codModelo = "friolinkB";
const char* modeloFL = "Friolink-B";
const char* VERSION = "3.0.1";
const char* HVERSION = "3.0.H5C";

#define REEMPLAZO 0
#define DEBUG_MODE 1

// ======================== CONFIGURACIÓN Y CONSTANTES ========================
#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// Configuración de red
#if REEMPLAZO
  #define ssidDefecto "noserednovalida"
  #define passWIFI "12345612_$"
  #define tokenDefecto "00000000000000000000AAAA"
#else
  #define ssidDefecto ""
  #define passWIFI ""
  #define tokenDefecto ""
#endif
#define urlBase "http://srvtablero.orionsi.ar:8080/api/"
#define mqtt_server_default "mqtt.orionsi.com.ar"
#define mqtt_port_default 38483
#define mqtt_port_secondary 1883
#define licActual_default "1"
#define tiempoIntervalo_default "10000"
#define passDiag_default "orion1234"
#define useStaticIP_default false
#define staticIP_default "0.0.0.0"
#define staticGateway_default "0.0.0.0"
#define staticSubnet_default "255.255.255.0"
#define staticDNS_default "0.0.0.0"
#define EEPROM_MAGIC 0x46524C42u
#define EEPROM_VERSION 1

////Direcciones por defecto Dixell XR06CH
#define addr_Probe1_default 257
#define addr_SetPoint_default 1539
#define addr_StatusOn_default 512
#define addr_Defrost_default 513
#define addr_PtaAbierta_default 521
#define addr_ErrorPb1_default 532
#define addr_ErrorPb2_default 533
#define addr_AltoValorPb1_default 526
#define addr_BajoValorPb1_default 527
#define addr_Compressor_default 515
#define addr_w_SetPoint_default 1539
#define addr_w_Status_default 512

// Timeouts y intervalos (en milisegundos)
const unsigned long WIFI_TIMEOUT = 10000; //10s
const unsigned long WIFI_CHECK_INTERVAL = 30000; //30s
unsigned long MQTT_SEND_INTERVAL = 10000; //60s
const unsigned long SENSOR_READ_INTERVAL = 5000; //5s
unsigned long CONFIG_MODE_TIMEOUT = 120000; //2min

/*Para mantenimiento*/
const char* PREFERRED_SSID     = "APPControlOrionSI";
const char* PREFERRED_PASSWORD = "12345678";
bool mantenimiento = false;
bool mantEsperandoAck = false;
unsigned long mantDeadline = 0;
const unsigned long MANT_TIMEOUT_MS = 5000;
const unsigned long MANT_REPUBLISH_MS = 5000;
unsigned long mantNextPublish = 0;
String conexDixell = "SINREVISAR";
String señalesOK = "";
String señalesError = "";

// ======================== PARA DETECTAR FAMILIA Y VERSIONES ===============================================
enum ModbusPingResult {
  MB_PING_OK = 0,
  MB_PING_SLAVE_ERROR,
  MB_PING_TIMEOUT,
  MB_PING_ERROR
};

struct DixellRawFingerprint {
  uint8_t  familyCode;
  uint8_t  firmware;

  bool     reg0Readable;
  bool     setpointReadable;
  bool     inputRegReadable;

  uint8_t  lastException;
  uint16_t responseTimeMs;

  uint32_t baudrate;
};

DixellRawFingerprint g_dixellFp;
bool g_dixellCached = false;
unsigned long g_dixellCachedAt = 0;

// Estados del sistema
enum SystemState {
  STATE_CONFIG_MODE,
  STATE_NORMAL_OPERATION,
  STATE_WIFI_RECONNECT,
  STATE_MQTT_RECONNECT,
  STATE_MAINTENANCE,
  STATE_DIXELL_ERROR
};

// Códigos de error
enum ErrorCode {
  ERROR_NONE = 0,
  ERROR_WIFI_TIMEOUT,
  ERROR_MQTT_FAILED,
  ERROR_SENSOR_READ_FAILED,
  ERROR_CONFIG_TIMEOUT
};

// Verificación Modbus
enum VerificationState {
  VER_IDLE,
  VER_PROBE1,
  VER_SETPOINT,
  VER_STATUSON,
  VER_DEFROST,
  VER_PTAABIERTA,
  VER_ERRORPB1,
  VER_ERRORPB2,
  VER_ALTOVALORPB1,
  VER_BAJOVALORPB1,
  VER_COMPRESSOR,
  VER_COMPLETE
};

VerificationState verificationState = VER_IDLE;
unsigned long lastVerificationTime = 0;
int verificationSuccessCount = 0;
int verificationTotalCount = 0;
String verificationResults = "";
bool verificationInProgress = false;

struct VerificationResult {
  bool ok_Probe1 = false;
  bool ok_SetPoint = false;
  bool ok_StatusOn = false;
  bool ok_Defrost = false;
  bool ok_PtaAbierta = false;
  bool ok_ErrorPb1 = false;
  bool ok_ErrorPb2 = false;
  bool ok_AltoValorPb1 = false;
  bool ok_BajoValorPb1 = false;
  bool ok_Compressor = false;
};

VerificationResult verResult;

// ======================== VARIABLES GLOBALES ========================
SystemState currentState = STATE_NORMAL_OPERATION;
ErrorCode lastError = ERROR_NONE;
bool config_mode = false;

char jsonBuffer[256];
char topicBuffer[128];
char ap_ssid[32];
char ap_diag_ssid[32];

enum ModbusFunc : uint8_t {
  FC01_COIL = 1,
  FC02_DI   = 2,
  FC03_HOLD = 3,
  FC04_INPUT = 4,
  FC05_WSC = 5,
  FC06_WSR = 6
};

struct EepromHeader {
  uint32_t magic;
  uint16_t version;
};

struct ConfigData {
  char Probe1[64];
  char SetPoint[64];
  char StatusOn[64];
  char Defrost[64];
  char PtaAbierta[64];
  char ErrorPb1[64];
  char ErrorPb2[64];
  char AltoValorPb1[64];
  char BajoValorPb1[64];
  char Compressor[64];
  char tokenDispositivo[64];
  char stored_ssid[64];
  char stored_password[64];
  char mqtt_server[64];
  int mqtt_port;
  char licenciaActual[5];
  char tiempoIntervalo[32];
  uint16_t addr_Probe1;
  uint16_t addr_SetPoint;
  uint16_t addr_StatusOn;
  uint16_t addr_Defrost;
  uint16_t addr_PtaAbierta;
  uint16_t addr_ErrorPb1;
  uint16_t addr_ErrorPb2;
  uint16_t addr_AltoValorPb1;
  uint16_t addr_BajoValorPb1;
  uint16_t addr_Compressor;
  uint16_t addr_w_SetPoint;
  uint16_t addr_w_Status;
  char passDiag[32];
  bool useStaticIP;
  char staticIP[16];
  char staticGateway[16];
  char staticSubnet[16];
  char staticDNS[16];
  ModbusFunc func_Probe1;
  ModbusFunc func_SetPoint;
  ModbusFunc func_StatusOn;
  ModbusFunc func_Defrost;
  ModbusFunc func_PtaAbierta;
  ModbusFunc func_ErrorPb1;
  ModbusFunc func_ErrorPb2;
  ModbusFunc func_AltoValorPb1;
  ModbusFunc func_BajoValorPb1;
  ModbusFunc func_Compressor;
  ModbusFunc func_w_SetPoint;
  ModbusFunc func_w_Status;
  uint32_t checksum;
};

enum ModbusReadState {
  READING_PROBE1,
  READING_SETPOINT,
  READING_STATUSON,
  READING_DEFROST,
  READING_PTAABIERTA,
  READING_ERRORPB1,
  READING_ERRORPB2,
  READING_ALTOVALORPB1,
  READING_BAJOVALORPB1,
  READING_COMPRESSOR,
  ALL_READINGS_DONE
};

struct SensorReadStatus {
  bool ok_Probe1 = false;
  bool ok_SetPoint = false;
  bool ok_StatusOn = false;
  bool ok_Defrost = false;
  bool ok_PtaAbierta = false;
  bool ok_ErrorPb1 = false;
  bool ok_ErrorPb2 = false;
  bool ok_AltoValorPb1 = false;
  bool ok_BajoValorPb1 = false;
  bool ok_Compressor = false;
  bool cycleComplete = false;
};

// Configuración inicializada a cero; loadConfigFromEEPROM aplicará defaults si corresponde
ConfigData config = {};

static const char* mqttRcToText(int rc) {
  switch (rc) {
    case MQTT_CONNECTED:              return "MQTT_CONNECTED";
    case MQTT_CONNECT_BAD_PROTOCOL:   return "MQTT_CONNECT_BAD_PROTOCOL";
    case MQTT_CONNECT_BAD_CLIENT_ID:  return "MQTT_CONNECT_BAD_CLIENT_ID";
    case MQTT_CONNECT_UNAVAILABLE:    return "MQTT_CONNECT_UNAVAILABLE";
    case MQTT_CONNECT_BAD_CREDENTIALS:return "MQTT_CONNECT_BAD_CREDENTIALS";
    case MQTT_CONNECT_UNAUTHORIZED:   return "MQTT_CONNECT_UNAUTHORIZED";
    case MQTT_CONNECTION_TIMEOUT:     return "MQTT_CONNECTION_TIMEOUT";
    case MQTT_CONNECTION_LOST:        return "MQTT_CONNECTION_LOST";
    case MQTT_DISCONNECTED:           return "MQTT_DISCONNECTED";
    default:                          return "UNKNOWN";
  }
}

// Gestión de tiempo
unsigned long lastSensorRead = 0;
unsigned long lastMqttSend = -MQTT_SEND_INTERVAL;
unsigned long lastWifiCheck = 0;
unsigned long lastConfigModeStart = 0;
unsigned int msEntreSolicitudes=75;

unsigned long wifiRetryDelay = 5000;
unsigned long lastWifiRetry = 0;
unsigned long mqttRetryDelay = 5000;
unsigned long lastMqttRetry = 0;
int wifiRetryCount = 0;
int mqttRetryCount = 0;
int lastStationCount=0;
int cuentaMantenimiento=0;
int cuentaErroresTotales=0;
char erroresBufferMant[512];
bool huboErrorMant = false;
SensorReadStatus sensorStatus;
ModbusReadState modbusReadState = READING_DEFROST;
bool seguirLeyendoEnConfig=false;

// ======================== OBJETOS ========================
ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

Ticker wdtTicker;

#define RS485_RO D7
#define RS485_DI D5

SoftwareSerial modbusSerial(RS485_RO, RS485_DI);

ModbusMaster node;

// Declaraciones
void resetWatchdog();
void loadConfigFromEEPROM();
void saveConfigToEEPROM();
void setupWiFi();
uint32_t calculateChecksum(const ConfigData* data) ;
bool connectToWiFi(const char* ssid, const char* password, unsigned long timeout = WIFI_TIMEOUT);
void startNormalMode();
void initializeMqtt();
void startConfigMode();
void setupWebServer();
String getIntroPageHTML();
String getCredentialsPageHTML();
void handleSave();
void handleNormalOperation(unsigned long currentTime);
void handleWifiReconnect();
void resetToFactory();
void handleDixellErrorPage();
void actLicencia(const char* value);
void actTiempoIntervalo(const char* value);
void handleConfigMode();
void preTransmission();
void postTransmission();
static inline String trimSafe(const String& s);
static inline String collapseSpaces(const String& in);
static inline bool isEmailBasic(const String& e);
static inline String keepDigitsPlus(const String& in);
static inline String urlEncode(const String &s);
void handleMaintenance(unsigned long now);
bool canReachMqtt(const char* host, int port, uint16_t timeout_ms, String* errOut);
void handleStatusDixellJson();
extern const char scanPageHeader[] PROGMEM;
void handleScan();
String getDiagnosticPageHTML();
void handleSaveDiagSettings();
void handleSaveStaticIP();
String getFormattedUptime();
void handleSaveWifiClient();
void handleSaveApConfig();
void sendAlertAndGoBack(String message);
String cleanString(const char* str) ;
void handleNetworkConfig();
String cleanString(const char* str);
bool checkModbusConnectionNonBlocking();
bool tryPreferredAPAndSend();
bool readHoldingRegister(uint16_t address, uint16_t &data);
bool readCoil(uint16_t address, uint16_t &data);
void callbackMQTTMant(char* topic, byte* payload, unsigned int length);

void setup() {
  #if DEBUG_MODE
    delay(2000);
  #endif

  Serial.begin(115200);
  mqttClient.setBufferSize(512);
  {
    char verBuf[64];
    snprintf(verBuf, sizeof(verBuf), "\n=== %s v%s ===", modeloFL, VERSION);
    DEBUG_PRINTLN(verBuf);
  }

  wdtTicker.attach(1, resetWatchdog);

  setupWiFi();

  modbusSerial.begin(9600);
  node.begin(1, modbusSerial);

  DixellRawFingerprint fp;
  bool dixellOk = getDixellFingerprintCached(fp);

  if (tryPreferredAPAndSend()) {
    DEBUG_PRINTLN("Flujo APPControlOrionSI completado. No se continúa con startNormalMode ni ConfigMode.");
    return;
  }

  EEPROM.begin(sizeof(EepromHeader) + sizeof(ConfigData) + 16);
  loadConfigFromEEPROM();

  {
    char ssid_full[32];
    snprintf(ssid_full, sizeof(ssid_full), "%s", ssidFL);
    ssid_full[sizeof(ssid_full)-1] = '\0';
    strncpy(ap_ssid, ssid_full, sizeof(ap_ssid) - 1);
    ap_ssid[sizeof(ap_ssid)-1] = '\0';
  }
  DEBUG_PRINTLN(config.stored_ssid);
  DEBUG_PRINTLN(config.stored_password);
  DEBUG_PRINTLN(config.tokenDispositivo);
  DEBUG_PRINTLN("Setup completado");

  if (strlen(config.stored_ssid) > 0) {
    DEBUG_PRINTLN("Credenciales encontradas. Intentando conexión en modo WIFI_STA...");
    startNormalMode();
  } else {
    startConfigMode();
  }
  DEBUG_PRINTLN("Setup completado");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentState == STATE_MAINTENANCE) {
    handleMaintenance(currentTime);
    yield();
    return;
  }

  if (mantenimiento) {
    if (mqttClient.connected()) mqttClient.loop();

    if (mantEsperandoAck) {
      if (millis() > mantDeadline) {
        DEBUG_PRINTLN("⏱️ Timeout esperando ACK de mantenimiento.");
        mantEsperandoAck = false;
      }
      yield();
      return;
    }
    DEBUG_PRINTLN("Fin mantenimiento. Continuando con flujo normal.");
    mantenimiento = false;
  }

  switch (currentState) {
    case STATE_CONFIG_MODE:
      handleConfigMode();
      break;

    case STATE_NORMAL_OPERATION:
      handleNormalOperation(currentTime);
      break;

    case STATE_WIFI_RECONNECT:
      handleWifiReconnect();
      break;

    case STATE_MQTT_RECONNECT:
      currentState = STATE_NORMAL_OPERATION;
      break;

    case STATE_DIXELL_ERROR:
      handleConfigMode();
      break;
  }

  if (mqttClient.connected()) {
    mqttClient.loop();
  }

  server.handleClient();

  yield();
}

