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
const char* ssidFL="FLB1146";
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

//Direcciones por defecto Dixell XR75CH
//#define addr_Probe1_default 264
//#define addr_SetPoint_default 886
//#define addr_StatusOn_default 512
//#define addr_Defrost_default 513
//#define addr_PtaAbierta_default 519
//#define addr_ErrorPb1_default 520
//#define addr_ErrorPb2_default 521
//#define addr_AltoValorPb1_default 524
//#define addr_BajoValorPb1_default 525
//#define addr_Compressor_default 542
//#define addr_w_SetPoint_default 886
//#define addr_w_Status_default 512

//Direcciones por defecto Danfoss AK-CC55
//#define addr_Probe1_default 2530
//#define addr_SetPoint_default 2612
//#define addr_StatusOn_default 116
//#define addr_Defrost_default 2511
//#define addr_PtaAbierta_default 20009
//#define addr_ErrorPb1_default 20002
//#define addr_ErrorPb2_default 20003
//#define addr_AltoValorPb1_default 20007
//#define addr_BajoValorPb1_default 20008
//#define addr_Compressor_default 2684
//#define addr_w_SetPoint_default 2612
//#define addr_w_Status_default 116

//Funciones de lectura Dixell XR06CH
#define func_Probe1_default       FC03_HOLD
#define func_SetPoint_default     FC03_HOLD
#define func_StatusOn_default     FC01_COIL
#define func_Defrost_default      FC01_COIL
#define func_PtaAbierta_default   FC01_COIL
#define func_ErrorPb1_default     FC01_COIL
#define func_ErrorPb2_default     FC01_COIL
#define func_AltoValorPb1_default FC01_COIL
#define func_BajoValorPb1_default FC01_COIL
#define func_Compressor_default   FC01_COIL
//Funciones de escritura Dixell XR06CH
#define func_w_SetPoint_default   FC06_WSR
#define func_w_Status_default     FC05_WSC

//Funciones de lectura Dixell XR75CH
//#define func_Probe1_default       FC03_HOLD
//#define func_SetPoint_default     FC03_HOLD
//#define func_StatusOn_default     FC01_COIL
//#define func_Defrost_default      FC01_COIL
//#define func_PtaAbierta_default   FC01_COIL
//#define func_ErrorPb1_default     FC01_COIL
//#define func_ErrorPb2_default     FC01_COIL
//#define func_AltoValorPb1_default FC01_COIL
//#define func_BajoValorPb1_default FC01_COIL
//#define func_Compressor_default   FC01_COIL
////Funciones de escritura Dixell XR06CH
//#define func_w_SetPoint_default   FC06_WSR
//#define func_w_Status_default     FC05_WSC

//Funciones de lectura Danfoss AK-CC55
//#define func_Probe1_default       FC03_HOLD
//#define func_SetPoint_default     FC03_HOLD
//#define func_StatusOn_default     FC03_HOLD
//#define func_Defrost_default      FC03_HOLD
//#define func_PtaAbierta_default   FC03_HOLD
//#define func_ErrorPb1_default     FC02_DI
//#define func_ErrorPb2_default     FC02_DI
//#define func_AltoValorPb1_default FC03_HOLD
//#define func_BajoValorPb1_default FC03_HOLD
//#define func_Compressor_default   FC03_HOLD
//Funciones de escritura Danfoss AK-CC55
//#define func_w_SetPoint_default   FC06_WSR
//#define func_w_Status_default     FC05_WSC

// Timeouts y intervalos (en milisegundos)
const unsigned long WIFI_TIMEOUT = 10000; //10s
const unsigned long WIFI_CHECK_INTERVAL = 30000; //30s
unsigned long MQTT_SEND_INTERVAL = 10000; //60s
const unsigned long SENSOR_READ_INTERVAL = 5000; //5s
unsigned long CONFIG_MODE_TIMEOUT = 120000; //2min

/*Para mantenimiento*/
const char* PREFERRED_SSID     = "APPControlOrionSI";
const char* PREFERRED_PASSWORD = "12345678";
bool mantenimiento = false;          // ya la tenés
bool mantEsperandoAck = false;
unsigned long mantDeadline = 0;
const unsigned long MANT_TIMEOUT_MS = 5000; // 10 s para esperar respuesta
const unsigned long MANT_REPUBLISH_MS = 5000;  // reintentar ping cada 30s
unsigned long mantNextPublish = 0;
String conexDixell = "SINREVISAR";  // Inicial
String señalesOK = "";             // Señales leídas con éxito
String señalesError = "";          // Señales que fallaron

// ======================== PARA DETECTAR FAMILIA Y VERSIONES ===============================================
enum ModbusPingResult { 
  MB_PING_OK = 0, 
  MB_PING_SLAVE_ERROR, // incluye BUSY 
  MB_PING_TIMEOUT, 
  MB_PING_ERROR 
};

struct DixellRawFingerprint {
  uint8_t  familyCode;        // 0xFF si no existe
  uint8_t  firmware;          // 0xFF si no existe

  bool     reg0Readable;
  bool     setpointReadable;
  bool     inputRegReadable;

  uint8_t  lastException;     // código Modbus crudo
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

//Para verificar la conexion con el dispositivo (lo hacemos solo en el momento de configuracion, ver si hace falta hacerlo en algun otro momento)
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

// Variables globales para la verificación
VerificationState verificationState = VER_IDLE;
unsigned long lastVerificationTime = 0;
int verificationSuccessCount = 0;
int verificationTotalCount = 0;
String verificationResults = "";
bool verificationInProgress = false;

// Estructura para resultados de verificación
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

// Estado del sistema
SystemState currentState = STATE_NORMAL_OPERATION;
ErrorCode lastError = ERROR_NONE;
bool config_mode = false;

// Buffers reutilizables (reducción de memoria)
char jsonBuffer[256];
char topicBuffer[128];
char ap_ssid[32];
char ap_diag_ssid[32]; // Buffer para el SSID completo

// Tipos de función Modbus que puede usar cada dirección
enum ModbusFunc : uint8_t {
  FC01_COIL = 1,    // Read Coils
  FC02_DI   = 2,    // Read Discrete Inputs
  FC03_HOLD = 3,    // Read Holding Registers
  FC04_INPUT = 4,    // Read Input Registers
  FC05_WSC = 5,   // Write Single Coils
  FC06_WSR = 6    // Write Single Registers
};

// Estructura para datos de configuración
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
  // Tipo de función Modbus asociada a cada dirección
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
  ALL_READINGS_DONE // Estado final
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
  bool cycleComplete = false; // Indica si el ciclo de lectura ha terminado
};

// Configuración
ConfigData config = {
  "",  // Probe1
  "",  // SetPoint
  "",  // StatusOn
  "",  // Defrost
  "",  // PtaAbierta
  "",  // ErrorPb1
  "",  // ErrorPb2
  "",  // AltoValorPb1
  "",  // BajoValorPb1
  "",  // Compressor
  ssidDefecto,  //stored_ssid
  passWIFI,  //stored_password
  tokenDefecto,  //tokenDispositivo
  mqtt_server_default,  // mqtt_server
  mqtt_port_default,  // mqtt_port
  licActual_default,  // licenciaActual
  tiempoIntervalo_default,  // tiempoIntervalo --5m
  addr_Probe1_default,
  addr_SetPoint_default,
  addr_StatusOn_default,
  addr_Defrost_default,
  addr_PtaAbierta_default,
  addr_ErrorPb1_default,
  addr_ErrorPb2_default,
  addr_AltoValorPb1_default,
  addr_BajoValorPb1_default,
  addr_Compressor_default,
  addr_w_SetPoint_default,
  addr_w_Status_default,
  passDiag_default, // passDiag
  useStaticIP_default,     // useStaticIP (por defecto, usar DHCP)
  staticIP_default, // staticIP
  staticGateway_default, // staticGateway
  staticSubnet_default, // staticSubnet
  staticDNS_default, // staticDNS
  func_Probe1_default, 
  func_SetPoint_default,
  func_StatusOn_default, 
  func_Defrost_default,
  func_PtaAbierta_default, 
  func_ErrorPb1_default,
  func_ErrorPb2_default, 
  func_AltoValorPb1_default,
  func_BajoValorPb1_default, 
  func_Compressor_default,
  func_w_SetPoint_default, 
  func_w_Status_default,
  0  // checksum
};

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

// Gestión de reconexión con backoff exponencial
unsigned long wifiRetryDelay = 5000;
unsigned long lastWifiRetry = 0;
unsigned long mqttRetryDelay = 5000;
unsigned long lastMqttRetry = 0;
int wifiRetryCount = 0;
int mqttRetryCount = 0;
int lastStationCount=0;
int cuentaMantenimiento=0;
int cuentaErroresTotales=0;
char erroresBufferMant[512]; // Puede ser más grande si hay muchos errores
bool huboErrorMant = false;
SensorReadStatus sensorStatus;
ModbusReadState modbusReadState = READING_DEFROST;
bool seguirLeyendoEnConfig=false;

// ======================== OBJETOS ========================
// Conectividad
ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Ticker para watchdog
Ticker wdtTicker;

// Definir pines para SoftwareSerial
#define RS485_RO D7   // RX desde el módulo
#define RS485_DI D5   // TX hacia el módulo

// Crear objeto SoftwareSerial
SoftwareSerial modbusSerial(RS485_RO, RS485_DI);

// Crear instancia del maestro Modbus
ModbusMaster node;

// ======================== DECLARACION DE FUNCIONES QUE SE DESARROLLAR MAS ADELANTE ========================
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
  //resetToFactory();
  //return;
  #if DEBUG_MODE
    delay(2000);
  #endif
  
  Serial.begin(115200);
  mqttClient.setBufferSize(512);
  DEBUG_PRINTLN("\n=== "+String(modeloFL)+" v" + String(VERSION) + " ===");
  
  // Inicializar watchdog
  wdtTicker.attach(1, resetWatchdog);
  
  setupWiFi();

  // Inicializar el nodo Modbus
  modbusSerial.begin(9600);
  node.begin(1, modbusSerial); // Dirección del esclavo Modbus (Dixel)

  /*Leemos la familia y modelo del dispositivo*/
  DixellRawFingerprint fp;
  bool dixellOk = getDixellFingerprintCached(fp);
  
  if (tryPreferredAPAndSend()) {
    DEBUG_PRINTLN("Flujo APPControlOrionSI completado. No se continúa con startNormalMode ni ConfigMode.");
    return;   // 🚨 corta acá: no sigue con la lógica normal
  }
  
  if (tryPreferredAPAndSend()) {
    DEBUG_PRINTLN("Flujo APPControlOrionSI completado. No se continúa con startNormalMode ni ConfigMode.");
    return;   // 🚨 corta acá: no sigue con la lógica normal
  }

  // Cargar configuración
  EEPROM.begin(sizeof(ConfigData) + 100); // Margen extra
  loadConfigFromEEPROM();
  
  // Generar SSID único
  String randomDigits = String(random(10000, 99999));
  String ssid_full = String(ssidFL);// + randomDigits;
  ssid_full.toCharArray(ap_ssid, sizeof(ap_ssid));
  DEBUG_PRINTLN(config.stored_ssid);
  DEBUG_PRINTLN(config.stored_password);
  DEBUG_PRINTLN(config.tokenDispositivo);
  DEBUG_PRINTLN("Setup completado");
  
  // Intentar conexión WiFi si hay credenciales
  if (strlen(config.stored_ssid) > 0) {
    DEBUG_PRINTLN("Credenciales encontradas. Intentando conexión en modo WIFI_STA...");
    startNormalMode();
  } else {
    startConfigMode();
  }
  DEBUG_PRINTLN("Setup completado");
}

void loop() {
  //clearEEPROM();
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();

  if (currentState == STATE_MAINTENANCE) {
    handleMaintenance(currentTime);
    yield();
    return;   // 🚨 nunca ejecuta el flujo normal mientras exista mantenimiento
  }
  
  if (mantenimiento) {
    // Mantener MQTT vivo y escuchar
    if (mqttClient.connected()) mqttClient.loop();

    if (mantEsperandoAck) {
      if (millis() > mantDeadline) {
        DEBUG_PRINTLN("⏱️ Timeout esperando ACK de mantenimiento.");
        mantEsperandoAck = false;
        // decidir qué hacer: reintentar, reiniciar, etc.
      }
      // Importante: NO return aquí. Dejá que loop siga llamándose
      // para que mqttClient.loop() procese callbacks.
      yield();
      return; // si DE VERDAD no querés que corra nada más del loop normal
    }
  // Si llegó el ACK o terminó el timeout, podés decidir:
    //  - Quedarte en mantenimiento
    //  - O bien terminar mantenimiento y seguir flujo normal
    // Ejemplo: terminar mantenimiento y reiniciar flujo normal:
    DEBUG_PRINTLN("Fin mantenimiento. Continuando con flujo normal.");
    mantenimiento = false;
    // opcional: mqttClient.disconnect(); WiFi.disconnect(true);
    // y dejar que el estado normal siga abajo…
  }
  
  // Máquina de estados principal
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
      // Manejado en handleNormalOperation
      currentState = STATE_NORMAL_OPERATION;
      break;

    case STATE_DIXELL_ERROR:
      handleConfigMode();
      break;
  }
  
  // Mantener conexión MQTT activa (no bloqueante)
  if (mqttClient.connected()) {
    mqttClient.loop();
  }

  // Mantener el servidor web activo en cualquier modo
  server.handleClient();
  
  // Yield para el sistema
  yield();  
}
