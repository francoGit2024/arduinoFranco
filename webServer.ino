// ======================== SERVIDOR WEB OPTIMIZADO ========================
extern const char credPageA[];
extern const char credPageB[];
extern const char credPageB1[];
extern const char credPageC[];
extern const char credPageScripts[];
extern const char scanPageHeaderConf[];
extern const char credPageCConf[];
extern const char credPageScriptsConf[];
extern const char resultPageHeaderConfFin[];
extern const char resultPageSuccessConfFin[];
extern const char resultPageSuccessMiddleConfFin[];
extern const char resultPageErrorConfFin[];
extern const char resultPageErrorMiddleConfFin[];
extern const char resultPageErrorEndConfFin[];
extern const char resultPageScriptConfFin[];
extern const char resultPageCountdownConfFin[];
extern const char networkConfigPageHeader[];
extern const char networkConfigPageTitle[];
extern const char networkConfigPageDevice[];
extern const char networkConfigPageVersion[];
extern const char networkConfigPageStaticChecked[];
extern const char networkConfigPageStaticDisplay[];
extern const char networkConfigPageStaticIP[];
extern const char networkConfigPageStaticGateway[];
extern const char networkConfigPageStaticSubnet[];
extern const char networkConfigPageStaticDNS[];
extern const char networkConfigPageAPSSID[];
extern const char networkConfigPageAPPassword[];
extern const char networkConfigPageAPChannelOption[];
extern const char networkConfigPageAPChannelSelected[];
extern const char networkConfigPageAPChannel[];
extern const char networkConfigPageAPChannelEnd[];

int current_page = 1; // 1=intro, 2=scan, 3=credentials, 4=result
String selected_ssid = "";
String user_code = "";

void handleRoot() {
  if (currentState == STATE_NORMAL_OPERATION) {
    String htmlDiag = getDiagnosticPageHTML();
    server.send(200, "text/html", htmlDiag.c_str());
    return;
  }

  current_page = 1;
  String htmlIntro = getIntroPageHTML();
  server.send(200, "text/html", htmlIntro.c_str());
}

void handleIntro() {
  current_page = 1;
  String htmlRoot = getIntroPageHTML();
  server.send(200, "text/html", htmlRoot.c_str());
}

void handleScanPage() {
  current_page = 2;
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  server.sendContent_P(scanPageHeader);
}

void handleCredentials() {
  current_page = 3;
  if (server.hasArg("ssid")) {
    selected_ssid = server.arg("ssid");
  }

  char rssiWarningHtml[128];
  rssiWarningHtml[0] = '\0';
  if (server.hasArg("rssi")) {
    int rssi = server.arg("rssi").toInt();
    if (rssi < -64) {
      snprintf(rssiWarningHtml, sizeof(rssiWarningHtml),
        "<div class='warning-message'><strong>⚠️ Advertencia:</strong> La señal de esta red es débil (%d dBm). Puede haber cortes o una conexión inestable.</div>",
        rssi);
    }
  }

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");

  server.sendContent_P(credPageA);
  yield();
  server.sendContent_P(credPageB);
  yield();
  server.sendContent(ssidFL);
  yield();
  server.sendContent_P(credPageB1);
  yield();

  server.sendContent(selected_ssid.c_str());
  yield();

  if (rssiWarningHtml[0] != '\0') {
    server.sendContent(rssiWarningHtml);
    yield();
  }
  server.sendContent_P(credPageC);
  yield();
  server.sendContent_P(credPageScripts);
  yield();
  server.sendContent("");
  yield();
}

void handleSelectNetwork() {
  if (server.hasArg("ssid")) {
    selected_ssid = server.arg("ssid");
    server.send(200, "application/json", "{\"success\":true,\"message\":\"Red seleccionada\"}");
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Error al seleccionar red\"}");
  }
}

void handleResult() {
  current_page = 4;
  bool connection_success = (WiFi.status() == WL_CONNECTED);

  DEBUG_PRINT("Free Heap antes de enviar página: ");
  DEBUG_PRINTLN(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");

  server.sendContent_P(resultPageHeaderConfFin);

  if (connection_success) {
    server.sendContent_P(resultPageSuccessConfFin);
    server.sendContent(config.stored_ssid);
    server.sendContent_P(resultPageSuccessMiddleConfFin);
  } else {
    server.sendContent_P(resultPageErrorConfFin);
    server.sendContent(selected_ssid.c_str());
    server.sendContent_P(resultPageErrorMiddleConfFin);
    String wifiStatus = getWiFiStatusString();
    server.sendContent(wifiStatus.c_str());
    server.sendContent_P(resultPageErrorEndConfFin);
  }

  server.sendContent_P(resultPageScriptConfFin);

  if (connection_success) {
    server.sendContent_P(resultPageCountdownConfFin);
  }

  server.sendContent("");

  DEBUG_PRINT("Free Heap después de enviar página: ");
  DEBUG_PRINTLN(ESP.getFreeHeap());
}

void handleRestart() {
  server.send(200, "text/plain", "Reiniciando dispositivo...");
  delay(1000);
  ESP.restart();
}

void handleStatus() {
  StaticJsonDocument<256> doc;
  doc["mode"] = config_mode ? "config" : "normal";
  doc["wifi_status"] = getWiFiStatusString();
  if (WiFi.status() == WL_CONNECTED) {
    doc["ip"] = WiFi.localIP().toString();
  } else {
    doc["ip"] = "No conectado";
  }
  doc["stored_ssid"] = config.stored_ssid;

  char buf[512];
  size_t n = serializeJson(doc, buf, sizeof(buf));
  (void)n;
  server.send(200, "application/json", buf);
}

void handleNotFound() {
  server.send(404, "text/html", "<h1>Página no encontrada</h1><p><a href='/'>Volver al inicio</a></p>");
}

void handleRootConf() {
  current_page = 1;
  String htmlIntro = getIntroPageHTMLConf();
  server.send(200, "text/html", htmlIntro.c_str());
}

void handleIntroConf() {
  current_page = 1;
  String htmlRoot = getIntroPageHTMLConf();
  server.send(200, "text/html", htmlRoot.c_str());
}

void handleScanPageConf() {
  current_page = 2;
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  server.sendContent_P(scanPageHeaderConf);
}

void handleCredentialsConf() {
  current_page = 3;
  if (server.hasArg("ssid")) {
    selected_ssid = server.arg("ssid");
  }
  char rssiWarningHtml[128];
  rssiWarningHtml[0] = '\0';
  if (server.hasArg("rssi")) {
    int rssi = server.arg("rssi").toInt();
    if (rssi < -64) {
      snprintf(rssiWarningHtml, sizeof(rssiWarningHtml),
        "<div class='warning-message'><strong>⚠️ Advertencia:</strong> La señal de esta red es débil (%d dBm). Puede haber cortes o una conexión inestable.</div>",
        rssi);
    }
  }

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");

  server.sendContent_P(credPageA);
  yield();
  server.sendContent_P(credPageB);
  yield();
  server.sendContent(ssidFL);
  yield();
  server.sendContent_P(credPageB1);
  yield();

  server.sendContent(selected_ssid.c_str());
  yield();

  if (rssiWarningHtml[0] != '\0') {
    server.sendContent(rssiWarningHtml);
    yield();
  }

  server.sendContent_P(credPageCConf);
  yield();
  server.sendContent_P(credPageScriptsConf);
  yield();
  server.sendContent("");
  yield();
}

void handleSelectNetworkConf() {
  if (server.hasArg("ssid")) {
    selected_ssid = server.arg("ssid");
    server.send(200, "application/json", "{\"success\":true,\"message\":\"Red seleccionada\"}");
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Error al seleccionar red\"}");
  }
}

void setupWebServer() {
  char dbgBuf[128];
  snprintf(dbgBuf, sizeof(dbgBuf), "Token:%s", config.tokenDispositivo);
  DEBUG_PRINTLN(dbgBuf);

  if (strcmp(config.tokenDispositivo, "000000000000000000000000") != 0
        && strlen(config.tokenDispositivo) != 0) {
    server.on("/", handleRoot);
    server.on("/intro", handleIntroConf);
    server.on("/scan-page", handleScanPageConf);
    server.on("/scan", handleScanConf);
    server.on("/credentials", handleCredentialsConf);
    server.on("/select-network", handleSelectNetworkConf);
    server.on("/save", handleSaveConf);
    server.on("/result", handleResult);
    server.on("/restart", handleRestart);
    server.on("/status", handleStatus);
    server.onNotFound(handleNotFound);
  } else {
    server.on("/", handleRoot);
    server.on("/intro", handleIntro);
    server.on("/scan-page", handleScanPage);
    server.on("/scan", handleScan);
    server.on("/credentials", handleCredentials);
    server.on("/select-network", handleSelectNetwork);
    server.on("/save", handleSave);
    server.on("/result", handleResult);
    server.on("/restart", handleRestart);
    server.on("/status", handleStatus);
    server.onNotFound(handleNotFound);
  }

  server.on("/diagnostic", []() {
    String htmlDiag = getDiagnosticPageHTML();
    server.send(200, "text/html", htmlDiag.c_str());
  });

  server.on("/network-config", handleNetworkConfig);
  server.on("/save_static_ip", handleSaveStaticIP);
  server.on("/save_wifi_client", handleSaveWifiClient);
  server.on("/save_ap_config", handleSaveApConfig);
  server.on("/save_diag_settings", handleSaveDiagSettings);
  server.on("/statusDixell.json", handleStatusDixellJson);
}

void handleStatusDixellJson() {
  StaticJsonDocument<512> doc;
  doc["dixell_status"] = conexDixell;
  doc["dixell_checked"] = isVerificationComplete() ? true : false;
  doc["signals_ok"] = señalesOK;
  doc["signals_error"] = señalesError;

  char buf[512];
  size_t n = serializeJson(doc, buf, sizeof(buf));
  (void)n;
  server.send(200, "application/json", buf);
}

void handleSaveStaticIP() {
  DEBUG_PRINTLN("Recibida solicitud para guardar configuración de IP estática.");

  bool useStatic = server.hasArg("useStaticIP");
  String ip = server.arg("staticIP");
  String gateway = server.arg("staticGateway");
  String subnet = server.arg("staticSubnet");
  String dns = server.arg("staticDNS");

  auto isValidIP = [](const String& s) {
    int dots = 0;
    for (int i = 0; i < s.length(); ++i) {
      char c = s.charAt(i);
      if (c == '.') dots++;
      else if (c < '0' || c > '9') return false;
    }
    return dots == 3;
  };

  if (useStatic && (!isValidIP(ip) || !isValidIP(gateway) || !isValidIP(subnet) || !isValidIP(dns))) {
    server.send(400, "text/html", "<h1>Error</h1><p>Una o más direcciones IP tienen un formato inválido.</p><a href='/'>Volver</a>");
    return;
  }

  config.useStaticIP = useStatic;
  strncpy(config.staticIP, ip.c_str(), sizeof(config.staticIP) - 1);
  config.staticIP[sizeof(config.staticIP) - 1] = '\0';
  strncpy(config.staticGateway, gateway.c_str(), sizeof(config.staticGateway) - 1);
  config.staticGateway[sizeof(config.staticGateway) - 1] = '\0';
  strncpy(config.staticSubnet, subnet.c_str(), sizeof(config.staticSubnet) - 1);
  config.staticSubnet[sizeof(config.staticSubnet) - 1] = '\0';
  strncpy(config.staticDNS, dns.c_str(), sizeof(config.staticDNS) - 1);
  config.staticDNS[sizeof(config.staticDNS) - 1] = '\0';

  saveConfigToEEPROM();

  server.send(200, "text/html", "<h1>Configuración Guardada</h1><p>El dispositivo se reiniciará para aplicar los cambios. Por favor, espere...</p>");

  DEBUG_PRINTLN("Configuración de IP guardada. Reiniciando dispositivo en 2 segundos...");
  delay(2000);
  ESP.restart();
}

