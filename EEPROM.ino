// ======================== GESTIÓN DE CONFIGURACIÓN ========================
//
// Este archivo asume que `struct EepromHeader` y `struct ConfigData` están
// declarados en FrioLinkB.ino (o en un header común).

extern ConfigData config;
extern uint32_t calculateChecksum(const ConfigData* data);
extern void DEBUG_PRINTLN(const char*);

// Offsets en EEPROM
const size_t HEADER_OFFSET = 0;
const size_t CONFIG_OFFSET = HEADER_OFFSET + sizeof(EepromHeader);

void applyDefaultsToConfig(ConfigData &cfg) {
  memset(&cfg, 0, sizeof(cfg));

  strncpy(cfg.stored_ssid, ssidDefecto, sizeof(cfg.stored_ssid) - 1);
  strncpy(cfg.stored_password, passWIFI, sizeof(cfg.stored_password) - 1);
  strncpy(cfg.tokenDispositivo, tokenDefecto, sizeof(cfg.tokenDispositivo) - 1);
  strncpy(cfg.mqtt_server, mqtt_server_default, sizeof(cfg.mqtt_server) - 1);
  cfg.mqtt_port = mqtt_port_default;
  strncpy(cfg.licenciaActual, licActual_default, sizeof(cfg.licenciaActual) - 1);
  strncpy(cfg.tiempoIntervalo, tiempoIntervalo_default, sizeof(cfg.tiempoIntervalo) - 1);
  cfg.addr_Probe1 = addr_Probe1_default;
  cfg.addr_SetPoint = addr_SetPoint_default;
  cfg.addr_StatusOn = addr_StatusOn_default;
  cfg.addr_Defrost = addr_Defrost_default;
  cfg.addr_PtaAbierta = addr_PtaAbierta_default;
  cfg.addr_ErrorPb1 = addr_ErrorPb1_default;
  cfg.addr_ErrorPb2 = addr_ErrorPb2_default;
  cfg.addr_AltoValorPb1 = addr_AltoValorPb1_default;
  cfg.addr_BajoValorPb1 = addr_BajoValorPb1_default;
  cfg.addr_Compressor = addr_Compressor_default;
  cfg.addr_w_SetPoint = addr_w_SetPoint_default;
  cfg.addr_w_Status = addr_w_Status_default;
  strncpy(cfg.passDiag, passDiag_default, sizeof(cfg.passDiag) - 1);
  cfg.useStaticIP = useStaticIP_default;
  strncpy(cfg.staticIP, staticIP_default, sizeof(cfg.staticIP) - 1);
  strncpy(cfg.staticGateway, staticGateway_default, sizeof(cfg.staticGateway) - 1);
  strncpy(cfg.staticSubnet, staticSubnet_default, sizeof(cfg.staticSubnet) - 1);
  strncpy(cfg.staticDNS, staticDNS_default, sizeof(cfg.staticDNS) - 1);
  cfg.func_Probe1 = func_Probe1_default;
  cfg.func_SetPoint = func_SetPoint_default;
  cfg.func_StatusOn = func_StatusOn_default;
  cfg.func_Defrost = func_Defrost_default;
  cfg.func_PtaAbierta = func_PtaAbierta_default;
  cfg.func_ErrorPb1 = func_ErrorPb1_default;
  cfg.func_ErrorPb2 = func_ErrorPb2_default;
  cfg.func_AltoValorPb1 = func_AltoValorPb1_default;
  cfg.func_BajoValorPb1 = func_BajoValorPb1_default;
  cfg.func_Compressor = func_Compressor_default;
  cfg.func_w_SetPoint = func_w_SetPoint_default;
  cfg.func_w_Status = func_w_Status_default;

  cfg.checksum = calculateChecksum(&cfg);
}

void loadConfigFromEEPROM() {
  EepromHeader header;
  EEPROM.get(HEADER_OFFSET, header);

  bool needDefaults = false;

  if (header.magic != EEPROM_MAGIC || header.version != EEPROM_VERSION) {
    DEBUG_PRINTLN("EEPROM header inválido o versión distinta -> aplicar defaults");
    needDefaults = true;
  } else {
    EEPROM.get(CONFIG_OFFSET, config);
    uint32_t calculatedChecksum = calculateChecksum(&config);
    if (config.checksum != calculatedChecksum) {
      DEBUG_PRINTLN("Checksum inválido en EEPROM -> aplicar defaults");
      needDefaults = true;
    } else {
      DEBUG_PRINTLN("Configuración cargada desde EEPROM (checksum OK)");
      return;
    }
  }

  if (needDefaults) {
    applyDefaultsToConfig(config);
    header.magic = EEPROM_MAGIC;
    header.version = EEPROM_VERSION;
    EEPROM.put(HEADER_OFFSET, header);
    EEPROM.put(CONFIG_OFFSET, config);
    EEPROM.commit();
    DEBUG_PRINTLN("Defaults aplicados y guardados en EEPROM");
  }
}

void saveConfigToEEPROM() {
  uint32_t newChecksum = calculateChecksum(&config);
  if (config.checksum != newChecksum) {
    config.checksum = newChecksum;
    EepromHeader header = { EEPROM_MAGIC, EEPROM_VERSION };
    EEPROM.put(HEADER_OFFSET, header);
    EEPROM.put(CONFIG_OFFSET, config);
    EEPROM.commit();
    DEBUG_PRINTLN("Configuración guardada en EEPROM");
  } else {
    DEBUG_PRINTLN("saveConfigToEEPROM: no hay cambios (checksum igual)");
  }
}

