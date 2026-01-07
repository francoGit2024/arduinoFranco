// ======================== GESTIÓN DE MQTT ========================

//void preTransmission() {
//  digitalWrite(MAX485_DE_RE, HIGH);
//  delayMicroseconds(50);
//}
//
//void postTransmission() {
//  delayMicroseconds(50);
//  digitalWrite(MAX485_DE_RE, LOW);
//  delayMicroseconds(50);
//}

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
  }

  DEBUG_PRINTLN("Error: No se pudo leer el coil después de ");
  DEBUG_PRINT(maxRetries);
  DEBUG_PRINTLN(" intentos.");
  return false;
}

bool readHoldingRegister(uint16_t address, uint16_t &data) {
  // Reducimos el número de reintentos. Si falla 3 veces seguidas, es probable que el problema persista.
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
      // NO HAY DELAY. Reintentamos inmediatamente.
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

    result = node.readDiscreteInputs(address, 1);  // FC02
    if (result == node.ku8MBSuccess) {
      data = node.getResponseBuffer(0);
      DEBUG_PRINT("Lectura exitosa. Valor: ");
      DEBUG_PRINTLN(data);
      return true;
    } else {
      DEBUG_PRINT("Fallo en intento ");
      DEBUG_PRINTLN(attempt + 1);
    }
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

    result = node.readInputRegisters(address, 1);  // FC04
    if (result == node.ku8MBSuccess) {
      data = node.getResponseBuffer(0);
      DEBUG_PRINT("Lectura exitosa. Valor: ");
      DEBUG_PRINTLN(data);
      return true;
    } else {
      DEBUG_PRINT("Fallo en intento ");
      DEBUG_PRINTLN(attempt + 1);
    }
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
  }

  DEBUG_PRINTLN("Error: No se pudo leer el coil después de ");
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
  }

  DEBUG_PRINTLN("Error: No se pudo leer el coil después de ");
  DEBUG_PRINT(maxRetries);
  DEBUG_PRINTLN(" intentos.");
  return false;
}

void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  DEBUG_PRINT("MQTT recibido [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("]: ");
  DEBUG_PRINTLN(message);
  
  // Procesar comandos de control
  String topicStr = String(topic);
  int value = message.toInt();
  uint8_t result;
  uint16_t data;

  // Cambiar intervalo de transmisión por MQTT (admite s o ms)
  if (String(topic) == "/FLB/" + String(ssidFL) + "/tiempoIntervalo") {
    long v = message.toInt();
    if (v < 1000) v = v * 1000;        // si te mandan "30" => 30 s
    char buf[16]; snprintf(buf, sizeof(buf), "%ld", v);
  
    actTiempoIntervalo(buf);           // guarda en EEPROM y actualiza MQTT_SEND_INTERVAL
  
    // (opcional) confirmar por MQTT
    mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/tiempoIntervalo_ack").c_str(), String(buf).c_str());
    return;
  }else if ((String(topic) == "/FLB/" + String(ssidFL) + "/w_setpoint")) {
    // Aquí puedes agregar la lógica para manejar el mensaje recibido en este topic
    DEBUG_PRINTLN("Intentando escribir el registro w_setpoint...");
    uint16_t value = message.toInt(); // Convertir el mensaje a un número entero
    writeModbus(config.func_w_SetPoint, config.addr_w_SetPoint, value); // Escribir el valor en el registro 1539 del dispositivo Modbus
    mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/w_setpoint_ack").c_str(), String(value).c_str());
    delay(300); // Esperar un segundo antes de repetir
    DEBUG_PRINTLN("Intentando leer el registro SetPoint...");
    if (readModbus(config.func_SetPoint, config.addr_SetPoint, data)) {
      DEBUG_PRINT("Registro SetPoint: ");
      DEBUG_PRINTLN(data);
      snprintf(jsonBuffer, sizeof(jsonBuffer),
        "{\"t\":\"%s\",\"d\":{"
        "\"%s\":%s"
        "},\"v\":\"%s\"}",
        ssidFL,
        "SetPoint", String(data).c_str(),
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
      DEBUG_PRINT("Error de comunicación al leer SetPoint: ");
      DEBUG_PRINTLN(result);
    }    
  } else if ((String(topic) == "/FLB/" + String(ssidFL) + "/w_status")) {
    // Aquí puedes agregar la lógica para manejar el mensaje recibido en este topic
    Serial.println("Intentando escribir el registro w_status...");
    uint16_t value = message.toInt(); // Convertir el mensaje a un número entero
    writeModbus(config.func_w_Status, config.addr_w_Status, value);
    mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/w_status_ack").c_str(), String(value).c_str());
    Serial.println(message);
    delay(100);
    Serial.println("Intentando leer el registro StatusOn...");
    if (readModbus(config.func_StatusOn, config.addr_StatusOn, data)) {
      Serial.print("Registro Status: ");
      Serial.println(data);
      snprintf(jsonBuffer, sizeof(jsonBuffer),
        "{\"t\":\"%s\",\"d\":{"
        "\"%s\":%s"
        "},\"v\":\"%s\"}",
        ssidFL,
        "StatusOn", String(data).c_str(),
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
      Serial.print("Error de comunicación al leer SetPoint: ");
      Serial.println(result, HEX);
    } 
  } else if (String(topic) == "/FLB/" + String(ssidFL) + "/w_LicActiva") {
    // Aquí puedes agregar la lógica para manejar el mensaje recibido en este topic
    Serial.println("Licencia Activa recibida:");
    const char* tipoLicencia = message.c_str(); // Convertir el mensaje a un número entero
    strncpy(config.licenciaActual, tipoLicencia, sizeof(config.licenciaActual) - 1);
    config.licenciaActual[sizeof(config.licenciaActual) - 1] = '\0';
    saveConfigToEEPROM();
    mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/w_LicActiva_ack").c_str(), tipoLicencia);
    delay(100);
  } else if (String(topic) == "/FLB/" + String(ssidFL) + "/w_rstFabrica") {
    // Aquí puedes agregar la lógica para manejar el mensaje recibido en este topic
    Serial.println("Reseteando a fabrica:");
    mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/w_rstFabrica_ack").c_str(), "OK");
    resetToFactory();
  } else if (String(topic) == "/FLB/" + String(ssidFL) + "/w_rstDispositivo") {
    // Aquí puedes agregar la lógica para manejar el mensaje recibido en este topic
    Serial.println("Reseteando dispositivo:");
    mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/w_rstDispositivo_ack").c_str(), "OK");
    resetDispositivo();
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_Probe1") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_Probe1 = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para Probe1: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_Probe1_ack").c_str(), String(newAddress).c_str());
    }
  }else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_SetPoint") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_SetPoint = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para SetPoint: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_SetPoint_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_StatusOn") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_StatusOn = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para StatusOn: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_StatusOn_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_Defrost") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_Defrost = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para Defrost: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_Defrost_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_PtaAbierta") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_PtaAbierta = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para PtaAbierta: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_PtaAbierta_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_ErrorPb1") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_ErrorPb1 = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para ErrorPb1: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_ErrorPb1_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_ErrorPb2") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_ErrorPb2 = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para ErrorPb2: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_ErrorPb2_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_AltoValorPb1") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_AltoValorPb1 = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para AltoValorPb1: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_AltoValorPb1_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_BajoValorPb1") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_BajoValorPb1 = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para BajoValorPb1: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_BajoValorPb1_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_Compressor") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_Compressor = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para Compressor: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_Compressor_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_w_setpoint") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_w_SetPoint = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para w_SetPoint: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_w_setpoint_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/addr_w_status") {
    uint16_t newAddress = message.toInt();
    if (newAddress > 0 && newAddress < 65536) { // Validación básica
      config.addr_w_Status = newAddress;
      saveConfigToEEPROM();
      DEBUG_PRINT("Nueva dirección para w_SetPoint: "); 
      DEBUG_PRINTLN(newAddress);
      // Opcional: enviar confirmación
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/addr_w_status_ack").c_str(), String(newAddress).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_Probe1") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_Probe1 = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_Probe1");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_Probe1_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_SetPoint") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_SetPoint = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_SetPoint");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_SetPoint_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_StatusOn") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_StatusOn = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_StatusOn");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_StatusOn_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_Defrost") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_Defrost = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_Defrost");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_Defrost_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_PtaAbierta") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_PtaAbierta = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_PtaAbierta");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_PtaAbierta_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_ErrorPb1") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_ErrorPb1 = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_ErrorPb1");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_ErrorPb1_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_ErrorPb2") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_ErrorPb2 = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_ErrorPb2");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_ErrorPb2_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_AltoValorPb1") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_AltoValorPb1 = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_AltoValorPb1");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_AltoValorPb1_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_BajoValorPb1") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_BajoValorPb1 = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_BajoValorPb1");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_BajoValorPb1_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_Compressor") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC01_COIL && newFunc <= FC04_INPUT) {
      config.func_Compressor = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_Compressor");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_Compressor_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_w_setpoint") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC05_WSC && newFunc <= FC06_WSR) {
      config.func_w_SetPoint = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_w_SetPoint");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_w_setpoint_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/func_w_status") {
    ModbusFunc newFunc = static_cast<ModbusFunc>(message.toInt());
    if (newFunc >= FC05_WSC && newFunc <= FC06_WSR) {
      config.func_w_Status = newFunc;
      saveConfigToEEPROM();
      DEBUG_PRINTLN("Actualizada func_w_SetPoint");
      mqttClient.publish(("/friolinkBMant/" + String(ssidFL) + "/func_w_status_ack").c_str(), String((int)newFunc).c_str());
    }
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/passDiag") {
    String newPass = message;
    strncpy(config.passDiag, newPass.c_str(), sizeof(config.passDiag) - 1);
    config.passDiag[sizeof(config.passDiag) - 1] = '\0';
    saveConfigToEEPROM();
    DEBUG_PRINT("Nueva contraseña del AP de diagnóstico guardada. Reiniciando para aplicar...");
    mqttClient.publish(("/FLB/" + String(ssidFL) + "/passDiag_ack").c_str(), "OK");
  } else if (topicStr == "/FLB/" + String(ssidFL) + "/sendDiag") {
    String diagnosticJson = getDiagnosticDataAsJson();

    // Verificar que el JSON es válido
    if (diagnosticJson.length() > 0) {
      WiFiClient client;
      client.setTimeout(30000); 
      
      HTTPClient https;
      https.setTimeout(30000);
      
      // Construir URL para el endpoint de diagnóstico
      String urlDiag = String(urlBase) + "login/FLBDiag";
      
      DEBUG_PRINT("URL de diagnóstico: ");
      DEBUG_PRINTLN(urlDiag);
      
      // Preparar el cuerpo del POST con JSON y ssidFL
      String postData = "{\"ssidFL\":\"" + String(ssidFL) + "\",\"diagnosticData\":" + diagnosticJson + "}";
      
      DEBUG_PRINT("JSON a enviar: ");
      DEBUG_PRINTLN(postData);
      
      if (https.begin(client, urlDiag)) {
        // Configurar headers para JSON
        https.addHeader("Content-Type", "application/json");
        https.addHeader("User-Agent", "FrioLink/1.0");
        
        DEBUG_PRINTLN("Enviando POST HTTP...");
        int httpCode = https.POST(postData);
        
        if (httpCode > 0) {
          DEBUG_PRINT("✅ Código HTTP: ");
          DEBUG_PRINTLN(String(httpCode));
          
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
  }
}

void initializeMqtt() {
  mqttClient.setServer(config.mqtt_server, config.mqtt_port);
  mqttClient.setCallback(callbackMQTT);
  DEBUG_PRINTLN("Cliente MQTT inicializado");
}

bool connectToMqtt() {
  mqttClient.setBufferSize(1024); // ✅ SEGURO y suficiente
  if (mqttClient.connected()) return true;
  
  unsigned long currentTime = millis();
  if (currentTime - lastMqttRetry < mqttRetryDelay) {
    return false;
  }
  
  DEBUG_PRINT("Conectando a MQTT...");
  if (mqttClient.connect(ssidFL)) {
    DEBUG_PRINTLN(" ¡Conectado!");
    
    // Suscribirse a tópicos de control
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/w_setpoint", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/w_status", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/w_LicActiva", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/tokenDispositivo", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/mqtt_server", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/mqtt_port", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/licenciaActual", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/tiempoIntervalo", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/w_rstFabrica", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/FLB/%s", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_Probe1", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_SetPoint", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_StatusOn", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_Defrost", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_PtaAbierta", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_ErrorPb1", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_ErrorPb2", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_AltoValorPb1", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_BajoValorPb1", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_Compressor", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_w_setpoint", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/addr_w_status", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/passDiag", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_Probe1", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_SetPoint", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_StatusOn", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_Defrost", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_PtaAbierta", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_ErrorPb1", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_ErrorPb2", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_AltoValorPb1", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_BajoValorPb1", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_Compressor", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_w_setpoint", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/func_w_status", ssidFL);
    mqttClient.subscribe(topicBuffer);
    snprintf(topicBuffer, sizeof(topicBuffer), "/FLB/%s/sendDiag", ssidFL);
    mqttClient.subscribe(topicBuffer);
    
    mqttRetryCount = 0;
    mqttRetryDelay = 5000;

    snprintf(jsonBuffer, sizeof(jsonBuffer),
      "{\"t\":\"%s\",\"d\":{"
      "\"%s\":%s"
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
    }else{
      DEBUG_PRINTLN("No se pueden enviar los datos de reconexión al MQTT");
    }
    return true;
  } else {
    DEBUG_PRINT(" Fallo. Estado: ");
    DEBUG_PRINTLN(mqttClient.state());
    
    lastMqttRetry = currentTime;
    mqttRetryCount++;
    mqttRetryDelay = min(mqttRetryDelay * 2, 30000UL);
    cuentaErroresTotales=cuentaErroresTotales+1;
    if (cuentaErroresTotales>10){
      resetDispositivo();
    }
    DEBUG_PRINT("Cuenta error total: ");
    DEBUG_PRINTLN(cuentaErroresTotales);
    return false;
  }
}

void resetSensorReadCycle() {
  modbusReadState = READING_DEFROST; // Reinicia la máquina de estados
  sensorStatus.cycleComplete = false;
  // Limpiamos los estados de la lectura anterior
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
  erroresBufferMant[0] = '\0'; // Limpiar buffer de errores
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
  // Variables estáticas para recordar el estado entre llamadas
  //static ModbusReadState currentState = READING_PROBE1;
  static unsigned long lastReadTime = 0;
  uint16_t data;

  // Si el ciclo ya está completo, esperamos a que se reinicie desde sendMqttData
  if (currentState == ALL_READINGS_DONE) {
    return;
  }

  // Si no ha pasado suficiente tiempo desde la última lectura, salimos sin hacer nada
  if (millis() - lastReadTime < msEntreSolicitudes) {
    return;
  }

  // Máquina de estados: lee un sensor, actualiza su estado y avanza al siguiente
  switch (modbusReadState) {
    case READING_DEFROST:
      if (readModbus(config.func_Defrost, config.addr_Defrost, data)) {
        strncpy(config.Defrost, String(data).c_str(), sizeof(config.Defrost) - 1);
        config.Defrost[sizeof(config.Defrost) - 1] = '\0';
        sensorStatus.ok_Defrost = true;
        DEBUG_PRINTLN("OK Defrost: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer Defrost en la dirección: " + String(config.addr_Defrost));
        registrarError("Defrost");
        strncpy(config.Defrost, String("Error").c_str(), sizeof(config.Defrost) - 1);
        config.Probe1[sizeof(config.Defrost) - 1] = '\0';
      }
      modbusReadState = READING_PTAABIERTA;
      break;
      
    case READING_PTAABIERTA:
      if (readModbus(config.func_PtaAbierta, config.addr_PtaAbierta, data)) {
        strncpy(config.PtaAbierta, String(data).c_str(), sizeof(config.PtaAbierta) - 1);
        config.PtaAbierta[sizeof(config.PtaAbierta) - 1] = '\0';
        sensorStatus.ok_PtaAbierta = true;
        DEBUG_PRINTLN("OK PtaAbierta: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer PtaAbierta en la dirección: " + String(config.addr_PtaAbierta));
        registrarError("PtaAbierta");
        strncpy(config.PtaAbierta, String("Error").c_str(), sizeof(config.PtaAbierta) - 1);
        config.Probe1[sizeof(config.PtaAbierta) - 1] = '\0';
      }
      modbusReadState = READING_ERRORPB1;
      break;

    case READING_ERRORPB1:
      if (readModbus(config.func_ErrorPb1, config.addr_ErrorPb1, data)) {
        strncpy(config.ErrorPb1, String(data).c_str(), sizeof(config.ErrorPb1) - 1);
        config.ErrorPb1[sizeof(config.ErrorPb1) - 1] = '\0';
        sensorStatus.ok_ErrorPb1 = true;
        DEBUG_PRINTLN("OK ErrorPb1: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer ErrorPb1 en la dirección: " + String(config.addr_ErrorPb1));
        registrarError("ErrorPb1");
        strncpy(config.ErrorPb1, String("Error").c_str(), sizeof(config.ErrorPb1) - 1);
        config.Probe1[sizeof(config.ErrorPb1) - 1] = '\0';
      }
      modbusReadState = READING_ERRORPB2;
      break;

    case READING_ERRORPB2:
      if (readModbus(config.func_ErrorPb2, config.addr_ErrorPb2, data)) {
        strncpy(config.ErrorPb2, String(data).c_str(), sizeof(config.ErrorPb2) - 1);
        config.ErrorPb2[sizeof(config.ErrorPb2) - 1] = '\0';
        sensorStatus.ok_ErrorPb2 = true;
        DEBUG_PRINTLN("OK ErrorPb2: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer ErrorPb2 en la dirección: " + String(config.addr_ErrorPb2));
        registrarError("ErrorPb2");
        strncpy(config.ErrorPb2, String("Error").c_str(), sizeof(config.ErrorPb2) - 1);
        config.Probe1[sizeof(config.ErrorPb2) - 1] = '\0';
      }
      modbusReadState = READING_ALTOVALORPB1;
      break;

    case READING_ALTOVALORPB1:
      if (readModbus(config.func_AltoValorPb1, config.addr_AltoValorPb1, data)) {
        strncpy(config.AltoValorPb1, String(data).c_str(), sizeof(config.AltoValorPb1) - 1);
        config.AltoValorPb1[sizeof(config.AltoValorPb1) - 1] = '\0';
        sensorStatus.ok_AltoValorPb1 = true;
        DEBUG_PRINTLN("OK AltoValorPb1: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer AltoValorPb1 en la dirección: " + String(config.addr_AltoValorPb1));
        registrarError("AltoValorPb1");
        strncpy(config.AltoValorPb1, String("Error").c_str(), sizeof(config.AltoValorPb1) - 1);
        config.Probe1[sizeof(config.AltoValorPb1) - 1] = '\0';
      }
      modbusReadState = READING_BAJOVALORPB1;
      break;

    case READING_BAJOVALORPB1:
      if (readModbus(config.func_BajoValorPb1, config.addr_BajoValorPb1, data)) {
        strncpy(config.BajoValorPb1, String(data).c_str(), sizeof(config.BajoValorPb1) - 1);
        config.BajoValorPb1[sizeof(config.BajoValorPb1) - 1] = '\0';
        sensorStatus.ok_BajoValorPb1 = true;
        DEBUG_PRINTLN("OK BajoValorPb1: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer BajoValorPb1 en la dirección: " + String(config.addr_BajoValorPb1));
        registrarError("BajoValorPb1");
        strncpy(config.BajoValorPb1, String("Error").c_str(), sizeof(config.BajoValorPb1) - 1);
        config.Probe1[sizeof(config.BajoValorPb1) - 1] = '\0';
      }
      modbusReadState = READING_COMPRESSOR;
      break;

    case READING_COMPRESSOR:
      if (readModbus(config.func_Compressor, config.addr_Compressor, data)) {
        strncpy(config.Compressor, String(data).c_str(), sizeof(config.Compressor) - 1);
        config.Compressor[sizeof(config.Compressor) - 1] = '\0';
        sensorStatus.ok_Compressor = true;
        DEBUG_PRINTLN("OK Compressor: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer Compressor en la dirección: " + String(config.addr_Compressor));
        registrarError("Compressor");
        strncpy(config.Compressor, String("Error").c_str(), sizeof(config.Compressor) - 1);
        config.Probe1[sizeof(config.Compressor) - 1] = '\0';
      }
      // Último sensor leído, marcamos el ciclo como completado
      modbusReadState = READING_PROBE1;
      break;
    
    case READING_PROBE1:
      if (readModbus(config.func_Probe1, config.addr_Probe1, data)) {
        strncpy(config.Probe1, String(data).c_str(), sizeof(config.Probe1) - 1);
        config.Probe1[sizeof(config.Probe1) - 1] = '\0';
        sensorStatus.ok_Probe1 = true;
        DEBUG_PRINTLN("OK Probe1: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer Probe1 en la dirección: " + String(config.addr_Probe1));
        registrarError("Probe1");
        strncpy(config.Probe1, String("Error").c_str(), sizeof(config.Probe1) - 1);
        config.Probe1[sizeof(config.SetPoint) - 1] = '\0';     
      }
      modbusReadState = READING_SETPOINT;
      break;

    case READING_SETPOINT:
      if (readModbus(config.func_SetPoint, config.addr_SetPoint, data)) {
        strncpy(config.SetPoint, String(data).c_str(), sizeof(config.SetPoint) - 1);
        config.SetPoint[sizeof(config.SetPoint) - 1] = '\0';
        sensorStatus.ok_SetPoint = true;
        DEBUG_PRINTLN("OK SetPoint: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer SetPoint en la dirección: " + String(config.addr_SetPoint));
        registrarError("SetPoint");
        strncpy(config.SetPoint, String("Error").c_str(), sizeof(config.SetPoint) - 1);
        config.Probe1[sizeof(config.SetPoint) - 1] = '\0';
      }
      modbusReadState = READING_STATUSON;
      break;

    case READING_STATUSON:
      if (readModbus(config.func_StatusOn, config.addr_StatusOn, data)) {
        strncpy(config.StatusOn, String(data).c_str(), sizeof(config.StatusOn) - 1);
        config.StatusOn[sizeof(config.StatusOn) - 1] = '\0';
        sensorStatus.ok_StatusOn = true;
        DEBUG_PRINTLN("OK StatusOn: "+String(data));
      } else {
        DEBUG_PRINTLN("Error de comunicación al leer StatusOn en la dirección: " + String(config.addr_StatusOn));
        registrarError("StatusOn");
        strncpy(config.StatusOn, String("Error").c_str(), sizeof(config.StatusOn) - 1);
        config.Probe1[sizeof(config.StatusOn) - 1] = '\0';
      }
      modbusReadState = ALL_READINGS_DONE;
      sensorStatus.cycleComplete = true;
      break;
  }

  // Actualizamos el tiempo de la última acción
  lastReadTime = millis();
}

void sendMqttData() {
  if (strcmp(config.licenciaActual, "1") != 0) {
    DEBUG_PRINT("No pude transmitir por tipo de licencia:");
    DEBUG_PRINTLN(config.licenciaActual);
    resetSensorReadCycle(); // Reiniciamos el ciclo para el próximo intento
    return;
  }

  char jsonBuffer[512];
  snprintf(jsonBuffer, sizeof(jsonBuffer), "{\"t\":\"%s\",\"d\":{", ssidFL);
  bool firstField = true;
  
  #define ADD_FIELD(cond, key, val) \
    if (cond) { \
      if (!firstField) strncat(jsonBuffer, ",", sizeof(jsonBuffer) - strlen(jsonBuffer) - 1); \
      char tempField[64]; \
      snprintf(tempField, sizeof(tempField), "\"%s\":%s", key, val); \
      strncat(jsonBuffer, tempField, sizeof(jsonBuffer) - strlen(jsonBuffer) - 1); \
      firstField = false; \
    }
  
  ADD_FIELD(sensorStatus.ok_Defrost,      "Defrost",      config.Defrost);
  ADD_FIELD(sensorStatus.ok_PtaAbierta,   "PtaAbierta",   config.PtaAbierta);
  ADD_FIELD(sensorStatus.ok_ErrorPb1,     "ErrorPb1",     config.ErrorPb1);
  ADD_FIELD(sensorStatus.ok_ErrorPb2,     "ErrorPb2",     config.ErrorPb2);
  ADD_FIELD(sensorStatus.ok_AltoValorPb1, "AltoValorPb1", config.AltoValorPb1);
  ADD_FIELD(sensorStatus.ok_BajoValorPb1, "BajoValorPb1", config.BajoValorPb1);
  ADD_FIELD(sensorStatus.ok_Compressor,   "Compressor",   config.Compressor);
  ADD_FIELD(sensorStatus.ok_Probe1,       "Probe1",       config.Probe1);
  ADD_FIELD(sensorStatus.ok_SetPoint,     "SetPoint",     config.SetPoint);
  ADD_FIELD(sensorStatus.ok_StatusOn,     "StatusOn",     config.StatusOn);
  ADD_FIELD(true, "licActiva", config.licenciaActual);
  
  // Cierra JSON
  strncat(jsonBuffer, "},", sizeof(jsonBuffer) - strlen(jsonBuffer) - 1);
  char versionPart[32];
  snprintf(versionPart, sizeof(versionPart), "\"v\":\"%s\",", VERSION);
  strncat(jsonBuffer, versionPart, sizeof(jsonBuffer) - strlen(jsonBuffer) - 1);
  char HVersionPart[32];
  snprintf(HVersionPart, sizeof(HVersionPart), "\"hv\":\"%s\"}", HVERSION);
  strncat(jsonBuffer, HVersionPart, sizeof(jsonBuffer) - strlen(jsonBuffer) - 1);

  DEBUG_PRINTLN(jsonBuffer);

  connectToMqtt();
  
  snprintf(topicBuffer, sizeof(topicBuffer), "/%s", codModelo);
  bool resultMQTT = mqttClient.publish(topicBuffer, jsonBuffer);
  
  if (resultMQTT) {
    DEBUG_PRINTLN("Datos MQTT enviados desde sendBatchMqttData");
  } else {
    DEBUG_PRINTLN("Error enviando datos MQTT desde sendBatchMqttData");
    handleError(ERROR_MQTT_FAILED);
  }

  if (huboErrorMant) {
    DEBUG_PRINTLN("Hubo errores al leer los datos en sendBatchMqttData, se envian a mantenimiento ");
    snprintf(jsonBuffer, sizeof(jsonBuffer), 
             "{\"t\":\"%s\",\"errores\":\"%s\",\"v\":\"%s\"}",
             ssidFL, erroresBufferMant, VERSION);
  
    snprintf(topicBuffer, sizeof(topicBuffer), "/friolinkBMant/FLB/ErrorLectura/%s", ssidFL);
  
    resultMQTT = mqttClient.publish(topicBuffer, jsonBuffer);
  
    if (resultMQTT) {
      DEBUG_PRINTLN("Errores enviados al topic de mantenimiento desde sendBatchMqttData");
    } else {
      DEBUG_PRINTLN("Error enviando datos MQTT de mantenimiento desde sendBatchMqttData");
      handleError(ERROR_MQTT_FAILED);
    }
  }

  // Importante: reiniciamos el ciclo de lectura para que comience de nuevo
  resetSensorReadCycle();
}
