// ======================== GESTIÓN DE MQTT ========================

#include <ArduinoJson.h>

// registrarError
void registrarError(const char* mensaje) {
  huboErrorMant = true;
  strncat(erroresBufferMant, mensaje, sizeof(erroresBufferMant) - strlen(erroresBufferMant) - 1);
  strncat(erroresBufferMant, ";", sizeof(erroresBufferMant) - strlen(erroresBufferMant) - 1);
}

bool readCoil(uint16_t address, uint16_t &data) {
  const int maxRetries = 3;
  uint8_t result;

  for (int attempt = 0; attempt < maxRetries; ++attempt) {
    DEBUG_PRINT("Intentando leer Coil en dirección ");
    DEBUG_PRINT(address);
    DEBUG_PRINT(" (intento ");
    DEBUG_PRINT(attempt + 1);
    DEBUG_PRINTLN(")...");

    result = node.readCoils(address, 1);
    if (result == node.ku8MBSuccess) {
      data = node.getResponseBuffer(0);
      DEBUG_PRINT("Lectura exitosa. Valor: ");
      DEBUG_PRINTLN(data);
      return true;
    } else {
      DEBUG_PRINT("Fallo en intento ");
      DEBUG_PRINTLN(attempt + 1);
    }
    delay(30);
  }

  DEBUG_PRINTLN("Error: No se pudo leer el coil después de ");
  DEBUG_PRINT(maxRetries);
  DEBUG_PRINTLN(" intentos.");
  return false;
}

bool readHoldingRegister(uint16_t address, uint16_t &data) {
  const int maxRetries = 3;
  uint8_t result;

  for (int attempt = 0; attempt < maxRetries; ++attempt) {
    DEBUG_PRINT("Intentando leer Holding Register en dirección ");
    DEBUG_PRINT(address);
    DEBUG_PRINT(" (intento ");
    DEBUG_PRINT(attempt + 1);
    DEBUG_PRINTLN(")...");

    result = node.readHoldingRegisters(address, 1);
    if (result == node.ku8MBSuccess) {
      data = node.getResponseBuffer(0);
      DEBUG_PRINT("Lectura exitosa. Valor: ");
      DEBUG_PRINTLN(data);
      return true;
    } else {
      DEBUG_PRINT("Fallo en intento ");
      DEBUG_PRINT(attempt + 1);
      DEBUG_PRINT(", con el error: ");
      DEBUG_PRINTLN(result);
      delay(30);
    }
  }

  DEBUG_PRINTLN("Error: No se pudo leer el registro después de ");
  DEBUG_PRINT(maxRetries);
  DEBUG_PRINTLN(" intentos.");
  return false;
}

bool readDiscreteInput(uint16_t address, uint16_t &data) {
  const int maxRetries = 3;
  uint8_t result;

  for (int attempt = 0; attempt < maxRetries; ++attempt) {
    DEBUG_PRINT("Intentando leer Discrete Input en dirección ");
    DEBUG_PRINT(address);
    DEBUG_PRINT(" (intento ");
    DEBUG_PRINT(attempt + 1);
    DEBUG_PRINTLN(")...");

    result = node.readDiscreteInputs(address, 1);
    if (result == node.ku8MBSuccess) {
      data = node.getResponseBuffer(0);
      DEBUG_PRINT("Lectura exitosa. Valor: ");
      DEBUG_PRINTLN(data);
      return true;
    } else {
      DEBUG_PRINT("Fallo en intento ");
      DEBUG_PRINTLN(attempt + 1);
    }
    delay(30);
  }

  DEBUG_PRINTLN("Error: No se pudo leer el Discrete Input después de ");
  DEBUG_PRINT(maxRetries);
  DEBUG_PRINTLN(" intentos.");
  return false;
}

bool readInputRegister(uint16_t address, uint16_t &data) {
  const int maxRetries = 3;
  uint8_t result;

  for (int attempt = 0; attempt < maxRetries; ++attempt) {
    DEBUG_PRINT("Intentando leer Input Register en dirección ");
    DEBUG_PRINT(address);
    DEBUG_PRINT(" (intento ");
    DEBUG_PRINT(attempt + 1);
    DEBUG_PRINTLN(")...");

    result = node.readInputRegisters(address, 1);
    if (result == node.ku8MBSuccess) {
      data = node.getResponseBuffer(0);
      DEBUG_PRINT("Lectura exitosa. Valor: ");
      DEBUG_PRINTLN(data);
      return true;
    } else {
      DEBUG_PRINT("Fallo en intento ");
      DEBUG_PRINTLN(attempt + 1);
    }
    delay(30);
  }

  DEBUG_PRINTLN("Error: No se pudo leer el Input Register después de ");
  DEBUG_PRINT(maxRetries);
  DEBUG_PRINTLN(" intentos.");
  return false;
}

bool writeCoil(uint16_t address, uint16_t &data) {
  const int maxRetries = 3;
  uint8_t result;

  for (int attempt = 0; attempt < maxRetries; ++attempt) {
    DEBUG_PRINT("Intentando escribir Coil en dirección ");
    DEBUG_PRINT(address);
    DEBUG_PRINT(" (intento ");
    DEBUG_PRINT(attempt + 1);
    DEBUG_PRINTLN(")...");

    result = node.writeSingleCoil(address, data);
    if (result == node.ku8MBSuccess) {
      DEBUG_PRINT("Escritura exitosa. Valor: ");
      DEBUG_PRINTLN(data);
      return true;
    } else {
      DEBUG_PRINT("Fallo en intento ");
      DEBUG_PRINTLN(attempt + 1);
    }
    delay(30);
  }

  DEBUG_PRINTLN("Error: No se pudo escribir el coil después de ");
  DEBUG_PRINT(maxRetries);
  DEBUG_PRINTLN(" intentos.");
  return false;
}

bool writeSingleReg(uint16_t address, uint16_t &data) {
  const int maxRetries = 3;
  uint8_t result;

  for (int attempt = 0; attempt < maxRetries; ++attempt) {
    DEBUG_PRINT("Intentando escribir Registro en dirección ");
    DEBUG_PRINT(address);
    DEBUG_PRINT(" (intento ");
    DEBUG_PRINT(attempt + 1);
    DEBUG_PRINTLN(")...");

    result = node.writeSingleRegister(address, data);
    if (result == node.ku8MBSuccess) {
      DEBUG_PRINT("Escritura exitosa. Valor: ");
      DEBUG_PRINTLN(data);
      return true;
    } else {
      DEBUG_PRINT("Fallo en intento ");
      DEBUG_PRINTLN(attempt + 1);
    }
    delay(30);
  }

  DEBUG_PRINTLN("Error: No se pudo escribir el registro después de ");
  DEBUG_PRINT(maxRetries);
  DEBUG_PRINTLN(" intentos.");
  return false;
}

void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  char messageBuf[256];
  if (length >= sizeof(messageBuf)) length = sizeof(messageBuf) - 1;
  memcpy(messageBuf, payload, length);
  messageBuf[length] = '\0';

  DEBUG_PRINT("MQTT recibido [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("]: ");
  DEBUG_PRINTLN(messageBuf);

  char expectedTopic[128];
  uint16_t data;
  int intVal = atoi(messageBuf);

  snprintf(expectedTopic, sizeof(expectedTopic), "/FLB/%s/tiempoIntervalo", ssidFL);
  if (strcmp(topic, expectedTopic) == 0) {
    long v = atol(messageBuf);
    if (v < 1000) v = v * 1000;
    char buf[16]; snprintf(buf, sizeof(buf), "%ld", v);
    actTiempoIntervalo(buf);
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s/tiempoIntervalo_ack", ssidFL);
    mqttClient.publish(topicBuffer, buf);
    return;
  }

  snprintf(expectedTopic, sizeof(expectedTopic), "/FLB/%s/w_setpoint", ssidFL);
  if (strcmp(topic, expectedTopic) == 0) {
    DEBUG_PRINTLN("Intentando escribir el registro w_setpoint...");
    uint16_t value = (uint16_t)intVal;
    writeModbus(config.func_w_SetPoint, config.addr_w_SetPoint, value);
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s/w_setpoint_ack", ssidFL);
    char ackBuf[16]; snprintf(ackBuf, sizeof(ackBuf), "%u", value);
    mqttClient.publish(topicBuffer, ackBuf);
    delay(300);
    DEBUG_PRINTLN("Intentando leer el registro SetPoint...");
    if (readModbus(config.func_SetPoint, config.addr_SetPoint, data)) {
      DEBUG_PRINT("Registro SetPoint: ");
      DEBUG_PRINTLN(data);
      snprintf(jsonBuffer, sizeof(jsonBuffer),
        "{\"t\":\"%s\",\"d\":{"
        "\"%s\":%u"
        "},\"v\":\"%s\"}",
        ssidFL,
        "SetPoint", data,
        VERSION
      );
      snprintf(topicBuffer, sizeof(topicBuffer), "/%s", codModelo);
      bool resultMQTT = mqttClient.publish(topicBuffer, jsonBuffer);

      if (resultMQTT) {
        DEBUG_PRINTLN("Datos MQTT enviados ad hoc");
        DEBUG_PRINTLN(jsonBuffer);
      } else {
        DEBUG_PRINTLN("Error enviando datos MQTT");
        handleError(ERROR_MQTT_FAILED);
      }
    } else {
      DEBUG_PRINTLN("Error de comunicación al leer SetPoint");
    }
    return;
  }

  snprintf(expectedTopic, sizeof(expectedTopic), "/FLB/%s/w_status", ssidFL);
  if (strcmp(topic, expectedTopic) == 0) {
    DEBUG_PRINTLN("Intentando escribir el registro w_status...");
    uint16_t value = (uint16_t)intVal;
    writeModbus(config.func_w_Status, config.addr_w_Status, value);
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s/w_status_ack", ssidFL);
    char ackBuf[16]; snprintf(ackBuf, sizeof(ackBuf), "%u", value);
    mqttClient.publish(topicBuffer, ackBuf);
    delay(100);
    DEBUG_PRINTLN("Intentando leer el registro StatusOn...");
    if (readModbus(config.func_StatusOn, config.addr_StatusOn, data)) {
      DEBUG_PRINT("Registro Status: ");
      DEBUG_PRINTLN(data);
      snprintf(jsonBuffer, sizeof(jsonBuffer),
        "{\"t\":\"%s\",\"d\":{"
        "\"%s\":%u"
        "},\"v\":\"%s\"}",
        ssidFL,
        "StatusOn", data,
        VERSION
      );
      snprintf(topicBuffer, sizeof(topicBuffer), "/%s", codModelo);
      bool resultMQTT = mqttClient.publish(topicBuffer, jsonBuffer);

      if (resultMQTT) {
        DEBUG_PRINTLN("Datos MQTT enviados ad hoc");
        DEBUG_PRINTLN(jsonBuffer);
      } else {
        DEBUG_PRINTLN("Error enviando datos MQTT");
        handleError(ERROR_MQTT_FAILED);
      }
    } else {
      DEBUG_PRINTLN("Error de comunicación al leer StatusOn");
    }
    return;
  }

  snprintf(expectedTopic, sizeof(expectedTopic), "/FLB/%s/w_LicActiva", ssidFL);
  if (strcmp(topic, expectedTopic) == 0) {
    DEBUG_PRINTLN("Licencia Activa recibida:");
    strncpy(config.licenciaActual, messageBuf, sizeof(config.licenciaActual)-1);
    config.licenciaActual[sizeof(config.licenciaActual)-1] = '\0';
    saveConfigToEEPROM();
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s/w_LicActiva_ack", ssidFL);
    mqttClient.publish(topicBuffer, config.licenciaActual);
    return;
  }

  snprintf(expectedTopic, sizeof(expectedTopic), "/FLB/%s/w_rstFabrica", ssidFL);
  if (strcmp(topic, expectedTopic) == 0) {
    DEBUG_PRINTLN("Reseteando a fabrica:");
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s/w_rstFabrica_ack", ssidFL);
    mqttClient.publish(topicBuffer, "OK");
    resetToFactory();
    return;
  }

  snprintf(expectedTopic, sizeof(expectedTopic), "/FLB/%s/w_rstDispositivo", ssidFL);
  if (strcmp(topic, expectedTopic) == 0) {
    DEBUG_PRINTLN("Reseteando dispositivo:");
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s/w_rstDispositivo_ack", ssidFL);
    mqttClient.publish(topicBuffer, "OK");
    resetDispositivo();
    return;
  }

  snprintf(expectedTopic, sizeof(expectedTopic), "/FLB/%s/", ssidFL);
  if (strncmp(topic, expectedTopic, strlen(expectedTopic)) == 0) {
    const char* suffix = topic + strlen(expectedTopic);
    if (strcmp(suffix, "addr_Probe1") == 0) {
      uint16_t newAddress = (uint16_t)atoi(messageBuf);
      if (newAddress > 0 && newAddress < 65536) {
        config.addr_Probe1 = newAddress; saveConfigToEEPROM();
        DEBUG_PRINT("Nueva dirección para Probe1: "); DEBUG_PRINTLN(newAddress);
        snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s/addr_Probe1_ack", ssidFL);
        char ack[16]; snprintf(ack, sizeof(ack), "%u", newAddress);
        mqttClient.publish(topicBuffer, ack);
      }
      return;
    }
    if (strncmp(suffix, "func_", 5) == 0) {
      if (strcmp(suffix, "func_Probe1") == 0) {
        ModbusFunc newFunc = static_cast<ModbusFunc>(atoi(messageBuf));
        if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
          config.func_Probe1 = newFunc; saveConfigToEEPROM();
          DEBUG_PRINTLN("Actualizada func_Probe1");
          snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s/func_Probe1_ack", ssidFL);
          char ack[8]; snprintf(ack, sizeof(ack), "%d", (int)newFunc);
          mqttClient.publish(topicBuffer, ack);
        }
      }
      return;
    }
    if (strcmp(suffix, "passDiag") == 0) {
      if (length < (int)sizeof(config.passDiag)) {
        strncpy(config.passDiag, messageBuf, sizeof(config.passDiag)-1);
        config.passDiag[sizeof(config.passDiag)-1] = '\0';
        saveConfigToEEPROM();
        DEBUG_PRINTLN("Nueva contraseña del AP de diagnóstico guardada. Reiniciando para aplicar...");
        snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/passDiag_ack", ssidFL);
        mqttClient.publish(topicBuffer, "OK");
      }
      return;
    }
    if (strcmp(suffix, "sendDiag") == 0) {
      String diagnosticJson = getDiagnosticDataAsJson();
      if (diagnosticJson.length() > 0) {
        WiFiClient client;
        client.setTimeout(30000);
        HTTPClient https;
        https.setTimeout(30000);
        String urlDiag = String(urlBase) + "login/FLBDiag";
        String postData = "{\"ssidFL\":\"" + String(ssidFL) + "\",\"diagnosticData\":" + diagnosticJson + "}";
        DEBUG_PRINT("URL de diagnóstico: "); DEBUG_PRINTLN(urlDiag);
        DEBUG_PRINT("JSON a enviar: "); DEBUG_PRINTLN(postData);
        if (https.begin(client, urlDiag)) {
          https.addHeader("Content-Type", "application/json");
          https.addHeader("User-Agent", "FrioLink/1.0");
          DEBUG_PRINTLN("Enviando POST HTTP...");
          int httpCode = https.POST(postData);
          if (httpCode > 0) {
            DEBUG_PRINT("✅ Código HTTP: "); DEBUG_PRINTLN(String(httpCode));
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
              String response = https.getString();
              DEBUG_PRINTLN("✅ Diagnóstico enviado correctamente");
              DEBUG_PRINTLN("Respuesta del servidor: " + response);
            } else {
              DEBUG_PRINTLN("⚠️  Respuesta HTTP no esperada: " + String(httpCode));
              String response = https.getString();
              DEBUG_PRINTLN("Contenido de respuesta: " + response);
            }
          } else {
            DEBUG_PRINTLN("❌ Error en POST HTTP: " + https.errorToString(httpCode));
            DEBUG_PRINTLN("Código de error: " + String(httpCode));
          }
          https.end();
        } else {
          DEBUG_PRINTLN("❌ No se pudo conectar al servidor HTTP");
        }
      } else {
        DEBUG_PRINTLN("❌ JSON de diagnóstico inválido o vacío");
      }
      return;
    }
  }
}

void initializeMqtt() {
  mqttClient.setServer(config.mqtt_server, config.mqtt_port);
  mqttClient.setCallback(callbackMQTT);
  DEBUG_PRINTLN("Cliente MQTT inicializado");
}

bool connectToMqtt() {
  mqttClient.setBufferSize(1024);
  if (mqttClient.connected()) return true;

  unsigned long currentTime = millis();
  if (currentTime - lastMqttRetry < mqttRetryDelay) {
    return false;
  }

  DEBUG_PRINT("Conectando a MQTT...");
  if (mqttClient.connect(ssidFL)) {
    DEBUG_PRINTLN(" ¡Conectado!");

    const char* topics[] = {
      "w_setpoint","w_status","w_LicActiva","tokenDispositivo","mqtt_server","mqtt_port",
      "licenciaActual","tiempoIntervalo","w_rstFabrica","addr_Probe1","addr_SetPoint",
      "addr_StatusOn","addr_Defrost","addr_PtaAbierta","addr_ErrorPb1","addr_ErrorPb2",
      "addr_AltoValorPb1","addr_BajoValorPb1","addr_Compressor","addr_w_setpoint","addr_w_status",
      "passDiag","func_Probe1","func_SetPoint","func_StatusOn","func_Defrost","func_PtaAbierta",
      "func_ErrorPb1","func_ErrorPb2","func_AltoValorPb1","func_BajoValorPb1","func_Compressor",
      "func_w_setpoint","func_w_status","sendDiag"
    };
    for (size_t i=0;i<sizeof(topics)/sizeof(topics[0]);++i){
      snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/%s", ssidFL, topics[i]);
      mqttClient.subscribe(topicBuffer);
    }
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/FLB/%s", ssidFL);
    mqttClient.subscribe(topicBuffer);

    mqttRetryCount = 0;
    mqttRetryDelay = 5000;

    snprintf(jsonBuffer, sizeof(jsonBuffer),
      "{\"t\":\"%s\",\"d\":{"
      "\"%s\":\"%s\""
      "},\"v\":\"%s\"}",
      ssidFL,
      "Estado", "Reconectando MQTT",
      VERSION
    );

    DEBUG_PRINTLN("Informando reconexion");
    DEBUG_PRINTLN(jsonBuffer);

    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/%s", codModelo);
    bool resultMQTT = mqttClient.publish(topicBuffer, jsonBuffer);

    if (resultMQTT) {
      DEBUG_PRINTLN("Datos MQTT de mantenimiento enviados");
    } else {
      DEBUG_PRINTLN("No se pueden enviar los datos de reconexión al MQTT");
    }
    return true;
  } else {
    DEBUG_PRINT(" Fallo. Estado: ");
    DEBUG_PRINTLN(mqttClient.state());

    lastMqttRetry = currentTime;
    mqttRetryCount++;
    mqttRetryDelay = min(mqttRetryDelay * 2, 30000UL);
    cuentaErroresTotales = cuentaErroresTotales + 1;
    if (cuentaErroresTotales > 10) {
      resetDispositivo();
    }
    DEBUG_PRINT("Cuenta error total: ");
    DEBUG_PRINTLN(cuentaErroresTotales);
    return false;
  }
}

void resetSensorReadCycle() {
  modbusReadState = READING_DEFROST;
  sensorStatus.cycleComplete = false;
  sensorStatus.ok_Probe1 = false;
  sensorStatus.ok_SetPoint = false;
  sensorStatus.ok_StatusOn = false;
  sensorStatus.ok_Defrost = false;
  sensorStatus.ok_PtaAbierta = false;
  sensorStatus.ok_ErrorPb1 = false;
  sensorStatus.ok_ErrorPb2 = false;
  sensorStatus.ok_AltoValorPb1 = false;
  sensorStatus.ok_BajoValorPb1 = false;
  sensorStatus.ok_Compressor = false;
  erroresBufferMant[0] = '\0';
  huboErrorMant = false;
}

bool readModbus(ModbusFunc func, uint16_t address, uint16_t &data) {
  switch (func) {
    case FC01_COIL:
      return readCoil(address, data);
    case FC02_DI:
      return readDiscreteInput(address, data);
    case FC03_HOLD:
      return readHoldingRegister(address, data);
    case FC04_INPUT:
      return readInputRegister(address, data);
    default:
      return false;
  }
}

bool writeModbus(ModbusFunc func, uint16_t address, uint16_t &data) {
  switch (func) {
    case FC05_WSC:
      return writeCoil(address, data);
    case FC06_WSR:
      return writeSingleReg(address, data);
    default:
      return false;
  }
}

void readSensorDataNonBlocking() {
  static unsigned long lastReadTime = 0;
  uint16_t data;

  if (modbusReadState == ALL_READINGS_DONE) {
    return;
  }

  if (millis() - lastReadTime < msEntreSolicitudes) {
    return;
  }

  switch (modbusReadState) {
    case READING_DEFROST:
      if (readModbus(config.func_Defrost, config.addr_Defrost, data)) {
        snprintf(config.Defrost, sizeof(config.Defrost), "%u", data);
        sensorStatus.ok_Defrost = true;
      } else {
        registrarError("Defrost");
        snprintf(config.Defrost, sizeof(config.Defrost), "Error");
      }
      modbusReadState = READING_PTAABIERTA;
      break;

    case READING_PTAABIERTA:
      if (readModbus(config.func_PtaAbierta, config.addr_PtaAbierta, data)) {
        snprintf(config.PtaAbierta, sizeof(config.PtaAbierta), "%u", data);
        sensorStatus.ok_PtaAbierta = true;
      } else {
        registrarError("PtaAbierta");
        snprintf(config.PtaAbierta, sizeof(config.PtaAbierta), "Error");
      }
      modbusReadState = READING_ERRORPB1;
      break;

    case READING_ERRORPB1:
      if (readModbus(config.func_ErrorPb1, config.addr_ErrorPb1, data)) {
        snprintf(config.ErrorPb1, sizeof(config.ErrorPb1), "%u", data);
        sensorStatus.ok_ErrorPb1 = true;
      } else {
        registrarError("ErrorPb1");
        snprintf(config.ErrorPb1, sizeof(config.ErrorPb1), "Error");
      }
      modbusReadState = READING_ERRORPB2;
      break;

    case READING_ERRORPB2:
      if (readModbus(config.func_ErrorPb2, config.addr_ErrorPb2, data)) {
        snprintf(config.ErrorPb2, sizeof(config.ErrorPb2), "%u", data);
        sensorStatus.ok_ErrorPb2 = true;
      } else {
        registrarError("ErrorPb2");
        snprintf(config.ErrorPb2, sizeof(config.ErrorPb2), "Error");
      }
      modbusReadState = READING_ALTOVALORPB1;
      break;

    case READING_ALTOVALORPB1:
      if (readModbus(config.func_AltoValorPb1, config.addr_AltoValorPb1, data)) {
        snprintf(config.AltoValorPb1, sizeof(config.AltoValorPb1), "%u", data);
        sensorStatus.ok_AltoValorPb1 = true;
      } else {
        registrarError("AltoValorPb1");
        snprintf(config.AltoValorPb1, sizeof(config.AltoValorPb1), "Error");
      }
      modbusReadState = READING_BAJOVALORPB1;
      break;

    case READING_BAJOVALORPB1:
      if (readModbus(config.func_BajoValorPb1, config.addr_BajoValorPb1, data)) {
        snprintf(config.BajoValorPb1, sizeof(config.BajoValorPb1), "%u", data);
        sensorStatus.ok_BajoValorPb1 = true;
      } else {
        registrarError("BajoValorPb1");
        snprintf(config.BajoValorPb1, sizeof(config.BajoValorPb1), "Error");
      }
      modbusReadState = READING_COMPRESSOR;
      break;

    case READING_COMPRESSOR:
      if (readModbus(config.func_Compressor, config.addr_Compressor, data)) {
        snprintf(config.Compressor, sizeof(config.Compressor), "%u", data);
        sensorStatus.ok_Compressor = true;
      } else {
        registrarError("Compressor");
        snprintf(config.Compressor, sizeof(config.Compressor), "Error");
      }
      modbusReadState = READING_PROBE1;
      break;

    case READING_PROBE1:
      if (readModbus(config.func_Probe1, config.addr_Probe1, data)) {
        snprintf(config.Probe1, sizeof(config.Probe1), "%u", data);
        sensorStatus.ok_Probe1 = true;
      } else {
        registrarError("Probe1");
        snprintf(config.Probe1, sizeof(config.Probe1), "Error");
      }
      modbusReadState = READING_SETPOINT;
      break;

    case READING_SETPOINT:
      if (readModbus(config.func_SetPoint, config.addr_SetPoint, data)) {
        snprintf(config.SetPoint, sizeof(config.SetPoint), "%u", data);
        sensorStatus.ok_SetPoint = true;
      } else {
        registrarError("SetPoint");
        snprintf(config.SetPoint, sizeof(config.SetPoint), "Error");
      }
      modbusReadState = READING_STATUSON;
      break;

    case READING_STATUSON:
      if (readModbus(config.func_StatusOn, config.addr_StatusOn, data)) {
        snprintf(config.StatusOn, sizeof(config.StatusOn), "%u", data);
        sensorStatus.ok_StatusOn = true;
      } else {
        registrarError("StatusOn");
        snprintf(config.StatusOn, sizeof(config.StatusOn), "Error");
      }
      modbusReadState = ALL_READINGS_DONE;
      sensorStatus.cycleComplete = true;
      break;

    default:
      modbusReadState = VER_IDLE;
      break;
  }

  lastReadTime = millis();
}

void sendMqttData() {
  if (strcmp(config.licenciaActual, "1") != 0) {
    DEBUG_PRINT("No pude transmitir por tipo de licencia:");
    DEBUG_PRINTLN(config.licenciaActual);
    resetSensorReadCycle();
    return;
  }

  char out[512];
  snprintf(out, sizeof(out), "{\"t\":\"%s\",\"d\":{", ssidFL);
  bool first = true;

  #define ADD_FIELD(key, val) \
    do { \
      if (!first) strncat(out, ",", sizeof(out) - strlen(out) - 1); \
      char tmp[128]; snprintf(tmp, sizeof(tmp), "\"%s\":\"%s\"", key, val); \
      strncat(out, tmp, sizeof(out) - strlen(out) - 1); \
      first = false; \
    } while(0)

  if (sensorStatus.ok_Defrost) ADD_FIELD("Defrost", config.Defrost);
  if (sensorStatus.ok_PtaAbierta) ADD_FIELD("PtaAbierta", config.PtaAbierta);
  if (sensorStatus.ok_ErrorPb1) ADD_FIELD("ErrorPb1", config.ErrorPb1);
  if (sensorStatus.ok_ErrorPb2) ADD_FIELD("ErrorPb2", config.ErrorPb2);
  if (sensorStatus.ok_AltoValorPb1) ADD_FIELD("AltoValorPb1", config.AltoValorPb1);
  if (sensorStatus.ok_BajoValorPb1) ADD_FIELD("BajoValorPb1", config.BajoValorPb1);
  if (sensorStatus.ok_Compressor) ADD_FIELD("Compressor", config.Compressor);
  if (sensorStatus.ok_Probe1) ADD_FIELD("Probe1", config.Probe1);
  if (sensorStatus.ok_SetPoint) ADD_FIELD("SetPoint", config.SetPoint);
  if (sensorStatus.ok_StatusOn) ADD_FIELD("StatusOn", config.StatusOn);

  if (!first) strncat(out, "},", sizeof(out) - strlen(out) - 1);
  else strncat(out, "},", sizeof(out) - strlen(out) - 1);

  char verp[64];
  snprintf(verp, sizeof(verp), "\"v\":\"%s\",\"hv\":\"%s\"}", VERSION, HVERSION);
  strncat(out, verp, sizeof(out) - strlen(out) - 1);

  DEBUG_PRINTLN(out);

  connectToMqtt();

  snprintf(topicBuffer, sizeof(topicBuffer), "/%s", codModelo);
  bool resultMQTT = mqttClient.publish(topicBuffer, out);

  if (!resultMQTT) {
    handleError(ERROR_MQTT_FAILED);
  }

  if (huboErrorMant) {
    char errJson[512];
    snprintf(errJson, sizeof(errJson), "{\"t\":\"%s\",\"errores\":\"%s\",\"v\":\"%s\"}", ssidFL, erroresBufferMant, VERSION);
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/FLB/ErrorLectura/%s", ssidFL);
    mqttClient.publish(topicBuffer, errJson);
  }

  resetSensorReadCycle();
}

