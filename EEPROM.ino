// ======================== GESTIÓN DE CONFIGURACIÓN ========================

void loadConfigFromEEPROM() {
  EEPROM.get(0, config);
  
  uint32_t calculatedChecksum = calculateChecksum(&config);
  if (config.checksum != calculatedChecksum) {
    DEBUG_PRINTLN("Configuración corrupta, usando valores por defecto");

    // Restaurar valores por defecto
    config.checksum = calculateChecksum(&config);
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
  } else {
    //strncpy(config.stored_password, "SinClave", sizeof(config.stored_password) - 1);
    //config.stored_password[sizeof(config.stored_password) - 1] = '\0';
    DEBUG_PRINTLN("Configuración cargada desde EEPROM");
  }
}

void saveConfigToEEPROM() {
  uint32_t newChecksum = calculateChecksum(&config);
  if (config.checksum != newChecksum) {
    config.checksum = newChecksum;
    EEPROM.put(0, config);
    EEPROM.commit();
    DEBUG_PRINTLN("Configuración guardada en EEPROM");
  }
}
