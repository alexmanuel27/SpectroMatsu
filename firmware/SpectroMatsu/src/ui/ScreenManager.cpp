// src/ui/ScreenManager.cpp
#include "ScreenManager.h"
#include "Screens.h" // For drawing screens
#include <TFT_eSPI.h> // For tft
extern TFT_eSPI tft; // Global display instance
#include "../graphics/GraphDrawer.h" // For loading screen

// Para WiFi
#include <WiFi.h>
#include <WiFiAP.h>

// Define un color semi-transparente para el fondo cuando el menú está abierto
#define MENU_BG_COLOR 0x18E3 // Color gris oscuro en RGB565 (aproximado)

ScreenManager::ScreenManager() 
    : _currentScreen(MAIN_SCREEN), 
      _previousScreen(MAIN_SCREEN), 
      _menuVisible(false), 
      _wifiEnabled(false), 
      _loadingStartTime(0), 
      _lastTouchProcessedTime(0),
      _sampleCount(0),
      // --- Inicializar servidor web ---
      _server(80), // Puerto 80 para HTTP
      _webServerStarted(false)
      // --- Fin inicialización ---
{}

void ScreenManager::init() {
    tft.init();
    tft.setRotation(1); // Landscape mode
    randomSeed(analogRead(0)); // Initialize random generator

    // Inicializar WiFi aquí si es necesario, pero no activarlo aún
    Serial.println("System initialized.");
    Screens::drawMainScreen(_wifiEnabled); // Show main screen on startup
    // if (_menuVisible) {
    //     Screens::drawMenu(_menuVisible, _currentScreen); // Ya no existe
    // }
}

void ScreenManager::update() {
    // Manejar cliente web si el servidor está corriendo
    if (_webServerStarted) {
        _server.handleClient();
    }

    if ((_currentScreen == REFERENCE_SCREEN || _currentScreen == SAMPLE_SCREEN) &&
        millis() - _loadingStartTime >= LOADING_DURATION) {
        if (_currentScreen == REFERENCE_SCREEN) {
            _switchToScreen(MAIN_SCREEN);
        } else if (_currentScreen == SAMPLE_SCREEN) {
            _switchToScreen(GRAPH_SCREEN);
        }
    }
}

void ScreenManager::processTouch(int x, int y) {
    if (_isTouchDebounced()) {
        return;
    }

    if (_menuVisible) {
        _handleMenuTouch(x, y);
    } else {
        switch (_currentScreen) {
            case MAIN_SCREEN:
                _handleMainScreenTouch(x, y);
                break;
            case GRAPH_SCREEN:
                _handleGraphScreenTouch(x, y);
                break;
            case SAVE_SCREEN:
                _handleSaveScreenTouch(x, y);
                break;
            case SETTINGS_SCREEN:
                _handleSettingsScreenTouch(x, y);
                break;
            case SAMPLES_SCREEN:
                _handleSamplesScreenTouch(x, y);
                break;
            case ABOUT_SCREEN:
                _handleAboutScreenTouch(x, y);
                break;
            default:
                break;
        }
    }
    _resetTouchDebounce();
}

bool ScreenManager::_isTouchDebounced() {
    return (millis() - _lastTouchProcessedTime) < TOUCH_DEBOUNCE_DELAY;
}

void ScreenManager::_resetTouchDebounce() {
    _lastTouchProcessedTime = millis();
}

void ScreenManager::_switchToScreen(ScreenState newState) {
    if (newState != _currentScreen) {
        _previousScreen = _currentScreen;
        _currentScreen = newState;
        Serial.println("Cambiando a pantalla: " + String(newState));
        switch (_currentScreen) {
            { case MAIN_SCREEN:
                Screens::drawMainScreen(_wifiEnabled); // Pass wifi state
                break;
            }
            case REFERENCE_SCREEN:
            case SAMPLE_SCREEN:
                _loadingStartTime = millis(); // Start timer
                if (_currentScreen == REFERENCE_SCREEN) {
                    GraphDrawer::drawLoadingScreen("Taking reference...");
                } else {
                    GraphDrawer::drawLoadingScreen("Taking sample...");
                }
                break;
            { case GRAPH_SCREEN:
                Screens::drawGraphScreen();
                break;
            }
            { case SAVE_SCREEN:
                String tempName = generateSampleName();
                Screens::drawSaveScreen(tempName);
                break;
            }
            { case SETTINGS_SCREEN:
                Screens::drawSettingsScreen(_wifiEnabled, getSystemInfo()); // Pass wifi state and system info
                break;
            }
            { case SAMPLES_SCREEN:
                Screens::drawSamplesScreen(_wifiEnabled, _samples, _sampleCount); // Pass wifi state and samples
                break;
            }
            { case ABOUT_SCREEN:
                // Pasamos true para mostrar el QR
                Screens::drawAboutScreen(true); // Nuevo parámetro
                break;
            }
        }
        if (_menuVisible) {
             _menuVisible = false;
        }
        _resetTouchDebounce();
    }
}

void ScreenManager::_toggleMenu() {
    _menuVisible = !_menuVisible;
    if (_menuVisible) {
        delay(100); // Espera 100 milisegundos

        // Al abrir el menú, dibujamos la pantalla actual y luego el menú
        // Para simular superposición, dibujamos un rectángulo oscuro sobre la pantalla actual (AHORA SIEMPRE)
        // Dibujar un rectángulo semi-transparente (oscurece la pantalla actual)
        tft.fillRect(0, 0, tft.width(), tft.height(), MENU_BG_COLOR);
        // Luego dibujamos el menú encima
        // Screens::drawMenu(_menuVisible, _currentScreen); // Ya no existe
        // Dibujamos el menú directamente aquí o en una función auxiliar si se complica.
        // Por ahora, solo dibujamos el fondo oscuro y asumimos que la lógica de dibujo del menú está en _handleMenuTouch o se gestiona de otra manera si se abre/cierra.
        // La lógica de dibujo del menú se moverá a la función _handleMenuTouch o se simplificará.
        // Por ahora, solo aplicamos el oscurecimiento.
        // El contenido del menú se dibuja cuando se llama a _handleMenuTouch si es necesario.
        // Vamos a implementar el dibujo del menú aquí, ya que antes lo hacíamos en Screens.
        // Creamos una función auxiliar para dibujar el menú.
        _drawMenu(_currentScreen);
    } else {
        // Al cerrar el menú, redibujemos completamente la pantalla actual
        // para limpiar el efecto de oscurecimiento y el menú.
        _redrawCurrentScreen();
    }
    _resetTouchDebounce();
}

// --- Nueva función auxiliar para dibujar el menú ---
void ScreenManager::_drawMenu(ScreenState currentScreen) {
    // Calculate menu dimensions
    int numOptions = 4; // HOME, SETTINGS, SAMPLES, ABOUT
    int menuHeight = (MENU_ITEM_HEIGHT + MENU_ITEM_MARGIN) * numOptions - MENU_ITEM_MARGIN;
    int menuX = 0; // Menu appears from the left edge
    int menuY = (tft.height() - menuHeight) / 2; // Vertically centered

    // Draw menu background
    tft.fillRect(menuX, menuY, MENU_WIDTH, menuHeight, TFT_DARKGREY);

    // Draw menu border
    tft.drawRect(menuX, menuY, MENU_WIDTH, menuHeight, TFT_WHITE);

    // Draw menu options
    String options[] = {"HOME", "SETTINGS", "SAMPLES", "ABOUT"};
    int currentOptionIndex = _getScreenMenuIndex(currentScreen); // Usamos la función interna

    for (int i = 0; i < numOptions; i++) {
        int optY = _getMenuOptionY(i); // Usamos la función interna
        // Draw option rectangle
        tft.drawRect(menuX, optY, MENU_WIDTH, MENU_ITEM_HEIGHT, TFT_WHITE);

        // Draw indicator if this option matches the current screen
        if (i == currentOptionIndex) {
            // Draw a small circle/dot on the right side of the option
            int indicatorX = menuX + MENU_WIDTH - MENU_INDICATOR_MARGIN - MENU_INDICATOR_SIZE / 2;
            int indicatorY = optY + MENU_ITEM_HEIGHT / 2;
            tft.fillCircle(indicatorX, indicatorY, MENU_INDICATOR_SIZE / 2, TFT_WHITE); // Filled circle
        }

        // Draw option text
        tft.setTextColor(TFT_WHITE);
        tft.setTextDatum(MC_DATUM); // Middle-center
        tft.drawString(options[i], menuX + MENU_WIDTH / 2, optY + MENU_ITEM_HEIGHT / 2, 2);
    }
}
// --- Fin nueva función ---

// --- Nueva función auxiliar para redibujar la pantalla actual ---
void ScreenManager::_redrawCurrentScreen() {
    switch (_currentScreen) {
        { case MAIN_SCREEN:
            Screens::drawMainScreen(_wifiEnabled); // Pass wifi state
            break;
        }
        { case GRAPH_SCREEN:
            Screens::drawGraphScreen();
            break;
        }
        { case SAVE_SCREEN:
            _switchToScreen(GRAPH_SCREEN);
            break;
        }
        { case SETTINGS_SCREEN:
            Screens::drawSettingsScreen(_wifiEnabled, getSystemInfo()); // Pass wifi state and system info
            break;
        }
        { case SAMPLES_SCREEN:
            Screens::drawSamplesScreen(_wifiEnabled, _samples, _sampleCount); // Pass wifi state and samples
            break;
        }
        { case ABOUT_SCREEN:
            // Pasamos true para mostrar el QR al salir del menú
            Screens::drawAboutScreen(true); // Nuevo parámetro
            break;
        }
        // No redibujar durante loading screens
        default:
            break;
    }
}
// --- Fin nueva función ---

// --- Nueva función auxiliar para obtener índice de menú ---
int ScreenManager::_getScreenMenuIndex(ScreenState state) {
    switch (state) {
        case MAIN_SCREEN: return 0; // "HOME"
        case SETTINGS_SCREEN: return 1; // "SETTINGS"
        case SAMPLES_SCREEN: return 2; // "SAMPLES"
        case ABOUT_SCREEN: return 3; // "ABOUT"
        default: return -1; // Not a menu option
    }
}
// --- Fin nueva función ---

// --- Nueva función auxiliar para obtener posición Y de opción de menú ---
int ScreenManager::_getMenuOptionY(int optionIndex) {
    // Calculates the Y position for a specific menu option
    // Option 0: _menuY + margin
    // Option 1: _menuY + margin + height + margin
    // ...
    int numOptions = 4; // HOME, SETTINGS, SAMPLES, ABOUT
    int menuHeight = (MENU_ITEM_HEIGHT + MENU_ITEM_MARGIN) * numOptions - MENU_ITEM_MARGIN;
    int menuY = (tft.height() - menuHeight) / 2; // Recalcular Y para consistencia
    return menuY + (MENU_ITEM_HEIGHT + MENU_ITEM_MARGIN) * optionIndex;
}
// --- Fin nueva función ---

void ScreenManager::_toggleWiFi() {
    _wifiEnabled = !_wifiEnabled; // Cambia el estado interno
    if (_wifiEnabled) {
        Serial.println("WiFi AP ON");
        WiFi.mode(WIFI_AP);
        if (WiFi.softAP(WIFI_SSID, WIFI_PASS)) {
            Serial.println("WiFi AP started successfully");
            IPAddress myIP = WiFi.softAPIP();
            Serial.print("AP IP address: ");
            Serial.println(myIP);

            // --- Iniciar el servidor web ---
            _startWebServer();
            // --- Fin inicio ---
        } else {
            Serial.println("WiFi AP failed to start");
            _wifiEnabled = false; // Revierte el estado si falla
        }
    } else {
        Serial.println("WiFi AP OFF");

        // --- Detener el servidor web antes de apagar el AP ---
        _stopWebServer();
        // --- Fin detención ---

        WiFi.softAPdisconnect(true); // true para deshabilitar también el modo AP
        WiFi.mode(WIFI_OFF); // Opcional: Deshabilita completamente WiFi
    }

    // Redibuja el botón de WiFi en la pantalla actual para reflejar el cambio
    // Solo en MAIN_SCREEN
    if (_currentScreen == MAIN_SCREEN) {
        // Llamamos directamente a la función de dibujo de la pantalla principal
        // para que actualice el estado del botón WiFi.
        Screens::drawMainScreen(_wifiEnabled); // Pass wifi state
    }
    // Reiniciar debounce al activar/desactivar WiFi
    _resetTouchDebounce();
}

// ... (resto del código en ScreenManager.cpp permanece igual) ...

void ScreenManager::_startWebServer() {
    if (!_webServerStarted) {
        // Definir las rutas que manejará el servidor
        _server.on("/", HTTP_GET, std::bind(&ScreenManager::_handleRoot, this));
        _server.on("/samples", HTTP_GET, std::bind(&ScreenManager::_handleSamples, this)); // Puede redirigir o mostrar lo mismo
        _server.on("/download/", HTTP_GET, std::bind(&ScreenManager::_handleDownload, this)); // Captura /download/id
        _server.on("/download_all", HTTP_GET, std::bind(&ScreenManager::_handleDownloadAll, this)); // Nueva ruta para descarga masiva
        _server.on("/turnoffap", HTTP_POST, std::bind(&ScreenManager::_handleTurnOffAP, this)); // Ruta para apagar AP
        _server.begin(); // Iniciar el servidor
        _webServerStarted = true;
        Serial.println("Web server started on AP IP");
    }
}


// ... (resto del código en ScreenManager.cpp permanece igual) ...

// --- Nueva función para detener el servidor web ---
void ScreenManager::_stopWebServer() {
    if (_webServerStarted) {
        _server.stop(); // Detener el servidor
        _webServerStarted = false;
        Serial.println("Web server stopped");
    }
}
// --- Fin nueva función ---

// --- Nueva función para manejar la descarga masiva ---
void ScreenManager::_handleDownloadAll() {
    if (_sampleCount == 0) {
        _server.send(404, "text/plain", "No samples available to download.");
        return;
    }

    // Crear un archivo ZIP o concatenar CSVs. Para simplicidad, concatenamos en un solo CSV con una hoja por muestra.
    // Podríamos usar una librería ZIP, pero para ahora, un CSV grande con un separador o encabezados por muestra es viable.
    String csvContent = "";

    for (int i = 0; i < _sampleCount; i++) {
        // Agregar encabezado identificativo para cada muestra
        csvContent += "\n# Sample ID: " + _samples[i].id + "\n";
        csvContent += "# Timestamp: " + _samples[i].timestamp + "\n";
        csvContent += "Wavelength (nm),Absorbance\n"; // Encabezados

        // Agregar datos de la muestra
        for (int j = 0; j < _samples[i].numPoints; j++) {
            csvContent += String(_samples[i].spectrum[j].wavelength) + "," +
                          String(_samples[i].spectrum[j].absorbance) + "\n";
        }
        csvContent += "\n"; // Línea en blanco entre muestras
    }

    // Enviar el archivo CSV concatenado como descarga
    _server.sendHeader("Content-Disposition", "attachment; filename=\"all_samples.csv\"");
    _server.send(200, "text/csv", csvContent);
}
// --- Fin nueva función ---

// ... (resto del código en ScreenManager.cpp permanece igual) ...





void ScreenManager::_handleRoot() {
    // Redirigir a la página de muestras para simplificar
    _server.sendHeader("Location", "/samples");
    _server.send(302, "text/plain", "");
}

void ScreenManager::_handleSamples() {
    String html = "<!DOCTYPE html><html lang='en'>";
    html += "<head>";
    html += "<meta charset='utf-8'>";
    html += "<title>SpectroMatsu - Samples</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f4f4f4; }";
    html += "h1 { color: #333; text-align: center; }";
    html += "table { border-collapse: collapse; width: 100%; margin-top: 20px; background-color: white; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }";
    html += "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }";
    html += "th { background-color: #4CAF50; color: white; }";
    html += "tr:nth-child(even) { background-color: #f2f2f2; }";
    html += "a { color: #007bff; text-decoration: none; font-weight: bold; }";
    html += "a:hover { text-decoration: underline; }";
    html += ".button { display: inline-block; padding: 10px 15px; margin: 5px; background-color: #007bff; color: white; text-decoration: none; border-radius: 4px; cursor: pointer; }";
    html += ".button:hover { background-color: #0056b3; }";
    html += ".controls { text-align: center; margin-bottom: 20px; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<h1>SpectroMatsu - Sample List</h1>";

    // Botón de descarga masiva
    html += "<div class='controls'>";
    html += "<a class='button' href='/download_all'>Download All Samples (CSV)</a>";
    html += "</div>";

    html += "<table>";
    html += "<thead><tr><th>ID</th><th>Date</th><th>Action</th></tr></thead>";
    html += "<tbody>";

    if (_sampleCount == 0) {
        html += "<tr><td colspan='3'>No samples saved.</td></tr>";
    } else {
        for (int i = 0; i < _sampleCount; i++) {
            html += "<tr>";
            html += "<td>" + _samples[i].id + "</td>";
            html += "<td>" + _samples[i].timestamp + "</td>";
            html += "<td><a href='/download/" + _samples[i].id + "'>Download CSV</a></td>";
            html += "</tr>";
        }
    }

    html += "</tbody>";
    html += "</table>";

    // Botón para apagar el AP
    html += "<div class='controls'>";
    html += "<form action='/turnoffap' method='POST' style='display:inline;'>";
    html += "<input type='submit' class='button' value='Turn Off WiFi AP' onclick='return confirm(\"Are you sure you want to turn off the WiFi AP?\");'>";
    html += "</form>";
    html += "</div>";

    html += "</body>";
    html += "</html>";
    _server.send(200, "text/html", html);
}

void ScreenManager::_handleDownload() {
    // Obtener el ID de la muestra de la URL
    String requestedId = _server.pathArg(0); // /download/<id> -> arg(0) es <id>
    if (requestedId.length() == 0) {
        _server.send(400, "text/plain", "Bad Request: No sample ID provided.");
        return;
    }

    // Buscar la muestra por ID
    SampleData* targetSample = nullptr;
    for (int i = 0; i < _sampleCount; i++) {
        if (_samples[i].id == requestedId) {
            targetSample = &_samples[i];
            break;
        }
    }

    if (targetSample == nullptr) {
        _server.send(404, "text/plain", "Sample not found.");
        return;
    }

    // Crear el contenido del archivo CSV
    String csvContent = "Wavelength (nm),Absorbance\n"; // Encabezados
    for (int j = 0; j < targetSample->numPoints; j++) {
        csvContent += String(targetSample->spectrum[j].wavelength) + "," +
                      String(targetSample->spectrum[j].absorbance) + "\n";
    }

    // Enviar el archivo CSV como descarga
    _server.sendHeader("Content-Disposition", "attachment; filename=\"" + requestedId + ".csv\"");
    _server.send(200, "text/csv", csvContent);
}

void ScreenManager::_handleTurnOffAP() {
    Serial.println("AP WiFi apagado solicitado desde web.");
    // Para apagarlo, simplemente llamamos a la función que maneja el estado interno
    _wifiEnabled = false; // Marcar como apagado
    _stopWebServer(); // Detener el servidor web
    WiFi.softAPdisconnect(true); // Desconectar AP
    WiFi.mode(WIFI_OFF); // Opcional: Deshabilitar completamente WiFi

    // Confirmar en la web
    String response = "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>AP Off</title>";
    response += "<style>body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }</style>";
    response += "</head><body><h1>WiFi AP Turned Off</h1>";
    response += "<p>You can now disconnect from the WiFi network \"" + String(WIFI_SSID) + "\".</p>";
    response += "<a href=\"#\" onclick=\"history.back(); return false;\">Go Back (Will Fail)</a></body></html>";
    _server.send(200, "text/html", response);

    // Opcional: Actualizar la UI local si es posible (aquí solo lo registramos)
    Serial.println("AP apagado. Actualizando UI local...");
    if (_currentScreen == MAIN_SCREEN) {
        // Llamamos directamente a la función de dibujo de la pantalla principal
        // para que actualice el estado del botón WiFi.
        Screens::drawMainScreen(_wifiEnabled); // Pass wifi state (ahora debería ser false)
    }
    // Actualizar el estado de la variable interna en ScreenManager
    // (esto se haría normalmente en la lógica de UI si se presionara el botón físico)
    // Por ahora, asumimos que la UI local se actualiza solo cuando se interactúa con ella.
}

// ... (resto del código en ScreenManager.cpp permanece igual) ...



String ScreenManager::generateSampleName() {
    return "Sample_" + String(millis());
}

void ScreenManager::_takeAndSaveSample(const String& sampleName) {
    if (_sampleCount >= MAX_SAMPLES) {
        Serial.println("No hay espacio para más muestras. Límite alcanzado.");
        return;
    }

    SampleData& sample = _samples[_sampleCount];
    sample.id = sampleName;
    sample.timestamp = DEFAULT_SAMPLE_DATE;
    sample.numPoints = 0;

    // Generar datos de espectro aleatorios
    for(int wavelength = GRAPH_WAVELENGTH_MIN; wavelength <= GRAPH_WAVELENGTH_MAX; wavelength += 10) {
        if (sample.numPoints >= MAX_SPECTRUM_POINTS) break; // Protección
        float absorbance = (float)random(0, (int)(GRAPH_ABSORBANCE_MAX * 100)) / 100.0;
        sample.spectrum[sample.numPoints].wavelength = static_cast<float>(wavelength);
        sample.spectrum[sample.numPoints].absorbance = absorbance;
        sample.numPoints++;
    }

    _sampleCount++;
    Serial.println("Muestra guardada en RAM: " + sampleName + " (ID: " + sample.id + ")");
}

String ScreenManager::getSystemInfo() {
    String info = "";
    info += "Firmware: v1.0.0\n"; // Versión del firmware
    info += "Device: LILYGO T-Display-S3\n"; // Modelo del dispositivo
    info += "WiFi AP: " + String(_wifiEnabled ? "ON" : "OFF") + "\n"; // Estado del AP
    if (_wifiEnabled) {
        IPAddress myIP = WiFi.softAPIP();
        info += "IP: " + myIP.toString() + "\n"; // Dirección IP del AP
    }
    info += "Samples: " + String(_sampleCount) + "/" + String(MAX_SAMPLES) + "\n"; // Contador de muestras
    // info += "Uptime: " + String(millis() / 1000) + "s\n"; // Tiempo encendido (opcional)
    return info;
}

// --- Manejo de toques ---
bool ScreenManager::_handleMenuButtonTouch(int x, int y) {
    if (isPointInRect(x, y, Screens::getMenuBtnX(), Screens::getMenuBtnY(), Screens::getMenuBtnSize(), Screens::getMenuBtnSize())) {
        Serial.println("Menu button pressed!");
        _toggleMenu();
        return true;
    }
    return false;
}

bool ScreenManager::_handleWifiButtonTouch(int x, int y) {
    if (isPointInRect(x, y, Screens::getWifiBtnX(), Screens::getWifiBtnY(), Screens::getWifiBtnWidth(), Screens::getWifiBtnHeight())) {
        Serial.println("WiFi button pressed!");
        _toggleWiFi();
        return true;
    }
    return false;
}

bool ScreenManager::_handleSaveButtonTouch(int x, int y) {
    if (isPointInRect(x, y, Screens::getSaveBtnX(), Screens::getSaveBtnY(), Screens::getSaveBtnWidth(), Screens::getSaveBtnHeight())) {
        Serial.println("SAVE button pressed!");
        String sampleName = generateSampleName();
        _takeAndSaveSample(sampleName);
        _switchToScreen(SAVE_SCREEN);
        return true;
    }
    return false;
}

void ScreenManager::_handleMainScreenTouch(int x, int y) {
    // Check WiFi button first (exists on this screen)
    if (_handleWifiButtonTouch(x, y)) return; // Exit if WiFi button was pressed
    // Check menu button next (exists on this screen)
    if (_handleMenuButtonTouch(x, y)) return; // Exit if menu button was pressed

    // Check main buttons only if menu is not open
    if (!_menuVisible) {
        if (isPointInRect(x, y, Screens::getBtn1X(), Screens::getBtn1Y(), Screens::getBtnWidth(), Screens::getBtnHeight())) {
            Serial.println("REFER button pressed!");
            _switchToScreen(REFERENCE_SCREEN);
        } else if (isPointInRect(x, y, Screens::getBtn2X(), Screens::getBtn2Y(), Screens::getBtnWidth(), Screens::getBtnHeight())) {
            Serial.println("SAMPLE button pressed!");
            _switchToScreen(SAMPLE_SCREEN);
        }
    }
}

void ScreenManager::_handleGraphScreenTouch(int x, int y) {
    // Check menu button first (exists on this screen)
    if (_handleMenuButtonTouch(x, y)) return; // Exit if menu button was pressed

    // Check SAVE button only if menu is not open
    if (!_menuVisible) {
        if (_handleSaveButtonTouch(x, y)) return; // Exit if SAVE button was pressed
    }
}

void ScreenManager::_handleSaveScreenTouch(int x, int y) {
    // Check menu button first (exists on this screen)
    if (_handleMenuButtonTouch(x, y)) return; // Exit if menu button was pressed
    // Add other touch logic for Save screen if needed, e.g., return to GRAPH_SCREEN
    // For now, touching anywhere (except menu) might return to graph screen
    if (!isPointInRect(x, y, Screens::getMenuBtnX(), Screens::getMenuBtnY(), Screens::getMenuBtnSize(), Screens::getMenuBtnSize())) {
         _switchToScreen(GRAPH_SCREEN); // Return to graph screen after saving
    }
}

void ScreenManager::_handleSettingsScreenTouch(int x, int y) {
    // Check menu button first (exists on this screen)
    if (_handleMenuButtonTouch(x, y)) return; // Exit if menu button was pressed
    // Add other touch logic for Settings screen if needed
}

void ScreenManager::_handleSamplesScreenTouch(int x, int y) {
    // Check menu button first (exists on this screen)
    if (_handleMenuButtonTouch(x, y)) return; // Exit if menu button was pressed
    // Add other touch logic for Samples screen if needed
}

void ScreenManager::_handleAboutScreenTouch(int x, int y) {
    // Check menu button first (exists on this screen)
    if (_handleMenuButtonTouch(x, y)) return; // Exit if menu button was pressed
    // Add other touch logic for About screen if needed
    // Por ejemplo, podrías tocar para alternar entre mostrar el QR y el texto
    // O tocar fuera del QR para salir
    // Por ahora, solo el menú
}

void ScreenManager::_handleMenuTouch(int x, int y) {
    if (!_menuVisible) return; // Only process if menu is visible

    // Calculate menu dimensions (for consistency with _drawMenu)
    int numOptions = 4; // HOME, SETTINGS, SAMPLES, ABOUT
    int menuHeight = (MENU_ITEM_HEIGHT + MENU_ITEM_MARGIN) * numOptions - MENU_ITEM_MARGIN;
    int menuX = 0; // Menu appears from the left edge
    int menuY = (tft.height() - menuHeight) / 2; // Vertically centered
    int menuWidth = MENU_WIDTH;

    if (isPointInRect(x, y, menuX, menuY, menuWidth, menuHeight)) {
        String options[] = {"HOME", "SETTINGS", "SAMPLES", "ABOUT"};
        for (int i = 0; i < 4; i++) {
            // int optY = Screens::getMenuOptionY(i); // Ya no existe
            int optY = _getMenuOptionY(i); // Usamos la función interna
            if (isPointInRect(x, y, menuX, optY, menuWidth, MENU_ITEM_HEIGHT)) {
                Serial.println("Menu option pressed: " + options[i]);
                if (options[i] == "HOME") {
                    _switchToScreen(MAIN_SCREEN);
                } else if (options[i] == "SETTINGS") {
                    _switchToScreen(SETTINGS_SCREEN);
                } else if (options[i] == "SAMPLES") {
                    _switchToScreen(SAMPLES_SCREEN);
                } else if (options[i] == "ABOUT") {
                    _switchToScreen(ABOUT_SCREEN);
                }
                _menuVisible = false;
                _resetTouchDebounce();
                return;
            }
        }
    } else {
        Serial.println("Menu closed by touching outside.");
        _menuVisible = false;
        // Redraw the current screen to clear the menu and background effect
        _redrawCurrentScreen(); // Usamos la función auxiliar
        _resetTouchDebounce();
    }
}

bool ScreenManager::isPointInRect(int16_t px, int16_t py, int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    return (px >= rx && px <= rx + rw && py >= ry && py <= ry + rh);
}