String getIntroPageHTML() {
  String html = R"(
    <!DOCTYPE html>
    <html lang='es'>
    <head>
        <meta charset='UTF-8'>
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>
        <title>Bienvenido - )" + String(modeloFL) + R"(</title>
        <style>
            body {
                font-family: Arial, sans-serif;
                margin: 0;
                padding: 20px;
                background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                min-height: 100vh;
            }
            .container {
                max-width: 500px;
                margin: 0 auto;
                background: white;
                border-radius: 10px;
                box-shadow: 0 10px 30px rgba(0,0,0,0.3);
                padding: 30px;
                text-align: center;
            }
            .header {
                margin-bottom: 30px;
            }
            .header img {
                max-width: 200px;
                height: auto;
                margin-bottom: 15px;
            }
            .header h1 {
                color: #333;
                margin: 0;
                font-size: 24px;
            }
            .header p {
                color: #666;
                margin: 10px 0 0 0;
            }
            .welcome-content {
                text-align: left;
                margin: 30px 0;
                line-height: 1.6;
            }
            .welcome-content h2 {
                color: #333;
                margin-bottom: 15px;
            }
            .welcome-content p {
                color: #666;
                margin-bottom: 15px;
            }
            .welcome-content ul {
                color: #666;
                margin-left: 20px;
            }
            .welcome-content li {
                margin-bottom: 8px;
            }
            .btn {
                background: #667eea;
                color: white;
                padding: 15px 30px;
                border: none;
                border-radius: 5px;
                cursor: pointer;
                font-size: 16px;
                width: 100%;
                margin-top: 20px;
            }
            .btn:hover {
                background: #5a6fd8;
            }
            .highlight {
                background: #f8f9fa;
                padding: 15px;
                border-radius: 5px;
                border-left: 4px solid #667eea;
                margin: 20px 0;
            }
        </style>
    </head>
    <body>
        <div class='container'>
            <div class='header'>
                <h1>FRIOLINK</h1>
                <p>Asistente de configuración</p>
            </div>
            
            <div class='welcome-content'>
                <h2>¡Bienvenido al Asistente de Configuración!</h2>
                
                <p>Este asistente te guiará paso a paso para conectar tu dispositivo a la red WiFi de manera fácil y segura.</p>
                
                <div class='highlight'>
                    <strong>¿Qué haremos?</strong>
                    <ul>
                        <li>🔍 <strong>Escanearemos</strong> las redes WiFi disponibles</li>
                        <li>🎯 <strong>Seleccionaremos</strong> tu red preferida</li>
                        <li>🔐 <strong>Configuraremos</strong> las credenciales de acceso</li>
                        <li>✅ <strong>Verificaremos</strong> que la conexión funcione correctamente</li>
                    </ul>
                </div>
                
                <p><strong>Tiempo estimado:</strong> 2-3 minutos</p>
                
                <p>El proceso es completamente seguro y tus credenciales se almacenan de forma cifrada en el dispositivo.</p>
                
                <p><em>¡Comencemos a configurar tu conexión WiFi!</em></p>
            </div>
            
            <button class='btn' onclick='goToScan()'>
                ▶️ Comenzar Configuración
            </button>
        </div>
    
        <script>
            function goToScan() {
                window.location.href = '/scan-page';
            }
        </script>
    </body>
    </html>
    )";
  
  return html;
}

const char scanPageHeader[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='es'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>Escaneo de Redes</title>
  <style>
    body { font-family: Arial; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); margin: 0; padding: 20px; min-height: 100vh; }
    .container { max-width: 500px; margin: 0 auto; background: white; border-radius: 10px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); padding: 30px; }
    .header { text-align: center; margin-bottom: 30px; }
    .header h1 { color: #333; margin: 0; font-size: 20px; }
    .progress { display: flex; justify-content: space-between; margin-bottom: 30px; font-size: 12px; }
    .progress-step { text-align: center; flex: 1; }
    .progress-step.active { color: #667eea; font-weight: bold; }
    .progress-step.completed { color: #28a745; }
    .btn { background: #667eea; color: white; padding: 12px 24px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; margin: 5px; }
    .btn:hover { background: #5a6fd8; }
    .btn-secondary { background: #6c757d; }
    .btn-secondary:hover { background: #5a6268; }
    .scan-btn { background: #28a745; width: 100%; margin-bottom: 20px; }
    .scan-btn:hover { background: #218838; }
    .network-item { padding: 15px; border: 2px solid #ddd; border-radius: 5px; margin: 10px 0; cursor: pointer; display: flex; justify-content: space-between; align-items: center; transition: all 0.3s; }
    .network-item:hover { background: #f8f9fa; border-color: #667eea; }
    .network-item.selected { background: #e3f2fd; border-color: #667eea; box-shadow: 0 0 10px rgba(102, 126, 234, 0.3); }
    .signal-strength { font-size: 12px; color: #666; }
    .message { padding: 10px; border-radius: 5px; margin: 10px 0; display: none; }
    .success { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
    .error { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    .navigation { display: flex; justify-content: space-between; margin-top: 30px; }
    .loading { text-align: center; padding: 20px; }
    .full-width-justified { text-align: justify; width: 100%; max-width: 100%; word-wrap: break-word; hyphens: auto; }
    /* Estilos para las alturas de las barras */
    .bar-height-1 { height: 4px; }  /* 20% de la altura total */
    .bar-height-2 { height: 8px; }  /* 40% */
    .bar-height-3 { height: 12px; } /* 60% */
    .bar-height-4 { height: 16px; } /* 80% */
    .bar-height-5 { height: 20px; } /* 100% */
    
    /* Contenedor de las barras de señal */
    .signal-bars {
      display: inline-flex;
      align-items: flex-end; /* Alinear las barras en la base */
      flex-direction: row-reverse; /* --> CAMBIO CLAVE: Invierte el orden de las barras */
      height: 20px; /* Altura fija del contenedor */
      gap: 2px; /* Espacio entre barras */
      margin-left: 8px;
      vertical-align: middle;
    }
    
    /* Estilo para cada barra individual */
    .bar {
      width: 4px;
      background-color: #9e9e9e; /* --> AÑADE !important AQUÍ */
      border-radius: 1px;
      transition: height 0.3s ease; /* Animación suave */
    }

    .signal-bars .bar.on {
      /* Este es el color por defecto, pero será sobrescrito por las clases de color de abajo */
    }
    /* Clases para los colores de las barras "encendidas" */
    .bar.on.color-excelente { background-color: #00c851; } /* Verde Material Design */
    .bar.on.color-muy-buena { background-color: #00c851; }
    .bar.on.color-buena { background-color: #ffd600; } /* Amarillo Material Design */
    .bar.on.color-regular { background-color: #ff9100; } /* Naranja Material Design */
    .bar.on.color-mala { background-color: #ff5252; } /* Rojo Material Design */
    .bar.on.color-muy-mala { 
      background-color: #d32f2f; /* Rojo fuerte Material Design */
      animation: parpadeo 1s infinite;
    }
    
    /* Animación para la señal muy mala */
    @keyframes parpadeo {
      0% { opacity: 1; }
      50% { opacity: 0.5; }
      100% { opacity: 1; }
    }
  </style>
</head>
<body>
  <div class='container'>
    <div class='header'>
      <h1 style='margin-bottom:10px;'>FRIOLINK</h1>
      <h1>📡 Escaneo de Redes WiFi</h1>
    </div>
    <div class='progress'>
      <div class='progress-step completed'>1. ✅</div>
      <div class='progress-step active'>2. Escaneo</div>
      <div class='progress-step'>3. ⏳</div>
      <div class='progress-step'>4. ⏳</div>
    </div>
    <div>
      <p class="full-width-justified">Seleccioná la red WiFi donde se conectará este disposito y presiona el botón 'Siguiente'.</p>
    </div>
    <div id='message' class='message'></div>
    <button id='btnEscanearRedes' class='btn scan-btn' onclick='scanNetworks()'>🔍 Escanear Redes WiFi</button>
    <div id='networksList' style='display:none;'>
      <h3 id='lblRedesDsiponibles'>Redes disponibles:</h3>
      <div id='redesEncontradas'></div>
    </div>
    <div id='loading' class='loading' style='display:none;'>
      <p>⏳ Escaneando redes WiFi...</p>
    </div>
    <div class='navigation'>
      <button class='btn btn-secondary' onclick='goBack()'>◀️ Anterior</button>
      <button class='btn' id='nextBtn' onclick='goToCredentials()' style='display:none;'>Siguiente ▶️</button>
    </div>
  </div>
  <script>
    let selectedNetwork = null;
    function showMessage(text, type) {
      const messageDiv = document.getElementById('message');
      messageDiv.textContent = text;
      messageDiv.className = 'message ' + type;
      messageDiv.style.display = 'block';
      if (type === 'success') {
        setTimeout(() => { messageDiv.style.display = 'none'; }, 3000);
      }
    }
    function scanNetworks() {
      document.getElementById('btnEscanearRedes').style.display = 'none';
      document.getElementById('lblRedesDsiponibles').style.display = 'none';
      document.getElementById('redesEncontradas').style.display = 'none';
      document.getElementById('loading').style.display = 'block';
      showMessage('Escaneando redes WiFi...', 'success');
      fetch('/scan')
        .then(response => response.json())
        .then(data => {
          let networks = data.networks;
          displayNetworks(networks);
          document.getElementById('loading').style.display = 'none';
          showMessage('¡Escaneo completado! Selecciona una red para continuar.', 'success');
          document.getElementById('btnEscanearRedes').style.display = 'block';
          document.getElementById('lblRedesDsiponibles').style.display = 'block';
          document.getElementById('redesEncontradas').style.display = 'block';
        })
        .catch(error => {
          document.getElementById('loading').style.display = 'none';
          showMessage('Error al escanear redes: ' + error.message, 'error');
          document.getElementById('btnEscanearRedes').style.display = 'block';
        });
    }
    function displayNetworks(networks) {
      const networksDiv = document.getElementById('redesEncontradas');
      const networksList = document.getElementById('networksList');
      networksDiv.innerHTML = '';
      networks.forEach((network, index) => {
        const networkDiv = document.createElement('div');
        networkDiv.className = 'network-item';
        networkDiv.onclick = () => selectNetwork(network, index);
        //const signalIcon = getSignalIcon(network.rssi);
        const signalBarsHtml = getSignalBarsHtml(network.rssi);
        const secureIcon = network.secure ? '🔐' : '🔓';
        networkDiv.innerHTML = `<span><strong>${network.ssid}</strong> ${secureIcon}</span><span class='signal-strength'>${signalBarsHtml}</span>`;
        networksDiv.appendChild(networkDiv);
      });
      networksList.style.display = 'block';
    }
    function getSignalIcon(rssi) {
      if (rssi > -50) return '📦';
      if (rssi > -60) return '📦';
      if (rssi > -70) return '📦';
      return '📦';
    }
    function getSignalBarsHtml(rssi) {
      let numBarsOn = 0;
      let colorClass = '';
    
      // Determinar cuántas barras se encienden y de qué color
      if (rssi >= -50) {        // Excelente
        numBarsOn = 5;
        colorClass = 'color-excelente';
      } else if (rssi >= -60) { // Muy buena
        numBarsOn = 4;
        colorClass = 'color-muy-buena';
      } else if (rssi >= -67) { // Buena / aceptable
        numBarsOn = 3;
        colorClass = 'color-buena';
      } else if (rssi >= -70) { // Regular
        numBarsOn = 2;
        colorClass = 'color-regular';
      } else if (rssi >= -80) { // Mala
        numBarsOn = 1;
        colorClass = 'color-mala';
      } else {                  // Muy mala
        numBarsOn = 1;
        colorClass = 'color-muy-mala';
      }
    
      let html = '<div class="signal-bars">';
      for (let i = 0; i < 5; i++) {
        // La altura de la barra i-ésima (de 5) es (i+1)/5 de la altura total.
        // Usamos clases de altura predefinidas.
        let heightClass = 'bar-height-' + (i + 1);
    
        if (i < numBarsOn) {
          // La barra está "encendida". Necesita clase de color y de altura.
          html += `<div class="bar on ${colorClass} ${heightClass}"></div>`;
        } else {
          // --> CAMBIO IMPORTANTE AQUÍ <--
          // La barra está "apagada". Necesita la clase de altura para ser visible.
          html += `<div class="bar ${heightClass}"></div>`;
        }
      }
      html += '</div>';
    
      return html;
    }
    function selectNetwork(network, index) {
      document.querySelectorAll('.network-item').forEach(item => { item.classList.remove('selected'); });
      document.querySelectorAll('.network-item')[index].classList.add('selected');
      selectedNetwork = network;
      document.getElementById('nextBtn').style.display = 'inline-block';
      fetch('/select-network', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'ssid=' + encodeURIComponent(network.ssid)
      });
    }
    function goBack() { window.location.href = '/intro'; }
    
    function goToCredentials() {
      if (selectedNetwork) {
        window.location.href = '/credentials?ssid=' + encodeURIComponent(selectedNetwork.ssid)+'&rssi=' + encodeURIComponent(selectedNetwork.rssi);
      } else {
        showMessage('Por favor selecciona una red primero', 'error');
      }
    }

    // Escanear redes al cargar la página
    window.addEventListener('load', function() {
      setTimeout(scanNetworks, 500);
    });
  </script>
</body>
</html>
)rawliteral";

void handleScan() {
  DEBUG_PRINTLN("Escaneando redes WiFi...");
  
  String json = "{\"networks\":[";
  int n = WiFi.scanNetworks();
  
  for (int i = 0; i < n; i++) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"secure\":" + String(WiFi.encryptionType(i) != ENC_TYPE_NONE ? "true" : "false");
    json += "}";
  }
  
  json += "]}";
  server.send(200, "application/json", json);
}

// --- Credentials page en PROGMEM, partida para insertar modeloFL y selected_ssid sin usar String ---

const char credPageA[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='es'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>Credenciales - " + String(modeloFL)+")rawliteral";

const char credPageB[] PROGMEM = R"rawliteral(</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; }
    .container { max-width: 500px; margin: 0 auto; background: white; border-radius: 10px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); padding: 30px; }
    .header { text-align: center; margin-bottom: 30px; }
    .header img { max-width: 150px; height: auto; margin-bottom: 10px; }
    .header h1 { color: #333; margin: 0; font-size: 20px; }
    .progress { display: flex; justify-content: space-between; margin-bottom: 30px; font-size: 12px; }
    .progress-step { text-align: center; flex: 1; }
    .progress-step.active { color: #667eea; font-weight: bold; }
    .progress-step.completed { color: #28a745; }
    .form-group { margin-bottom: 20px; }
    label { display: block; margin-bottom: 5px; font-weight: bold; color: #333; }
    input[type='text'], input[type='password'] { width: 100%; padding: 12px; border: 2px solid #ddd; border-radius: 5px; font-size: 16px; box-sizing: border-box; }
    input:focus { outline: none; border-color: #667eea; }
    .btn { background: #667eea; color: white; padding: 12px 24px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; margin: 5px; }
    .btn:hover { background: #5a6fd8; }
    .btn:disabled { background: #ccc; cursor: not-allowed; }
    .btn-secondary { background: #6c757d; }
    .btn-secondary:hover { background: #5a6268; }
    .btn-connect { background: #28a745; width: 100%; margin-top: 10px; }
    .btn-connect:hover { background: #218838; }
    .message { padding: 10px; border-radius: 5px; margin: 10px 0; display: none; }
    .success { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
    .error { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    .navigation { display: flex; justify-content: space-between; margin-top: 30px; }
    .selected-network { background: #e3f2fd; padding: 15px; border-radius: 5px; border-left: 4px solid #667eea; margin-bottom: 20px; }
    .loading { text-align: center; padding: 20px; display: none; }
    .help-text { font-size: 12px; color: #666; margin-top: 5px; }
    .full-width-justified { text-align: justify; width: 100%; max-width: 100%; word-wrap: break-word; hyphens: auto; }
    .password-container { position: relative; display: flex; align-items: center; }
    #password { flex-grow: 1; padding-right: 30px; }
    .toggle-password { position: absolute; right: 10px; cursor: pointer; user-select: none; }
    .toggle-password-contra { position: absolute; right: 10px; cursor: pointer; user-select: none; }
    .toggle-password-conf { position: absolute; right: 10px; cursor: pointer; user-select: none; }
    .form-group.big-label {margin-bottom: 20px;}
    .form-group.big-label label {
        display: block;
        font-size: 1.2rem; /* tamaño de texto más grande */
        font-weight: bold;
        margin-bottom: 8px;
        color: #333;
    }
    .form-group.big-label select {
      width: 100%;
      padding: 12px; /* más espacio interno */
      font-size: 1.1rem; /* texto más grande */
      border: 2px solid #666;
      border-radius: 6px;
      background-color: #f9f9f9;
      outline: none;
      transition: border-color 0.2s;
    }
    .form-group.big-label select:focus {
      border-color: #007BFF; /* color de foco */
    }
    .warning-message {
      background-color: #fff3cd; /* Color de fondo amarillo pálido */
      color: #856404; /* Color de texto amarillo oscuro */
      border: 1px solid #ffeaa7;
      border-radius: 4px;
      padding: 10px;
      margin-bottom: 20px; /* Espacio debajo del mensaje */
      font-size: 14px;
    }
  </style>
</head>
<body>
  <div class='container'>
    <div class='header'>
      <h1 style='margin-bottom:10px;'>FRIOLINK</h1>
      <h1>📡 Datos del )rawliteral";

const char credPageB1[] PROGMEM = R"rawliteral(
      </h1>
    </div>
    <div class='progress'>
      <div class='progress-step completed'>1. ✅</div>
      <div class='progress-step completed'>2. ✅</div>
      <div class='progress-step active'>3. Conf.</div>
      <div class='progress-step'>4. ⏳</div>
    </div>
          
    <div class='selected-network'>
      <strong>🌐 Red seleccionada:</strong> )rawliteral";

const char credPageC[] PROGMEM = R"rawliteral(
    </div>

    <div>
      <p class="full-width-justified">Indicá la contraseña del WiFi, completa los datos y presiona el botón 'Guardar'.</p>
    </div>
          
    <div id='message' class='message'></div>

    <div id='dixell-status-container' style='text-align: center; margin-bottom: 20px;'>
      <p>🔍 Verificando comunicación con el controlador de frío...</p>
    </div>
    
    <form id='credentialsForm'>
      <div class='form-group' id='formPass'>
        <label for='password'>🔑 Contraseña de la red WiFi:</label>
        <div class='password-container'>
          <input type='password' id='password' name='password' placeholder='Ingresa la contraseña de la red WiFi' required>
          <span class='toggle-password' onclick='togglePassword()'>👁️</span>
        </div>
      </div>

      <div class="form-group big-label" id="formOrgType">
        <label for="orgType">🏷️ Tipo de Organización:</label>
        <select id="orgType" name="orgType" onchange="toggleOrgFields()">
          <option value="nueva" selected>Nueva</option>
          <option value="existente">Existente</option>
        </select>
      </div>
      
      <div class='form-group' id='formOrgName'>
        <label for='orgName'>🏷️ Nombre de tu Organización:</label>
        <input type='text' id='orgName' name='orgName' placeholder='Nombre de tu organizacion' required>
      </div>

      <div class='form-group' id='formMailActual'>
        <label for='mailActual'>🏷️ Tu e-Mail:</label>
        <input type='text' id='mailActual' name='mailActual' placeholder='e-Mail' required>
      </div>

      <div class='form-group' id='formTelActual'>
        <label for='telActual'>🏷️ Nº de teléfono:</label>
        <input type='text' id='telActual' name='telActual' placeholder='Tel. Celular' required>
      </div>

      <div class='form-group' id='formUsuActual'>
        <label for='usuActual'>🏷️ Usuario:<span style='color: #074cba;'>  Se te va a pedir para ingresar a la App y la Web</span></label>
        <input type='text' id='usuActual' name='usuActual' placeholder='Usuario de conexión' required>
      </div>

      <div class='form-group' id='formPassContra'>
        <label for='passwordContra'>🔑 Contraseña:<span style='color: #074cba;'>  Tiene que tener 9 caracteres y al menos un simbolo @#$%!^&*</span></label>
        <div class='password-container'>
          <input type='password' id='passwordContra' name='passwordContra' placeholder='Ingresa la contraseña' required>
          <span class='toggle-password-contra' onclick='togglePassAct()'>👁️</span>
        </div>
      </div>

      <div class='form-group' id='formPassConf'>
        <label for='passConf'>🔑 Confirmar contraseña:</label>
        <div class='password-container'>
          <input type='password' id='passConf' name='passConf' placeholder='Ingresa la conf. contraseña' required>
          <span class='toggle-password-conf' onclick='togglePassConf()'>👁️</span>
        </div>
      </div>

      <label>💡 Sugerencia:<span style='color: #074cba;'>   Hace una captura de pantalla para recordar esta información.</span></label>
      <!-- <button type='submit' class='btn btn-connect' id='connectBtn'>🔗 Guardar</button> -->

      <button type='submit' class='btn btn-connect' id='connectBtn' disabled>
        ⏳ Verificando hardware...
      </button>
    </form>
          
    <div id='loading' class='loading'>
      <p>⏳ Probando conexión WiFi...</p>
      <p>Por favor espera...</p>
    </div>
          
    <div class='navigation'>
      <button class='btn btn-secondary' onclick='goBack()'>◀️ Anterior</button>
    </div>
  </div>
  )rawliteral";

  const char credPageScripts[] PROGMEM = R"rawliteral(<script>
    function toggleOrgFields() {
      const type = document.getElementById('orgType').value;
      const mailFields = document.getElementById('formMailActual');
      mailFields.style.display = (type === 'nueva') ? 'block' : 'none';
    
      // Habilitar o deshabilitar required
      mailFields.querySelectorAll('input').forEach(input => {
        input.required = (type === 'nueva');
      });

      const telFields = document.getElementById('formTelActual');
      telFields.style.display = (type === 'nueva') ? 'block' : 'none';
    
      // Habilitar o deshabilitar required
      telFields.querySelectorAll('input').forEach(input => {
        input.required = (type === 'nueva');
      });

      const passFields = document.getElementById('formPassContra');
      passFields.style.display = (type === 'nueva') ? 'block' : 'none';
    
      // Habilitar o deshabilitar required
      passFields.querySelectorAll('input').forEach(input => {
        input.required = (type === 'nueva');
      });

      const passConfFields = document.getElementById('formPassConf');
      passConfFields.style.display = (type === 'nueva') ? 'block' : 'none';
    
      // Habilitar o deshabilitar required
      passConfFields.querySelectorAll('input').forEach(input => {
        input.required = (type === 'nueva');
      });
    }
    
    function showMessage(text, type) {
      const messageDiv = document.getElementById('message');
      messageDiv.textContent = text;
      messageDiv.className = 'message ' + type;
      messageDiv.style.display = 'block';
      if (type === 'success') setTimeout(() => { messageDiv.style.display = 'none'; }, 3000);
    }
    
    document.getElementById('credentialsForm').addEventListener('submit', function(e) {
      e.preventDefault();
      const password = document.getElementById('password').value;
      const orgName = document.getElementById('orgName').value;
      const mailActual = document.getElementById('mailActual').value;
      const telActual = document.getElementById('telActual').value;
      const usuActual = document.getElementById('usuActual').value;
      const passwordContra = document.getElementById('passwordContra').value;
      const passConf = document.getElementById('passConf').value;
      const orgType = document.getElementById('orgType').value;
      
      mostrarTxt(true);
      document.getElementById('loading').style.display = 'block';
      document.getElementById('connectBtn').disabled = true;
      document.getElementById('connectBtn').textContent = '⏳ Conectando...';

      const orgCompleta='_S'+orgType+'_F'+orgName;
      
      const formData = new FormData();
      formData.append('password', password);
      formData.append('orgName', orgCompleta);
      formData.append('mailActual', mailActual);
      formData.append('telActual', telActual);
      formData.append('usuActual', usuActual);
      formData.append('passwordContra', passwordContra);
      formData.append('passConf', passConf);

      fetch('/save', { method: 'POST', body: formData })
        .then(r => r.json())
        .then(data => {
          if (data.success) {
            if (data.redirect) window.location.href = data.redirect;
            else { showMessage('¡Éxito! ' + data.message, 'success'); mostrarTxt(false); }
          } else { showMessage('Error: ' + data.message, 'error'); mostrarTxt(false); }
        })
        .catch(error => { showMessage('Error de conexión al almacenar: ' + error.message, 'error'); mostrarTxt(false); });
    });

    function mostrarTxt(ocultar){
      if (ocultar){
        document.getElementById('message').style.display = 'none';
        document.getElementById('formPass').style.display = 'none';
        document.getElementById('formMailActual').style.display = 'none';
        document.getElementById('formTelActual').style.display = 'none';
        document.getElementById('formUsuActual').style.display = 'none';
        document.getElementById('formPassContra').style.display = 'none';
        document.getElementById('formPassConf').style.display = 'none';
      } else {
        document.getElementById('loading').style.display = 'none';
        document.getElementById('connectBtn').disabled = false;
        document.getElementById('connectBtn').textContent = '🔗 Guardar';
        document.getElementById('formPass').style.display = 'block';
        document.getElementById('formMailActual').style.display = 'block';
        document.getElementById('formTelActual').style.display = 'block';
        document.getElementById('formUsuActual').style.display = 'block';
        document.getElementById('formPassContra').style.display = 'block';
        document.getElementById('formPassConf').style.display = 'block';
        document.getElementById('message').style.display = 'block';
      }
    }
          
    function togglePassword() {
      const passwordField = document.getElementById('password');
      const toggleIcon = document.querySelector('.toggle-password');
      if (passwordField.type === 'password') { passwordField.type = 'text'; toggleIcon.textContent = '🙈'; }
      else { passwordField.type = 'password'; toggleIcon.textContent = '👁️'; }
    }

    function togglePassAct() {
      const passwordContraField = document.getElementById('passwordContra');
      const toggleIconContra = document.querySelector('.toggle-password-contra');
      
      if (passwordContraField.type === 'password') {
        passwordContraField.type = 'text';
        toggleIconContra.textContent = '🙈';
      } else {
        passwordContraField.type = 'password';
        toggleIconContra.textContent = '👁️';
      }
    }

    function togglePassConf() {
      const passwordField = document.getElementById('passConf');
      const toggleIcon = document.querySelector('.toggle-password-conf');
      
      if (passwordField.type === 'password') {
        passwordField.type = 'text';
        toggleIcon.textContent = '🙈';
      } else {
        passwordField.type = 'password';
        toggleIcon.textContent = '👁️';
      }
    }
      
    function goBack() { window.location.href = '/scan-page'; }

    let dixellCheckInterval;

    function checkDixellStatus() {
      fetch('/statusDixell.json')
        .then(response => response.json())
        .then(data => {
          console.log("Estado controlador:", data);
          if (data.dixell_checked) {
            // La verificación ha terminado, limpiamos el intervalo
            clearInterval(dixellCheckInterval);

            const statusContainer = document.getElementById('dixell-status-container');
            const connectBtn = document.getElementById('connectBtn');

            if (data.dixell_status === 'CORRECTO') {
              // Éxito: ocultamos el mensaje y habilitamos el botón
              statusContainer.style.display = 'none';
              connectBtn.disabled = false;
              connectBtn.textContent = '🔗 Guardar';
            } else {
              // Error: mostramos el mensaje y quitamos el botón
              statusContainer.innerHTML = `
                <div class='message error' style='display: block; text-align: left;'>
                  <strong>❌ Error de Hardware:</strong> No se pudo comunicar con el controlador de frío. 
                  Por favor, revise las conexiones y reinicie el dispositivo.
                </div>
              `;
              connectBtn.disabled = false;
              connectBtn.textContent = '🔗 Guardar';
              //connectBtn.style.display = 'none'; // Ocultamos el botón por completo
            }
          }
        })
        .catch(error => {
          console.error('Error al verificar estado del controlador:', error);
          // No hacemos nada más, el intervalo volverá a intentarlo
        });
    }

    window.addEventListener('load', function() {
      // Empezamos a verificar cada 2 segundos
      dixellCheckInterval = setInterval(checkDixellStatus, 1000);
      // También hacemos una verificación inmediata
      checkDixellStatus(); 
    });
  </script>
</body>
</html>
)rawliteral";

bool connectToWiFiConfiguracion(String ssid, String password, int timeout = 15000) {
  WiFi.mode(WIFI_AP_STA);  // ⚠️ IMPORTANTE: Mantenemos el AP activo
  WiFi.begin(ssid.c_str(), password.c_str());

  Serial.print("Conectando a WiFi: ");
  DEBUG_PRINTLN(ssid);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    DEBUG_PRINTLN("\n✅ Conectado a WiFi!");
    Serial.print("IP asignada: ");
    DEBUG_PRINTLN(WiFi.localIP());
    return true;
  } else {
    DEBUG_PRINTLN("\n❌ No se pudo conectar al WiFi");
    return false;
  }
}

// Prueba TCP “viva” al broker (rápida y sin bloquear en exceso)
bool canReachMqtt(const char* host, int port, uint16_t timeout_ms, String* errOut) {
  // Cliente TCP temporal para NO interferir con tu cliente principal
  WiFiClient sock;
  // Si fuera TLS: WiFiClientSecure sock; sock.setInsecure();

  PubSubClient testClient(sock);
  testClient.setServer(host, port);
  // PubSubClient usa segundos:
  uint16_t to_s = (timeout_ms + 999) / 1000;
  if (to_s == 0) to_s = 1;
  testClient.setSocketTimeout(to_s);
  testClient.setKeepAlive(10);     // pequeño keepalive para handshake
  testClient.setBufferSize(512);   // por si el broker manda banners largos

  unsigned long t0 = millis();
  bool ok;
  
  ok = testClient.connect(ssidFL);
  
  unsigned long dt = millis() - t0;

  if (!ok) {
    int rc = testClient.state(); // código negativo/constantes
    if (errOut) {
      *errOut = String("MQTT connect fallo: rc=") + String(rc) + " (" + mqttRcToText(rc) + ")";
    }
    // Asegura cierre del socket
    sock.stop();
    return false;
  }

  // ¡Handshake OK!
  DEBUG_PRINT("MQTT CONNECT OK en "); DEBUG_PRINT(dt); DEBUG_PRINTLN(" ms");
  testClient.disconnect(); // cierra limpio
  sock.stop();
  return true;
}

void handleSave() {
  if (conexDixell != "CORRECTO") {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"No se puede guardar la configuración porque el controlador de frío no responde. Revise las conexiones y reinicie el dispositivo.\"}");
    return;
  }
  String ssid = selected_ssid;
  String password = server.arg("password");
  String orgName = server.arg("orgName");
  String mailActual = server.arg("mailActual");
  String telActual = server.arg("telActual");
  String usuActual = server.arg("usuActual");
  String passwordContra = server.arg("passwordContra");
  String passConf = server.arg("passConf");

  orgName    = collapseSpaces(trimSafe(orgName));
  mailActual = trimSafe(mailActual); mailActual.toLowerCase();
  telActual  = keepDigitsPlus(trimSafe(telActual));
  usuActual  = collapseSpaces(trimSafe(usuActual));
  // Las contraseñas sólo trim (no colapsar espacios internos por si el usuario los quiere usar)
  passwordContra = trimSafe(passwordContra);
  passConf       = trimSafe(passConf);
  
  if (ssid.length() == 0) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"SSID no puede estar vacío\"}");
    return;
  }
    
  // Conexión WiFi sin cortar AP
  DEBUG_PRINTLN("Intentando conectar a WiFi...");
  if (!connectToWiFiConfiguracion(ssid, password)) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"No se pudo conectar a la red WiFi con estas credenciales. Verifica la contraseña.\"}");
    return;
  }

  String errA, errB;
  const char* chosenServer = nullptr;
  int chosenPort = 0;

  DEBUG_PRINTLN("Verificando broker MQTT (puerto por defecto)...");
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
      server.send(500, "application/json",
        "{\"success\":false,"
        "\"message\":\"No se pudo conectar al broker MQTT ni por 38483 ni por 1883. "
        "Verifique conectividad y puertos abiertos.\"}");
      return;
    }
  }
  
  // POST a API externa
  DEBUG_PRINTLN("Conexión WiFi exitosa. Llamando a la API externa...");
  DEBUG_PRINTLN("Conectado a WiFi. IP: ");
  WiFiClient client;
  client.setTimeout(30000); 
  
  HTTPClient https;
  https.setTimeout(30000);
  
  // Construir URL con parámetros en query string
  String urlDispNuevo = String(urlBase) + "login/FLBCrearNuevaCuenta"
             + "?organizacion="   + urlEncode(orgName)
             + "&email="          + urlEncode(mailActual)
             + "&telefono="       + urlEncode(telActual)
             + "&usuario="        + urlEncode(usuActual)
             + "&contrasena="     + urlEncode(passwordContra)
             + "&codDispositivo=" + urlEncode(String(ssidFL));

  DEBUG_PRINT("Se esta tirando el HTTP: ");
  DEBUG_PRINTLN(urlDispNuevo);
  
    
  if (https.begin(client, urlDispNuevo)) {
    int httpCode = https.POST("");  // POST sin body
    if (httpCode <= 0) {
        Serial.printf("POST falló. Código: %d - %s\n", httpCode, https.errorToString(httpCode).c_str());
    }
    String payload = https.getString();
    https.end();
    
    DEBUG_PRINTLN("Respuesta HTTP: " + String(httpCode));
    DEBUG_PRINTLN("Payload: " + payload);
    
    if (httpCode > 0) {
      // Parsear JSON
      StaticJsonDocument<2048> JsonDispNuevo;
      DeserializationError error = deserializeJson(JsonDispNuevo, payload);

      if (error) {
        DEBUG_PRINT(F("ArduinoJson error: "));
        DEBUG_PRINTLN(error.c_str());
        DEBUG_PRINT(F("Payload len="));
        DEBUG_PRINTLN(payload.length());
        DEBUG_PRINTLN(F("Payload inicio >>>"));
        DEBUG_PRINTLN(payload.substring(0, 200)); // primeras ~200 chars
        DEBUG_PRINTLN(F("<<< Payload fin"));
        server.send(500, "application/json", "{\"success\":false,\"message\":\"Error al interpretar la respuesta del servidor\"}");
        return;
      }

      String valorDevuelto = JsonDispNuevo["resultado"]["msgError"]["valorDevuelto"];
      String mensaje = JsonDispNuevo["resultado"]["msgError"]["mensaje"];

      if (valorDevuelto == "01") {
        server.send(400, "application/json", "{\"success\":false,\"message\":\"" + mensaje + "\"}");
        return;
      }
      const char* tokenDispositivoSrv = JsonDispNuevo["resultado"]["msgError"]["idDispositivo"];
      const char* licenciaActualSrv = JsonDispNuevo["resultado"]["msgError"]["licenciaActual"];
      const char* tiempoActualSrv = JsonDispNuevo["resultado"]["msgError"]["tiempoDefecto"];
      const char* idDispositivoSrv = JsonDispNuevo["resultado"]["msgError"]["idDispositivo"];
      
      // Guardar configuración
      strncpy(config.stored_ssid, ssid.c_str(), sizeof(config.stored_ssid) - 1);
      config.stored_ssid[sizeof(config.stored_ssid) - 1] = '\0';
      strncpy(config.stored_password, password.c_str(), sizeof(config.stored_password) - 1);
      config.stored_password[sizeof(config.stored_password) - 1] = '\0';
      strncpy(config.licenciaActual, licenciaActualSrv, sizeof(config.licenciaActual) - 1);
      config.licenciaActual[sizeof(config.licenciaActual) - 1] = '\0';
      strncpy(config.tiempoIntervalo, tiempoActualSrv, sizeof(config.tiempoIntervalo) - 1);
      config.tiempoIntervalo[sizeof(config.tiempoIntervalo) - 1] = '\0';
      strncpy(config.tokenDispositivo, idDispositivoSrv, sizeof(config.tokenDispositivo) - 1);
      config.tokenDispositivo[sizeof(config.tokenDispositivo) - 1] = '\0';
      strncpy(config.mqtt_server, chosenServer, sizeof(config.mqtt_server) - 1);
      config.mqtt_server[sizeof(config.mqtt_server) - 1] = '\0';
      config.mqtt_port = chosenPort;

      //Guradar valores por defecto de direcciones y net
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
            
      DEBUG_PRINTLN("⚠️ Confirmación exitosa: " + mensaje);
      server.send(200, "application/json", "{\"success\":true,\"message\":\"" + mensaje + "\",\"redirect\":\"/result\"}");
      return;
      
    } else {
      server.send(500, "application/json", "{\"success\":false,\"message\":\"Error al contactar el servidor externo\"}");
    }

  } else {
    server.send(500, "application/json", "{\"success\":false,\"message\":\"No se pudo conectar a la API externa\"}");
  }
}

String getWiFiStatusString() {
  switch (WiFi.status()) {
    case WL_CONNECTED: return "Conectado";
    case WL_DISCONNECTED: return "Desconectado";
    case WL_CONNECTION_LOST: return "Conexión perdida";
    case WL_CONNECT_FAILED: return "Fallo en conexión";
    case WL_WRONG_PASSWORD: return "Contraseña incorrecta";
    case WL_NO_SSID_AVAIL: return "SSID no disponible";
    default: return "Desconocido";
  }
}

String getResultPageHTML() {
  bool connection_success = (WiFi.status() == WL_CONNECTED);
  
  String html = R"(
  <!DOCTYPE html>
  <html lang='es'>
  <head>
      <meta charset='UTF-8'>
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>
      <title>Resultado - )" + String(modeloFL) + R"(</title>
      <style>
          body {
              font-family: Arial, sans-serif;
              margin: 0;
              padding: 20px;
              background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
              min-height: 100vh;
          }
          .container {
              max-width: 500px;
              margin: 0 auto;
              background: white;
              border-radius: 10px;
              box-shadow: 0 10px 30px rgba(0,0,0,0.3);
              padding: 30px;
              text-align: center;
          }
          .header {
              margin-bottom: 30px;
          }
          .header img {
              max-width: 150px;
              height: auto;
              margin-bottom: 10px;
          }
          .header h1 {
              color: #333;
              margin: 0;
              font-size: 20px;
          }
          .progress {
              display: flex;
              justify-content: space-between;
              margin-bottom: 30px;
              font-size: 12px;
          }
          .progress-step {
              text-align: center;
              flex: 1;
          }
          .progress-step.completed {
              color: #28a745;
              font-weight: bold;
          }
          .result-icon {
              font-size: 64px;
              margin: 20px 0;
          }
          .result-title {
              font-size: 24px;
              font-weight: bold;
              margin-bottom: 15px;
          }
          .result-success {
              color: #28a745;
          }
          .result-error {
              color: #dc3545;
          }
          .result-details {
              background: #f8f9fa;
              padding: 20px;
              border-radius: 5px;
              margin: 20px 0;
              text-align: left;
          }
          .detail-row {
              display: flex;
              justify-content: space-between;
              margin-bottom: 10px;
              padding-bottom: 10px;
              border-bottom: 1px solid #dee2e6;
          }
          .detail-row:last-child {
              margin-bottom: 0;
              padding-bottom: 0;
              border-bottom: none;
          }
          .btn {
              background: #667eea;
              color: white;
              padding: 15px 30px;
              border: none;
              border-radius: 5px;
              cursor: pointer;
              font-size: 16px;
              margin: 10px;
              text-decoration: none;
              display: inline-block;
          }
          .btn:hover {
              background: #5a6fd8;
          }
          .btn-success {
              background: #28a745;
          }
          .btn-success:hover {
              background: #218838;
          }
          .btn-secondary {
              background: #6c757d;
          }
          .btn-secondary:hover {
              background: #5a6268;
          }
          .countdown {
              font-size: 14px;
              color: #666;
              margin-top: 15px;
          }
      </style>
  </head>
  <body>
      <div class='container'>
          <div class='header'>
              <h1>FRIOLINK</h1>
              <h1>📊 Resultado de la configuración</h1>
          </div>
          
           <div class='progress'>
              <div class='progress-step completed'>1. ✅</div>
              <div class='progress-step completed'>2. ✅</div>
              <div class='progress-step completed'>3. ✅</div>
              <div class='progress-step active'>4. Fin</div>
          </div>
  )";

  DEBUG_PRINTLN("El valor de connection es..."+String(connection_success));
  if (connection_success) {
    html += R"(
        <div class='result-icon'>✅</div>
        <div class='result-title result-success'>¡Conexión Exitosa!</div>
        <p>El dispositivo se ha conectado correctamente a la red WiFi y las credenciales han sido guardadas. Ya podes ingresar por la App de Friolink o la Web de monitoreo</p>
        
        <div class='result-details'>
            <div class='detail-row'>
                <strong>Red WiFi:</strong>
                <span>)" + String(config.stored_ssid) + R"(</span>
            </div>
            <div class='detail-row'>
                <strong>Estado:</strong>
                <span style='color: #28a745;'>Conectado y funcionando</span>
            </div>
        </div>
        
        <p><strong>✨ ¡Configuración completada!</strong></p>
        <p>El dispositivo se reiniciará automáticamente y se conectará a esta red en futuros encendidos.</p>
        
        <div class='countdown' id='countdown'>
            El dispositivo se reiniciará en <span id='timer'>2</span> segundos...
        </div>
        
        <a href='#' class='btn btn-success' onclick='restartNow()' style='display:none;'>
            🔄 Reiniciar Ahora
        </a>
    )";
     DEBUG_PRINTLN("El valor de connection es..."+String(html));
  } else {
    html += R"(
        <div class='result-icon'>❌</div>
        <div class='result-title result-error'>Error de Conexión</div>
        <p>No se pudo establecer la conexión con la red WiFi seleccionada.</p>
        
        <div class='result-details'>
            <div class='detail-row'>
                <strong>Red intentada:</strong>
                <span>)" + selected_ssid + R"(</span>
            </div>
            <div class='detail-row'>
                <strong>Estado:</strong>
                <span style='color: #dc3545;'>)" + getWiFiStatusString() + R"(</span>
            </div>
        </div>
        
        <p><strong>Posibles causas:</strong></p>
        <ul style='text-align: left; max-width: 300px; margin: 0 auto;'>
            <li>Contraseña incorrecta</li>
            <li>Red fuera de alcance</li>
            <li>Red temporalmente no disponible</li>
            <li>Configuración de seguridad incompatible</li>
        </ul>
        
        <a href='/credentials' class='btn'>
            🔄 Intentar Nuevamente
        </a>
        <a href='/scan-page' class='btn btn-secondary'>
            📡 Escanear Otras Redes
        </a>
    )";
  }

  html += R"(
    </div>

    <script>
        function restartNow() {
            if (confirm('¿Estás seguro de que quieres reiniciar el dispositivo ahora?')) {
                fetch('/restart', {method: 'POST'});
                alert('Dispositivo reiniciándose...');
            }
        }
    )";

    if (connection_success) {
      html += R"(
          // Countdown para reinicio automático
          let timeLeft = 2;
          const timerElement = document.getElementById('timer');
          const countdownElement = document.getElementById('countdown');
          
          const countdown = setInterval(() => {
              timeLeft--;
              timerElement.textContent = timeLeft;
              
              if (timeLeft <= 0) {
                  clearInterval(countdown);
                  countdownElement.innerHTML = 'Reiniciando dispositivo...';
                  setTimeout(() => {
                      fetch('/restart', {method: 'POST'});
                  }, 1000);
              }
          }, 1000);
      )";
    }

    html += R"(
    </script>
    </body>
    </html>
  )";
  
  return html;
}

String getIntroPageHTMLConf() {
  String html = R"(
    <!DOCTYPE html>
    <html lang='es'>
    <head>
        <meta charset='UTF-8'>
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>
        <title>Bienvenido - )" + String(modeloFL) + R"(</title>
        <style>
            body {
                font-family: Arial, sans-serif;
                margin: 0;
                padding: 20px;
                background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                min-height: 100vh;
            }
            .container {
                max-width: 500px;
                margin: 0 auto;
                background: white;
                border-radius: 10px;
                box-shadow: 0 10px 30px rgba(0,0,0,0.3);
                padding: 30px;
                text-align: center;
            }
            .header {
                margin-bottom: 30px;
            }
            .header img {
                max-width: 200px;
                height: auto;
                margin-bottom: 15px;
            }
            .header h1 {
                color: #333;
                margin: 0;
                font-size: 24px;
            }
            .header p {
                color: #666;
                margin: 10px 0 0 0;
            }
            .welcome-content {
                text-align: left;
                margin: 30px 0;
                line-height: 1.6;
            }
            .welcome-content h2 {
                color: #333;
                margin-bottom: 15px;
            }
            .welcome-content p {
                color: #666;
                margin-bottom: 15px;
            }
            .welcome-content ul {
                color: #666;
                margin-left: 20px;
            }
            .welcome-content li {
                margin-bottom: 8px;
            }
            .btn {
                background: #667eea;
                color: white;
                padding: 15px 30px;
                border: none;
                border-radius: 5px;
                cursor: pointer;
                font-size: 16px;
                width: 100%;
                margin-top: 20px;
            }
            .btn:hover {
                background: #5a6fd8;
            }
            .highlight {
                background: #f8f9fa;
                padding: 15px;
                border-radius: 5px;
                border-left: 4px solid #667eea;
                margin: 20px 0;
            }
        </style>
    </head>
    <body>
        <div class='container'>
            <div class='header'>
                <h1>FRIOLINK</h1>
                <p>Asistente de configuración</p>
            </div>
            
            <div class='welcome-content'>
                <h2>¡Bienvenido al Asistente de Configuración de red!</h2>
                
                <p>Como tu dispositivo ya esta configurado, este asistente te guiará paso a paso para conectarlo a una nueva red WiFi de manera fácil y segura.</p>
                
                <div class='highlight'>
                    <strong>¿Qué haremos?</strong>
                    <ul>
                        <li>🔍 <strong>Escanearemos</strong> las redes WiFi disponibles</li>
                        <li>🎯 <strong>Seleccionaremos</strong> tu red preferida</li>
                        <li>🔐 <strong>Configuraremos</strong> las credenciales de acceso</li>
                        <li>✅ <strong>Verificaremos</strong> que la conexión funcione correctamente</li>
                    </ul>
                </div>
                
                <p><strong>Tiempo estimado:</strong> 2-3 minutos</p>
                
                <p>El proceso es completamente seguro y tus credenciales se almacenan de forma cifrada en el dispositivo.</p>
                
                <p><em>¡Comencemos a configurar tu conexión WiFi!</em></p>
            </div>
            
            <button class='btn' onclick='goToScan()'>
                ▶️ Comenzar Configuración
            </button>
        </div>
    
        <script>
            function goToScan() {
                window.location.href = '/scan-page';
            }
        </script>
    </body>
    </html>
    )";
  
  return html;
}

const char scanPageHeaderConf[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='es'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>Escaneo de Redes</title>
  <style>
    body { font-family: Arial; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); margin: 0; padding: 20px; min-height: 100vh; }
    .container { max-width: 500px; margin: 0 auto; background: white; border-radius: 10px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); padding: 30px; }
    .header { text-align: center; margin-bottom: 30px; }
    .header h1 { color: #333; margin: 0; font-size: 20px; }
    .progress { display: flex; justify-content: space-between; margin-bottom: 30px; font-size: 12px; }
    .progress-step { text-align: center; flex: 1; }
    .progress-step.active { color: #667eea; font-weight: bold; }
    .progress-step.completed { color: #28a745; }
    .btn { background: #667eea; color: white; padding: 12px 24px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; margin: 5px; }
    .btn:hover { background: #5a6fd8; }
    .btn-secondary { background: #6c757d; }
    .btn-secondary:hover { background: #5a6268; }
    .scan-btn { background: #28a745; width: 100%; margin-bottom: 20px; }
    .scan-btn:hover { background: #218838; }
    .network-item { padding: 15px; border: 2px solid #ddd; border-radius: 5px; margin: 10px 0; cursor: pointer; display: flex; justify-content: space-between; align-items: center; transition: all 0.3s; }
    .network-item:hover { background: #f8f9fa; border-color: #667eea; }
    .network-item.selected { background: #e3f2fd; border-color: #667eea; box-shadow: 0 0 10px rgba(102, 126, 234, 0.3); }
    .signal-strength { font-size: 12px; color: #666; }
    .message { padding: 10px; border-radius: 5px; margin: 10px 0; display: none; }
    .success { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
    .error { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    .navigation { display: flex; justify-content: space-between; margin-top: 30px; }
    .loading { text-align: center; padding: 20px; }
    .full-width-justified { text-align: justify; width: 100%; max-width: 100%; word-wrap: break-word; hyphens: auto; }
    /* Estilos para las alturas de las barras */
    .bar-height-1 { height: 4px; }  /* 20% de la altura total */
    .bar-height-2 { height: 8px; }  /* 40% */
    .bar-height-3 { height: 12px; } /* 60% */
    .bar-height-4 { height: 16px; } /* 80% */
    .bar-height-5 { height: 20px; } /* 100% */
    
    /* Contenedor de las barras de señal */
    .signal-bars {
      display: inline-flex;
      align-items: flex-end; /* Alinear las barras en la base */
      flex-direction: row-reverse; /* --> CAMBIO CLAVE: Invierte el orden de las barras */
      height: 20px; /* Altura fija del contenedor */
      gap: 2px; /* Espacio entre barras */
      margin-left: 8px;
      vertical-align: middle;
    }
    
    /* Estilo para cada barra individual */
    .bar {
      width: 4px;
      background-color: #9e9e9e; /* --> AÑADE !important AQUÍ */
      border-radius: 1px;
      transition: height 0.3s ease; /* Animación suave */
    }
    .signal-bars .bar.on {
      /* Este es el color por defecto, pero será sobrescrito por las clases de color de abajo */
    }
    /* Clases para los colores de las barras "encendidas" */
    .bar.on.color-excelente { background-color: #00c851; } /* Verde Material Design */
    .bar.on.color-muy-buena { background-color: #00c851; }
    .bar.on.color-buena { background-color: #ffd600; } /* Amarillo Material Design */
    .bar.on.color-regular { background-color: #ff9100; } /* Naranja Material Design */
    .bar.on.color-mala { background-color: #ff5252; } /* Rojo Material Design */
    .bar.on.color-muy-mala { 
      background-color: #d32f2f; /* Rojo fuerte Material Design */
      animation: parpadeo 1s infinite;
    }
    
    /* Animación para la señal muy mala */
    @keyframes parpadeo {
      0% { opacity: 1; }
      50% { opacity: 0.5; }
      100% { opacity: 1; }
    }
  </style>
</head>
<body>
  <div class='container'>
    <div class='header'>
      <h1 style='margin-bottom:10px;'>FRIOLINK</h1>
      <h1>📡 Escaneo de Redes WiFi</h1>
    </div>
    <div class='progress'>
      <div class='progress-step completed'>1. ✅</div>
      <div class='progress-step active'>2. Escaneo</div>
      <div class='progress-step'>3. ⏳</div>
      <div class='progress-step'>4. ⏳</div>
    </div>
    <div>
      <p class="full-width-justified">Seleccioná la red WiFi donde se conectará este disposito y presiona el botón 'Siguiente'.</p>
    </div>
    <div id='message' class='message'></div>
    <button id='btnEscanearRedes' class='btn scan-btn' onclick='scanNetworks()'>🔍 Escanear Redes WiFi</button>
    <div id='networksList' style='display:none;'>
      <h3 id='lblRedesDsiponibles'>Redes disponibles:</h3>
      <div id='redesEncontradas'></div>
    </div>
    <div id='loading' class='loading' style='display:none;'>
      <p>⏳ Escaneando redes WiFi...</p>
    </div>
    <div class='navigation'>
      <button class='btn btn-secondary' onclick='goBack()'>◀️ Anterior</button>
      <button class='btn' id='nextBtn' onclick='goToCredentials()' style='display:none;'>Siguiente ▶️</button>
    </div>
  </div>
  <script>
    let selectedNetwork = null;
    function showMessage(text, type) {
      const messageDiv = document.getElementById('message');
      messageDiv.textContent = text;
      messageDiv.className = 'message ' + type;
      messageDiv.style.display = 'block';
      if (type === 'success') {
        setTimeout(() => { messageDiv.style.display = 'none'; }, 3000);
      }
    }
    function scanNetworks() {
      document.getElementById('btnEscanearRedes').style.display = 'none';
      document.getElementById('lblRedesDsiponibles').style.display = 'none';
      document.getElementById('redesEncontradas').style.display = 'none';
      document.getElementById('loading').style.display = 'block';
      showMessage('Escaneando redes WiFi...', 'success');
      fetch('/scan')
        .then(response => response.json())
        .then(data => {
          let networks = data.networks;
          displayNetworks(networks);
          document.getElementById('loading').style.display = 'none';
          showMessage('¡Escaneo completado! Selecciona una red para continuar.', 'success');
          document.getElementById('btnEscanearRedes').style.display = 'block';
          document.getElementById('lblRedesDsiponibles').style.display = 'block';
          document.getElementById('redesEncontradas').style.display = 'block';
        })
        .catch(error => {
          document.getElementById('loading').style.display = 'none';
          showMessage('Error al escanear redes: ' + error.message, 'error');
          document.getElementById('btnEscanearRedes').style.display = 'block';
        });
    }
    function displayNetworks(networks) {
      const networksDiv = document.getElementById('redesEncontradas');
      const networksList = document.getElementById('networksList');
      networksDiv.innerHTML = '';
      networks.forEach((network, index) => {
        const networkDiv = document.createElement('div');
        networkDiv.className = 'network-item';
        networkDiv.onclick = () => selectNetwork(network, index);
        //const signalIcon = getSignalIcon(network.rssi);
        const signalBarsHtml = getSignalBarsHtml(network.rssi);
        const secureIcon = network.secure ? '🔐' : '🔓';
        //networkDiv.innerHTML = `<span><strong>${network.ssid}</strong> ${secureIcon}</span><span class='signal-strength'>${signalIcon} ${network.rssi} dBm</span>`;
        networkDiv.innerHTML = `<span><strong>${network.ssid}</strong> ${secureIcon}</span><span class='signal-strength'>${signalBarsHtml}</span>`;
        networksDiv.appendChild(networkDiv);
      });
      networksList.style.display = 'block';
    }
    function getSignalIcon(rssi) {
      if (rssi > -50) return '📦';
      if (rssi > -60) return '📦';
      if (rssi > -70) return '📦';
      return '📦';
    }
    function getSignalBarsHtml(rssi) {
      let numBarsOn = 0;
      let colorClass = '';
    
      // Determinar cuántas barras se encienden y de qué color
      if (rssi >= -50) {        // Excelente
        numBarsOn = 5;
        colorClass = 'color-excelente';
      } else if (rssi >= -60) { // Muy buena
        numBarsOn = 4;
        colorClass = 'color-muy-buena';
      } else if (rssi >= -67) { // Buena / aceptable
        numBarsOn = 3;
        colorClass = 'color-buena';
      } else if (rssi >= -70) { // Regular
        numBarsOn = 2;
        colorClass = 'color-regular';
      } else if (rssi >= -80) { // Mala
        numBarsOn = 1;
        colorClass = 'color-mala';
      } else {                  // Muy mala
        numBarsOn = 1;
        colorClass = 'color-muy-mala';
      }
    
      let html = '<div class="signal-bars">';
      for (let i = 0; i < 5; i++) {
        // La altura de la barra i-ésima (de 5) es (i+1)/5 de la altura total.
        // Usamos clases de altura predefinidas.
        let heightClass = 'bar-height-' + (i + 1);
    
        if (i < numBarsOn) {
          // La barra está "encendida". Necesita clase de color y de altura.
          html += `<div class="bar on ${colorClass} ${heightClass}"></div>`;
        } else {
          // --> CAMBIO IMPORTANTE AQUÍ <--
          // La barra está "apagada". Necesita la clase de altura para ser visible.
          html += `<div class="bar ${heightClass}"></div>`;
        }
      }
      html += '</div>';
    
      return html;
    }
    function selectNetwork(network, index) {
      document.querySelectorAll('.network-item').forEach(item => { item.classList.remove('selected'); });
      document.querySelectorAll('.network-item')[index].classList.add('selected');
      selectedNetwork = network;
      document.getElementById('nextBtn').style.display = 'inline-block';
      fetch('/select-network', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'ssid=' + encodeURIComponent(network.ssid)
      });
    }
    function goBack() { window.location.href = '/intro'; }
    
    function goToCredentials() {
      if (selectedNetwork) {
        window.location.href = '/credentials?ssid=' + encodeURIComponent(selectedNetwork.ssid)+'&rssi=' + encodeURIComponent(selectedNetwork.rssi);
      } else {
        showMessage('Por favor selecciona una red primero', 'error');
      }
    }

    // Escanear redes al cargar la página
    window.addEventListener('load', function() {
      setTimeout(scanNetworks, 500);
    });
  </script>
</body>
</html>
)rawliteral";

void handleScanConf() {
  DEBUG_PRINTLN("Escaneando redes WiFi...");
  
  String json = "{\"networks\":[";
  int n = WiFi.scanNetworks();
  
  for (int i = 0; i < n; i++) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"secure\":" + String(WiFi.encryptionType(i) != ENC_TYPE_NONE ? "true" : "false");
    json += "}";
  }
  
  json += "]}";
  server.send(200, "application/json", json);
}

const char credPageCConf[] PROGMEM = R"rawliteral(
    </div>

    <div>
      <p class="full-width-justified">Indicá la contraseña del WiFi, completa los datos y presiona el botón 'Guardar'.</p>
    </div>
          
    <div id='message' class='message'></div>
          
    <form id='credentialsForm'>
      <div class='form-group' id='formPass'>
        <label for='password'>🔑 Contraseña de la red WiFi:</label>
        <div class='password-container'>
          <input type='password' id='password' name='password' placeholder='Ingresa la contraseña de la red WiFi' required>
          <span class='toggle-password' onclick='togglePassword()'>👁️</span>
        </div>
      </div>
      <button type='submit' class='btn btn-connect' id='connectBtn'>🔗 Guardar</button>
    </form>
          
    <div id='loading' class='loading'>
      <p>⏳ Probando conexión WiFi...</p>
      <p>Por favor espera...</p>
    </div>
          
    <div class='navigation'>
      <button class='btn btn-secondary' onclick='goBack()'>◀️ Anterior</button>
    </div>
  </div>
  )rawliteral";

  const char credPageScriptsConf[] PROGMEM = R"rawliteral(<script>
    function showMessage(text, type) {
      const messageDiv = document.getElementById('message');
      messageDiv.textContent = text;
      messageDiv.className = 'message ' + type;
      messageDiv.style.display = 'block';
      if (type === 'success') setTimeout(() => { messageDiv.style.display = 'none'; }, 3000);
    }
    
    document.getElementById('credentialsForm').addEventListener('submit', function(e) {
      e.preventDefault();
      const password = document.getElementById('password').value;
      
      document.getElementById('loading').style.display = 'block';
      document.getElementById('connectBtn').disabled = true;
      document.getElementById('connectBtn').textContent = '⏳ Conectando...';

      const formData = new FormData();
      formData.append('password', password);

      fetch('/save', { method: 'POST', body: formData })
        .then(r => r.json())
        .then(data => {
          if (data.success) {
            if (data.redirect) window.location.href = data.redirect;
            else { showMessage('¡Éxito! ' + data.message, 'success'); mostrarTxt(false); }
          } else { showMessage('Error: ' + data.message, 'error'); mostrarTxt(false); }
        })
        .catch(error => { showMessage('Error de conexión (CConf): ' + error.message, 'error'); });
    });

    function mostrarTxt(ocultar){
      if (ocultar){
        document.getElementById('message').style.display = 'none';
        document.getElementById('formPass').style.display = 'none';
      } else {
        document.getElementById('loading').style.display = 'none';
        document.getElementById('connectBtn').disabled = false;
        document.getElementById('connectBtn').textContent = '🔗 Guardar';
        document.getElementById('formPass').style.display = 'block';
        document.getElementById('message').style.display = 'block';
      }
    }
          
    function togglePassword() {
      const passwordField = document.getElementById('password');
      const toggleIcon = document.querySelector('.toggle-password');
      if (passwordField.type === 'password') { passwordField.type = 'text'; toggleIcon.textContent = '🙈'; }
      else { passwordField.type = 'password'; toggleIcon.textContent = '👁️'; }
    }
      
    function goBack() { window.location.href = '/scan-page'; }
  </script>
</body>
</html>
)rawliteral";

void handleSaveConf() {
  String ssid = selected_ssid;
  String password = server.arg("password");
  
  if (ssid.length() == 0) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"SSID no puede estar vacío\"}");
    return;
  }
    
  // Conexión WiFi sin cortar AP
  DEBUG_PRINTLN("Intentando conectar a WiFi...");
  if (!connectToWiFiConfiguracion(ssid, password)) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"No se pudo conectar a la red WiFi con estas credenciales. Verifica la contraseña.\"}");
    return;
  }

  // POST a API externa
  DEBUG_PRINTLN("Conexión WiFi exitosa....");
  DEBUG_PRINTLN("Conectado a WiFi. IP: ");
  WiFiClient client;
  client.setTimeout(15000); 
  
  strncpy(config.stored_ssid, ssid.c_str(), sizeof(config.stored_ssid) - 1);
  config.stored_ssid[sizeof(config.stored_ssid) - 1] = '\0';
  strncpy(config.stored_password, password.c_str(), sizeof(config.stored_password) - 1);
  config.stored_password[sizeof(config.stored_password) - 1] = '\0';

  saveConfigToEEPROM();
            
  DEBUG_PRINTLN("⚠️ Confirmación exitosa: ");
  server.send(200, "application/json", "{\"success\":true,\"message\":\"WiFi conectada correctamente\",\"redirect\":\"/result\"}");
  return;
}

void handleDixellErrorPage() {
  String html = "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Error de conexión con controlador</title>";
  html += "<style>";
  html += "body{font-family:Arial, sans-serif;background:#f8f8f8;padding:20px;}";
  html += "h1{color:#c00;}ul{background:#fff;padding:15px;border-radius:5px;}";
  html += "p{margin-top:20px;font-size:1.1em;}";
  html += "</style></head><body>";
  html += "<h1>⚠️ Error de conexión con el controlador</h1>";
  html += "<p><strong>Estado de conexión:</strong> " + conexDixell + "</p>";

  if (señalesError.length() > 0) {
    html += "<p><strong>Señales con error:</strong></p><ul style='margin-left:30px; background-color: transparent;'>";
    // Convertir la cadena separada por comas a lista HTML
    int start = 0, end = 0;
    while ((end = señalesError.indexOf(',', start)) != -1) {
      html += "<li style='background-color: transparent;'>" + señalesError.substring(start, end) + "</li>";
      start = end + 1;
    }
    html += "<li style='background-color: transparent;'>" + señalesError.substring(start) + "</li>"; // Último
    html += "</ul>";
  } else {
    html += "<p>No se pudieron leer las señales esperadas.</p>";
  }

  html += "<p>Por favor, revise la conexión física o contacte a soporte técnico al <strong>Whatsapp: 261-752 1393</strong>.</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Añade esta función completa al final del archivo webServerPaginas.txt
String getDiagnosticPageHTML() {
  DixellRawFingerprint fp;
  bool dixellOk = getDixellFingerprintCached(fp);

  int fc = dixellOk ? fp.familyCode : -1;
  int fi = dixellOk ? fp.firmware   : -1;
  
  // Calcular el tiempo desde la última transmisión MQTT
  unsigned long timeSinceLastMqtt = (millis() - lastMqttSend) / 1000;
  String colorProbe1 = (String(config.Probe1) == "Error") ? "red" : "black";
  String colorSetPoint = (String(config.SetPoint) == "Error") ? "red" : "black";
  String colorStatusOn = (String(config.StatusOn) == "Error") ? "red" : "black";
  String colorDefrost = (String(config.Defrost) == "Error") ? "red" : "black";
  String colorPtaAbierta = (String(config.PtaAbierta) == "Error") ? "red" : "black";
  String colorCompressor = (String(config.Compressor) == "Error") ? "red" : "black";
  String colorErrorPb1 = (String(config.ErrorPb1) == "Error") ? "red" : "black";
  String colorErrorPb2 = (String(config.ErrorPb2) == "Error") ? "red" : "black";
  String colorAltoValorPb1 = (String(config.AltoValorPb1) == "Error") ? "red" : "black";
  String colorBajoValorPb1 = (String(config.BajoValorPb1) == "Error") ? "red" : "black";
  
  String html = R"(
<!DOCTYPE html>
<html lang='es'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Panel de Diagnóstico - )" + String(modeloFL) + R"(</title>
    <meta http-equiv="refresh" content="5">
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; background: #f0f2f5; }
        .container { max-width: 600px; margin: auto; background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        h1, h2 { color: #333; text-align: center; }
        table { width: 100%; border-collapse: collapse; margin-top: 20px; }
        th, td { padding: 12px; border: 1px solid #ddd; text-align: left; }
        th { background-color: #667eea; color: white; }
        tr:nth-child(even) { background-color: #f2f2f2; }
        .status-ok { color: #28a745; font-weight: bold; }
        .status-error { color: #dc3545; font-weight: bold; }
        .form-group { margin-bottom: 15px; }
        label { display: block; margin-bottom: 5px; font-weight: bold; }
        input[type='text'], input[type='password'] { width: 100%; padding: 8px; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; }
        input[type='checkbox'] { transform: scale(1.5); margin-right: 10px; }
        .btn { background: #667eea; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; width: 100%; }
        .btn:hover { background: #5a6fd8; }
        .message { padding: 10px; border-radius: 5px; margin: 10px 0; display: none; }
        .success { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
        .error { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    </style>
    <script>
        function toggleStaticIPFields() {
            const checkbox = document.getElementById('useStaticIP');
            const fields = document.getElementById('staticIPFields');
            fields.style.display = checkbox.checked ? 'block' : 'none';
        }
    </script>
</head>
<body>
    <div class='container'>
        <h1>🔧 Panel de diagnóstico</h1>
        <p><strong>Dispositivo: )" + String(ssidFL) + R"(</strong> | SV: )" + String(VERSION) + R"( | HV: )" + String(HVERSION) + R"(</p>
        <p><em>Esta página se actualiza automáticamente cada 5 segundos.</em></p>

        <table>
            <tr><th>Parámetro</th><th>Valor</th></tr>
            <tr><td>Estado del Dispositivo</td><td class='status-ok'>Operación Normal</td></tr>
            <tr><td>Tiempo de Encendido:</td><td>)" + getFormattedUptime() + R"(</td></tr>
            <tr><td>Última transmisión</td><td>Hace )" + String(timeSinceLastMqtt) + R"( segundos</td></tr>
            <tr><td>IP (Cliente WiFi)</td><td>)" + WiFi.localIP().toString() + R"(<a href='/network-config' style='text-decoration:none'>⚙️</a></td></tr>
            <tr><td>IP (AP)</td><td>)" + WiFi.softAPIP().toString() + R"(</td></tr>
            <tr><td>Señal WiFi (RSSI)</td><td>)" + String(WiFi.RSSI()) + R"( dBm</td></tr>
            <tr><td>Familia/Firmware</td><td>)" + String(fc) + R"(/)"+ String(fi) + R"(</td></tr>
            <tr><td colspan="2" style="background-color:#e9ecef; text-align:center; font-weight:bold;">--- Datos de Sensores (Última Lectura) ---</td></tr>
            <tr><td>Probe1 (Dir.: )" + String(config.addr_Probe1)+ R"(, M: )"+String(config.func_Probe1) + R"()</td><td style='color:)" + colorProbe1 + R"('>)" + String(config.Probe1) + R"(</td></tr>
            <tr><td>SetPoint (Dir.: )" + String(config.addr_SetPoint)+ R"(, M: )"+String(config.func_SetPoint) + R"()</td><td style='color:)" + colorSetPoint + R"('>)" + String(config.SetPoint) + R"(</td></tr></td></tr>
            <tr><td>StatusOn (Dir.: )" + String(config.addr_StatusOn)+ R"(, M: )"+String(config.func_StatusOn) + R"()</td><td style='color:)" + colorStatusOn + R"('>)" + String(config.StatusOn) + R"(</td></tr></td></tr>
            <tr><td>Defrost (Dir.: )" + String(config.addr_Defrost)+ R"(, M: )"+String(config.func_Defrost) + R"()</td><td style='color:)" + colorDefrost + R"('>)" + String(config.Defrost) + R"(</td></tr></td></tr>
            <tr><td>PtaAbierta (Dir.: )" + String(config.addr_PtaAbierta) + R"(, M: )"+String(config.func_PtaAbierta) + R"()</td><td style='color:)" + colorPtaAbierta + R"('>)" + String(config.PtaAbierta) + R"(</td></tr></td></tr>
            <tr><td>Compressor (Dir.: )" + String(config.addr_Compressor) + R"(, M: )"+String(config.func_Compressor) + R"()</td><td style='color:)" + colorCompressor + R"('>)" + String(config.Compressor) + R"(</td></tr></td></tr>
            <tr><td>ErrorPb1 (Dir.: )" + String(config.addr_ErrorPb1) + R"(, M: )"+String(config.func_ErrorPb1) + R"()</td><td style='color:)" + colorErrorPb1 + R"('>)" + String(config.ErrorPb1) + R"(</td></tr></td></tr>
            <tr><td>ErrorPb2 (Dir.: )" + String(config.addr_ErrorPb2) + R"(, M: )"+String(config.func_ErrorPb2)  + R"()</td><td style='color:)" + colorErrorPb2 + R"('>)" + String(config.ErrorPb2) + R"(</td></tr></td></tr>
            <tr><td>AltoValorPb1 (Dir.: )" + String(config.addr_AltoValorPb1) + R"(, M: )"+String(config.func_AltoValorPb1) + R"()</td><td style='color:)" + colorAltoValorPb1 + R"('>)" + String(config.AltoValorPb1) + R"(</td></tr></td></tr>
            <tr><td>BajoValorPb1 (Dir.: )" + String(config.addr_BajoValorPb1) + R"(, M: )"+String(config.func_BajoValorPb1)  + R"()</td><td style='color:)" + colorErrorPb2 + R"('>)" + String(config.BajoValorPb1) + R"(</td></tr></td></tr>
            <tr><td>w_SetPoint (Dir.: )" + String(config.addr_w_SetPoint) + R"(, M: )"+String(config.func_w_SetPoint) + R"()</td><td style='color:red'>N/A</td></tr></td></tr>
            <tr><td>w_Status (Dir.: )" + String(config.addr_w_Status) + R"(, M: )"+String(config.func_w_Status) + R"()</td><td style='color:red'>N/A</td></tr></td></tr>
        </table>

        <!-- <a href='/network-config' class='btn'>⚙️ Configuración de Red</a>
        <a href='/' class='btn btn-secondary'>🔙 Volver a Configuración</a> -->
</body>
</html>
  )";
  return html;
}

void handleSaveDiagSettings() {
  DEBUG_PRINTLN("Recibida solicitud para guardar configuración del AP de diagnóstico.");

  String newPass = server.arg("newPassDiag");

  // Validación simple
  if (newPass.length() < 8) {
    server.send(400, "text/html", "<h1>Error</h1><p>La contraseña debe tener al menos 8 caracteres.</p><a href='/'>Volver</a>");
    return;
  }

  // Guardar en la estructura de configuración
  strncpy(config.passDiag, newPass.c_str(), sizeof(config.passDiag) - 1);
  config.passDiag[sizeof(config.passDiag) - 1] = '\0';

  // Guardar en EEPROM
  saveConfigToEEPROM();

  // Responder al usuario
  server.send(200, "text/html", "<h1>Contraseña del AP Actualizada</h1><p>El dispositivo se reiniciará para aplicar los cambios. Por favor, espere...</p>");

  DEBUG_PRINTLN("Contraseña del AP de diagnóstico guardada. Reiniciando dispositivo en 2 segundos...");
  delay(2000);
  ESP.restart();
}

const char resultPageHeaderConfFin[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='es'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Resultado - )" + String(modeloFL) + R"(</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; }
        .container { max-width: 500px; margin: 0 auto; background: white; border-radius: 10px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); padding: 30px; text-align: center; }
        .header { margin-bottom: 30px; }
        .header h1 { color: #333; margin: 0; font-size: 20px; }
        .progress { display: flex; justify-content: space-between; margin-bottom: 30px; font-size: 12px; }
        .progress-step { text-align: center; flex: 1; }
        .progress-step.completed { color: #28a745; font-weight: bold; }
        .progress-step.active { color: #667eea; font-weight: bold; }
        .result-icon { font-size: 64px; margin: 20px 0; }
        .result-title { font-size: 24px; font-weight: bold; margin-bottom: 15px; }
        .result-success { color: #28a745; }
        .result-error { color: #dc3545; }
        .result-details { background: #f8f9fa; padding: 20px; border-radius: 5px; margin: 20px 0; text-align: left; }
        .detail-row { display: flex; justify-content: space-between; margin-bottom: 10px; padding-bottom: 10px; border-bottom: 1px solid #dee2e6; }
        .detail-row:last-child { margin-bottom: 0; padding-bottom: 0; border-bottom: none; }
        .btn { background: #667eea; color: white; padding: 15px 30px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; margin: 10px; text-decoration: none; display: inline-block; }
        .btn:hover { background: #5a6fd8; }
        .btn-success { background: #28a745; }
        .btn-success:hover { background: #218838; }
        .btn-secondary { background: #6c757d; }
        .btn-secondary:hover { background: #5a6268; }
        .countdown { font-size: 14px; color: #666; margin-top: 15px; }
    </style>
</head>
<body>
    <div class='container'>
        <div class='header'>
            <h1>FRIOLINK</h1>
            <h1>📊 Resultado de la configuración</h1>
        </div>
        <div class='progress'>
            <div class='progress-step completed'>1. ✅</div>
            <div class='progress-step completed'>2. ✅</div>
            <div class='progress-step completed'>3. ✅</div>
            <div class='progress-step active'>4. Fin</div>
        </div>
)rawliteral";

const char resultPageSuccessConfFin[] PROGMEM = R"rawliteral(
        <div class='result-icon'>✅</div>
        <div class='result-title result-success'>¡Conexión Exitosa!</div>
        <p>El dispositivo se ha conectado correctamente a la red WiFi y las credenciales han sido guardadas. Ya podes ingresar por la App de Friolink o la Web de monitoreo</p>
        <div class='result-details'>
            <div class='detail-row'>
                <strong>Red WiFi:</strong>
                <span>)rawliteral";

const char resultPageSuccessMiddleConfFin[] PROGMEM = R"rawliteral(</span>
            </div>
            <div class='detail-row'>
                <strong>Estado:</strong>
                <span style='color: #28a745;'>Conectado y funcionando</span>
            </div>
        </div>
        <p><strong>✨ ¡Configuración completada!</strong></p>
        <p>El dispositivo se reiniciará automáticamente y se conectará a esta red en futuros encendidos.</p>
        <div class='countdown' id='countdown'>
            El dispositivo se reiniciará en <span id='timer'>2</span> segundos...
        </div>
        <a href='#' class='btn btn-success' onclick='restartNow()' style='display:none;'>
            🔄 Reiniciar Ahora
        </a>
)rawliteral";

const char resultPageErrorConfFin[] PROGMEM = R"rawliteral(
        <div class='result-icon'>❌</div>
        <div class='result-title result-error'>Error de Conexión</div>
        <p>No se pudo establecer la conexión con la red WiFi seleccionada.</p>
        <div class='result-details'>
            <div class='detail-row'>
                <strong>Red intentada:</strong>
                <span>)rawliteral";

const char resultPageErrorMiddleConfFin[] PROGMEM = R"rawliteral(</span>
            </div>
            <div class='detail-row'>
                <strong>Estado:</strong>
                <span style='color: #dc3545;'>)rawliteral";

const char resultPageErrorEndConfFin[] PROGMEM = R"rawliteral(</span>
            </div>
        </div>
        <p><strong>Posibles causas:</strong></p>
        <ul style='text-align: left; max-width: 300px; margin: 0 auto;'>
            <li>Contraseña incorrecta</li>
            <li>Red fuera de alcance</li>
            <li>Red temporalmente no disponible</li>
            <li>Configuración de seguridad incompatible</li>
        </ul>
        <a href='/credentials' class='btn'>
            🔄 Intentar Nuevamente
        </a>
        <a href='/scan-page' class='btn btn-secondary'>
            📡 Escanear Otras Redes
        </a>
)rawliteral";

const char resultPageScriptConfFin[] PROGMEM = R"rawliteral(
    </div>
    <script>
        function restartNow() {
            if (confirm('¿Estás seguro de que quieres reiniciar el dispositivo ahora?')) {
                fetch('/restart', {method: 'POST'});
                alert('Dispositivo reiniciándose...');
            }
        }
)rawliteral";

const char resultPageCountdownConfFin[] PROGMEM = R"rawliteral(
        let timeLeft = 2;
        const timerElement = document.getElementById('timer');
        const countdownElement = document.getElementById('countdown');
        const countdown = setInterval(() => {
            timeLeft--;
            timerElement.textContent = timeLeft;
            if (timeLeft <= 0) {
                clearInterval(countdown);
                countdownElement.innerHTML = 'Reiniciando dispositivo...';
                setTimeout(() => { fetch('/restart', {method: 'POST'}); }, 1000);
            }
        }, 1000);
    </script>
</body>
</html>
)rawliteral";

// Fragmentos HTML para la página de configuración de red
const char networkConfigPageHeader[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='es'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Configuración de Red - )rawliteral";

const char networkConfigPageTitle[] PROGMEM = R"rawliteral(</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; background: #f0f2f5; }
        .container { max-width: 600px; margin: auto; background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        h1, h2 { color: #333; text-align: center; }
        .section { margin-bottom: 25px; padding: 15px; border: 1px solid #ddd; border-radius: 5px; }
        .form-group { margin-bottom: 15px; }
        label { display: block; margin-bottom: 5px; font-weight: bold; }
        input[type='text'], input[type='password'] { width: 100%; padding: 8px; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; }
        input[type='checkbox'] { transform: scale(1.5); margin-right: 10px; }
        .btn { background: #667eea; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; width: 100%; margin-top: 10px; }
        .btn:hover { background: #5a6fd8; }
        .btn-secondary { background: #6c757d; }
        .btn-secondary:hover { background: #5a6268; }
        .message { padding: 10px; border-radius: 5px; margin: 10px 0; display: none; }
        .success { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
        .error { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    </style>
    <script>
        function toggleStaticIPFields() {
            const checkbox = document.getElementById('useStaticIP');
            const fields = document.getElementById('staticIPFields');
            fields.style.display = checkbox.checked ? 'block' : 'none';
        }
        function saveWifiClientConfig(event) {
            event.preventDefault();
            const form = document.getElementById('wifiClientForm');
            const formData = new FormData(form);
            showMessage('Guardando configuración...', 'info');
            fetch('/save_wifi_client', {
                method: 'POST',
                body: formData
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'error') {
                    showMessage(data.message, 'error');
                } else if (data.status === 'success') {
                    showMessage(data.message + ' El dispositivo se reiniciará...', 'success');
                    setTimeout(() => {
                        window.location.reload();
                    }, 5000);
                }
            })
            .catch(error => {
                console.error('Error:', error);
                showMessage('Error de comunicación con el dispositivo.', 'error');
            });
        }
        function showMessage(text, type) {
            const messageDiv = document.getElementById('message');
            messageDiv.textContent = text;
            messageDiv.className = 'message ' + type;
            messageDiv.style.display = 'block';
        }
    </script>
</head>
<body>
    <div class='container'>
        <h1>⚙️ Configuración de Red</h1>
        <p><strong>Dispositivo: )rawliteral";

const char networkConfigPageDevice[] PROGMEM = R"rawliteral(</strong> | SV: )rawliteral";

const char networkConfigPageVersion[] PROGMEM = R"rawliteral(</p>
        
        <div id='message' class='message'></div>
        
        <div class='section'>
            <h2>📶 Configuración del Cliente WiFi</h2>
            <form id='wifiClientForm' action='javascript:void(0);' onsubmit='saveWifiClientConfig(event)'>
                <div class='form-group'>
                    <label>
                        <input type='checkbox' id='useStaticIP' name='useStaticIP' )rawliteral";
                        
const char networkConfigPageCheckStaticIP[] PROGMEM = R"rawliteral( onchange='toggleStaticIPFields()' )rawliteral";

const char networkConfigPageStaticChecked[] PROGMEM = R"rawliteral(>
                        Usar IP Estática (desactivar para usar DHCP)
                    </label>
                </div>
                <div id='staticIPFields' style='display: )rawliteral";

const char networkConfigPageStaticDisplay[] PROGMEM = R"rawliteral(;'>
                    <div class='form-group'>
                        <label for='staticIP'>Dirección IP Estática:</label>
                        <input type='text' id='staticIP' name='staticIP' value=')rawliteral";

const char networkConfigPageStaticIP[] PROGMEM = R"rawliteral('>
                    </div>
                    <div class='form-group'>
                        <label for='staticGateway'>Puerta de Enlace (Gateway):</label>
                        <input type='text' id='staticGateway' name='staticGateway' value=')rawliteral";

const char networkConfigPageStaticGateway[] PROGMEM = R"rawliteral('>
                    </div>
                    <div class='form-group'>
                        <label for='staticSubnet'>Máscara de Subred:</label>
                        <input type='text' id='staticSubnet' name='staticSubnet' value=')rawliteral";

const char networkConfigPageStaticSubnet[] PROGMEM = R"rawliteral('>
                    </div>
                    <div class='form-group'>
                        <label for='staticDNS'>Servidor DNS:</label>
                        <input type='text' id='staticDNS' name='staticDNS' value=')rawliteral";

const char networkConfigPageStaticDNS[] PROGMEM = R"rawliteral('>
                    </div>
                </div>
                <button type='submit' class='btn'>💾 Guardar Configuración del Cliente WiFi</button>
            </form>
        </div>
        
        <div class='section'>
            <h2>📡 Configuración del Punto de Acceso (AP)</h2>
            <!--<form id='apConfigForm' action='/save_ap_config' method='POST'>-->
                <div class='form-group'>
                    <label for='apSSID'>SSID del AP:
                    <!--<input type='text' id='apSSID' name='apSSID' value='-->
                    </label>
                </div>)rawliteral";

const char networkConfigPageAPSSID[] PROGMEM = R"rawliteral(' readonly>
                    <small><em>El SSID del AP se genera automáticamente y no se puede modificar.</em></small>
                </div>
                <div class='form-group'>
                    <label for='apPassword'>Contraseña del AP:</label>
                    <input type='password' id='apPassword' name='apPassword' value=')rawliteral";

const char networkConfigPageAPPassword[] PROGMEM = R"rawliteral('>
                    <small><em>Deja en blanco para no cambiar la contraseña actual.</em></small>
                </div>
                <div class='form-group'>
                    <label for='apChannel'>Canal del AP:</label>
                    <select id='apChannel' name='apChannel'>)rawliteral";

const char networkConfigPageAPChannelOption[] PROGMEM = R"rawliteral(
                    <option value=')rawliteral";

const char networkConfigPageAPChannelSelected[] PROGMEM = R"rawliteral(' selected>Canal )rawliteral";

const char networkConfigPageAPChannel[] PROGMEM = R"rawliteral(</option>)rawliteral";

const char networkConfigPageAPChannelEnd[] PROGMEM = R"rawliteral(<!--
                    </select>
                </div>
                <button type='submit' class='btn'>💾 Guardar Configuración del AP</button>
            </form>
        </div>-->
        
        <p><em><strong>Nota:</strong> Al guardar cualquier configuración, el dispositivo se reiniciará para aplicar los cambios.</em></p>
        
        <a href='/diagnostic' class='btn btn-secondary'>🔙 Volver a Diagnóstico</a>
    </div>
</body>
</html>)rawliteral";

void handleNetworkConfig() {
  DEBUG_PRINTLN("Sirviendo página de configuración de red..."+String(config.staticIP));
  
  // Limpiar datos corruptos antes de usarlos
  String cleanStaticIP = cleanString(config.staticIP);
  String cleanStaticGateway = cleanString(config.staticGateway);
  String cleanStaticSubnet = cleanString(config.staticSubnet);
  String cleanStaticDNS = cleanString(config.staticDNS);
  String cleanPassDiag = cleanString(config.passDiag);

  String useStaticIPCh="none";
  String chkStaticIPCh="nose";
  if (String(config.useStaticIP)=="1"){
    useStaticIPCh="block";
    chkStaticIPCh="checked";
  }
  // Enviar la página en fragmentos desde PROGMEM
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  
  // Enviar cada fragmento
  server.sendContent_P(networkConfigPageHeader);
  server.sendContent(String(modeloFL));
  server.sendContent_P(networkConfigPageTitle);
  server.sendContent(String(ssidFL));
  server.sendContent_P(networkConfigPageDevice);
  server.sendContent(String(VERSION));
  server.sendContent_P(networkConfigPageVersion);
  server.sendContent(chkStaticIPCh);
  server.sendContent_P(networkConfigPageCheckStaticIP);
  server.sendContent_P(networkConfigPageStaticChecked);
  server.sendContent(useStaticIPCh);
  server.sendContent_P(networkConfigPageStaticDisplay);
  server.sendContent(cleanStaticIP);
  server.sendContent_P(networkConfigPageStaticIP);
  server.sendContent(cleanStaticGateway);
  server.sendContent_P(networkConfigPageStaticGateway);
  server.sendContent(cleanStaticSubnet);
  server.sendContent_P(networkConfigPageStaticSubnet);
  server.sendContent(cleanStaticDNS);
  server.sendContent_P(networkConfigPageStaticDNS);
  server.sendContent(String(ap_ssid));
  //server.sendContent_P(networkConfigPageAPSSID);
  //server.sendContent(cleanPassDiag);
  //server.sendContent_P(networkConfigPageAPPassword);
  //server.sendContent(cleanPassDiag);
  //server.sendContent_P(networkConfigPageAPChannelOption);
  
  // Opciones del canal del AP
  //for (int i = 1; i <= 11; i++) {
  //  server.sendContent(String(i));
  //  server.sendContent_P(networkConfigPageAPChannelSelected);
  //  server.sendContent(String(i));
  //  server.sendContent_P(networkConfigPageAPChannel);
  //}
  
  server.sendContent_P(networkConfigPageAPChannelEnd);
  
  // Finalizar la respuesta
  server.sendContent("");
}
