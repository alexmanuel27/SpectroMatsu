// src/ui/Screens.cpp
#include "Screens.h"
#include <TFT_eSPI.h>
extern TFT_eSPI tft; // Global display instance
#include "../graphics/GraphDrawer.h" // For GraphDrawer functions
#include "../utils/Config.h" // Para constantes
// --- Incluir la librería QRCode ---
#include <qrcode.h>
// --- Fin inclusión ---

// --- Definir el tamaño del buffer QR como una macro ---
#define QR_BUFFER_SIZE 3500 // Tamaño estimado para versión 10, ajusta si es necesario

// Static variable definitions
int Screens::_btn1X = 0;
int Screens::_btn1Y = 0;
int Screens::_btn2X = 0;
int Screens::_btn2Y = 0;
int Screens::_saveBtnX = 0;
int Screens::_saveBtnY = 0;
int Screens::_backBtnX = 0;
int Screens::_backBtnY = 0;
int Screens::_wifiBtnX = 0;
int Screens::_wifiBtnY = 0;
// --- Static Menu Button Coordinates ---
int Screens::_menuBtnX = 0;
int Screens::_menuBtnY = 0;
// --- Static Menu Coordinates ---
int Screens::_menuX = 0;
int Screens::_menuY = 0;
int Screens::_menuHeight = 0; // Calculated dynamically

void Screens::drawMainScreen(bool wifiEnabled) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString("Optical Spectrometer", tft.width()/2, 20, 2);

    // Calculate button positions
    int centerX = tft.width() / 2;
    _btn1X = centerX - BTN_WIDTH - 5;  // Left button
    _btn2X = centerX + 5;              // Right button
    _btn1Y = tft.height() / 2 - BTN_HEIGHT / 2;
    _btn2Y = _btn1Y;

    // Draw "REFER" button
    tft.fillRect(_btn1X, _btn1Y, BTN_WIDTH, BTN_HEIGHT, TFT_RED);
    tft.drawRect(_btn1X, _btn1Y, BTN_WIDTH, BTN_HEIGHT, TFT_WHITE);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM); // Middle-center
    tft.drawString("REFER", _btn1X + BTN_WIDTH/2, _btn1Y + BTN_HEIGHT/2, 4);

    // Draw "SAMPLE" button
    tft.fillRect(_btn2X, _btn2Y, BTN_WIDTH, BTN_HEIGHT, TFT_BLUE);
    tft.drawRect(_btn2X, _btn2Y, BTN_WIDTH, BTN_HEIGHT, TFT_WHITE);
    tft.drawString("SAMPLE", _btn2X + BTN_WIDTH/2, _btn2Y + BTN_HEIGHT/2, 4);

    // Draw menu button (3 horizontal lines) - ON ALL SCREENS WITH MENU ACCESS
    _menuBtnX = MENU_BTN_MARGIN;
    _menuBtnY = MENU_BTN_MARGIN;
    tft.fillRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, 4, TFT_WHITE); // Top line
    tft.fillRect(_menuBtnX, _menuBtnY + 8, MENU_BTN_SIZE, 4, TFT_WHITE); // Middle line
    tft.fillRect(_menuBtnX, _menuBtnY + 16, MENU_BTN_SIZE, 4, TFT_WHITE); // Bottom line
    // NO BORDER: tft.drawRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, MENU_BTN_SIZE, TFT_WHITE); // Border REMOVED

    // Draw WiFi button (Top-right corner) - ONLY ON MAIN SCREEN
    _wifiBtnX = tft.width() - WIFI_BTN_WIDTH - WIFI_BTN_MARGIN_X; // Right margin
    _wifiBtnY = WIFI_BTN_MARGIN_Y; // Top margin
    if (wifiEnabled) {
        tft.fillRect(_wifiBtnX, _wifiBtnY, WIFI_BTN_WIDTH, WIFI_BTN_HEIGHT, TFT_GREEN);
        tft.setTextColor(TFT_BLACK);
    } else {
        tft.fillRect(_wifiBtnX, _wifiBtnY, WIFI_BTN_WIDTH, WIFI_BTN_HEIGHT, TFT_DARKGREY);
        tft.setTextColor(TFT_WHITE);
    }
    tft.drawRect(_wifiBtnX, _wifiBtnY, WIFI_BTN_WIDTH, WIFI_BTN_HEIGHT, TFT_WHITE);
    tft.setTextDatum(MC_DATUM); // Middle-center
    tft.drawString("WiFi", _wifiBtnX + WIFI_BTN_WIDTH/2, _wifiBtnY + WIFI_BTN_HEIGHT/2, 2);
}

void Screens::drawReferenceScreen() {
    tft.fillScreen(TFT_BLACK);
    // No menu button on this screen
    GraphDrawer::drawLoadingScreen("Taking reference...");
}

void Screens::drawSampleScreen() {
    tft.fillScreen(TFT_BLACK);
    // No menu button on this screen
    GraphDrawer::drawLoadingScreen("Taking sample...");
}

void Screens::drawGraphScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString("Absorbance Spectrum", tft.width()/2, 10, 2);

    // Dibujar gráfico
    int margin = 35;
    int graphWidth = tft.width() - 2 * margin;
    int graphHeight = tft.height() - 70;
    int graphX = margin;
    int graphY = margin + 15;

    // Dibujar ejes
    tft.drawLine(graphX, graphY + graphHeight, graphX + graphWidth, graphY + graphHeight, TFT_WHITE); // X-axis
    tft.drawLine(graphX, graphY, graphX, graphY + graphHeight, TFT_WHITE); // Y-axis

    // Etiquetas de Ejes
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM); // Middle-center
    tft.drawString("Wavelength (nm)", tft.width()/2, tft.height() - 15, 2);

    tft.setRotation(3);
    tft.drawString("Absorbance", -tft.height()/2, 15, 2);
    tft.setRotation(1); // Return to display rotation (1)

    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString(String(GRAPH_WAVELENGTH_MIN), graphX, graphY + graphHeight + 5, 1);
    tft.drawString(String(GRAPH_WAVELENGTH_MAX), graphX + graphWidth - 10, graphY + graphHeight + 5, 1);

    tft.setTextDatum(5); // Right-middle (RC_DATUM)
    tft.drawString(String(GRAPH_ABSORBANCE_MIN, 1), graphX - 5, graphY + graphHeight, 1);
    tft.drawString(String(GRAPH_ABSORBANCE_MAX, 1), graphX - 5, graphY, 1);

    // Generate and draw random data points
    int lastPlotX = graphX;
    int lastPlotY = graphY + graphHeight;

    for(int wavelength = GRAPH_WAVELENGTH_MIN; wavelength <= GRAPH_WAVELENGTH_MAX; wavelength += 10) {
        float absorbance = (float)random(0, (int)(GRAPH_ABSORBANCE_MAX * 100)) / 100.0;
        int plotX = graphX + map(wavelength, GRAPH_WAVELENGTH_MIN, GRAPH_WAVELENGTH_MAX, 0, graphWidth);
        int plotY = graphY + graphHeight - map(absorbance * 100, (int)(GRAPH_ABSORBANCE_MIN * 100), (int)(GRAPH_ABSORBANCE_MAX * 100), 0, graphHeight);

        if(wavelength > GRAPH_WAVELENGTH_MIN) {
            tft.drawLine(lastPlotX, lastPlotY, plotX, plotY, TFT_GREEN);
        }
        lastPlotX = plotX;
        lastPlotY = plotY;
    }

    // Draw SAVE button in top-right corner (Replaces BACK button)
    _saveBtnX = tft.width() - SAVE_BTN_WIDTH - SAVE_BTN_MARGIN_X; // Right margin
    _saveBtnY = SAVE_BTN_MARGIN_Y; // Top margin
    tft.fillRect(_saveBtnX, _saveBtnY, SAVE_BTN_WIDTH, SAVE_BTN_HEIGHT, TFT_BLUE); // Blue color for SAVE
    tft.drawRect(_saveBtnX, _saveBtnY, SAVE_BTN_WIDTH, SAVE_BTN_HEIGHT, TFT_WHITE);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM); // Middle-center
    tft.drawString("SAVE", _saveBtnX + SAVE_BTN_WIDTH/2, _saveBtnY + SAVE_BTN_HEIGHT/2, 2);

    // Draw menu button (3 horizontal lines) - ON ALL SCREENS WITH MENU ACCESS
    _menuBtnX = MENU_BTN_MARGIN;
    _menuBtnY = MENU_BTN_MARGIN;
    tft.fillRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, 4, TFT_WHITE); // Top line
    tft.fillRect(_menuBtnX, _menuBtnY + 8, MENU_BTN_SIZE, 4, TFT_WHITE); // Middle line
    tft.fillRect(_menuBtnX, _menuBtnY + 16, MENU_BTN_SIZE, 4, TFT_WHITE); // Bottom line
    // NO BORDER: tft.drawRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, MENU_BTN_SIZE, TFT_WHITE); // Border REMOVED
}

void Screens::drawSaveScreen(const String& sampleName) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString("Saving Sample...", tft.width()/2, 20, 2);

    // Mostrar el nombre de la muestra que se está guardando
    tft.setTextDatum(MC_DATUM); // Middle-center
    tft.drawString("Name: " + sampleName, tft.width()/2, tft.height()/2 - 20, 2);

    // Mostrar mensaje de confirmación
    tft.drawString("Sample saved!", tft.width()/2, tft.height()/2 + 20, 2);

    // Draw menu button (3 horizontal lines) - ON ALL SCREENS WITH MENU ACCESS
    _menuBtnX = MENU_BTN_MARGIN;
    _menuBtnY = MENU_BTN_MARGIN;
    tft.fillRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, 4, TFT_WHITE); // Top line
    tft.fillRect(_menuBtnX, _menuBtnY + 8, MENU_BTN_SIZE, 4, TFT_WHITE); // Middle line
    tft.fillRect(_menuBtnX, _menuBtnY + 16, MENU_BTN_SIZE, 4, TFT_WHITE); // Bottom line
    // NO BORDER: tft.drawRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, MENU_BTN_SIZE, TFT_WHITE); // Border REMOVED
}

// --- Modificar drawSettingsScreen para mostrar la info línea por línea ---
void Screens::drawSettingsScreen(bool wifiEnabled, const String& systemInfo) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString("Settings", tft.width()/2, 20, 2);

    // Draw menu button (3 horizontal lines) - ON ALL SCREENS WITH MENU ACCESS
    _menuBtnX = MENU_BTN_MARGIN;
    _menuBtnY = MENU_BTN_MARGIN;
    tft.fillRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, 4, TFT_WHITE); // Top line
    tft.fillRect(_menuBtnX, _menuBtnY + 8, MENU_BTN_SIZE, 4, TFT_WHITE); // Middle line
    tft.fillRect(_menuBtnX, _menuBtnY + 16, MENU_BTN_SIZE, 4, TFT_WHITE); // Bottom line
    // NO BORDER: tft.drawRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, MENU_BTN_SIZE, TFT_WHITE); // Border REMOVED

    // Draw WiFi button (Top-right corner) - ONLY ON MAIN SCREEN, so not here
    // _wifiBtnX = tft.width() - WIFI_BTN_WIDTH - WIFI_BTN_MARGIN_X; // Right margin
    // _wifiBtnY = WIFI_BTN_MARGIN_Y; // Top margin
    // if (wifiEnabled) { ... }
    // tft.fillRect(_wifiBtnX, _wifiBtnY, WIFI_BTN_WIDTH, WIFI_BTN_HEIGHT, ...);
    // tft.drawRect(_wifiBtnX, _wifiBtnY, WIFI_BTN_WIDTH, WIFI_BTN_HEIGHT, TFT_WHITE);
    // tft.setTextDatum(MC_DATUM); // Middle-center
    // tft.drawString("WiFi", _wifiBtnX + WIFI_BTN_WIDTH/2, _wifiBtnY + WIFI_BTN_HEIGHT/2, 2);
    // NO DRAW WIFI BUTTON HERE

    // Mostrar la información del sistema línea por línea
    if (systemInfo.length() > 0) {
        tft.setTextColor(TFT_WHITE);
        tft.setTextDatum(TL_DATUM); // Top-left - Cambiamos a TL para alinear la columna izquierda
        int lineHeight = 20; // Altura estimada de cada línea (ajusta según sea necesario)
        int startY = 60; // Posición Y inicial para la primera línea de texto
        int x_pos = 20; // Margen izquierdo para el texto

        // Dividir la cadena de información en líneas
        int startIndex = 0;
        int endIndex = 0;
        int currentY = startY;

        while (endIndex < systemInfo.length()) {
            endIndex = systemInfo.indexOf('\n', startIndex);
            if (endIndex == -1) {
                // No se encontró más '\n', procesar el resto de la cadena
                String line = systemInfo.substring(startIndex);
                tft.drawString(line, x_pos, currentY, 2); // Fuente tamaño 2
                break; // Salir del bucle
            } else {
                // Encontramos un '\n', procesar la línea actual
                String line = systemInfo.substring(startIndex, endIndex);
                tft.drawString(line, x_pos, currentY, 2); // Fuente tamaño 2
                startIndex = endIndex + 1; // Empezar después del '\n'
                currentY += lineHeight; // Incrementar Y para la próxima línea

                // Opcional: Detener si se llega al final de la pantalla
                if (currentY > tft.height() - 20) break; // Deja un margen inferior
            }
        }
    } else {
        tft.setTextColor(TFT_RED);
        tft.drawString("System info not available.", 20, 60);
    }
}
// --- Fin modificación ---

void Screens::drawSamplesScreen(bool wifiEnabled, SampleData* samples, int sampleCount) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString("Samples List", tft.width()/2, 20, 2);

    // Draw menu button (3 horizontal lines) - ON ALL SCREENS WITH MENU ACCESS
    _menuBtnX = MENU_BTN_MARGIN;
    _menuBtnY = MENU_BTN_MARGIN;
    tft.fillRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, 4, TFT_WHITE); // Top line
    tft.fillRect(_menuBtnX, _menuBtnY + 8, MENU_BTN_SIZE, 4, TFT_WHITE); // Middle line
    tft.fillRect(_menuBtnX, _menuBtnY + 16, MENU_BTN_SIZE, 4, TFT_WHITE); // Bottom line
    // NO BORDER: tft.drawRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, MENU_BTN_SIZE, TFT_WHITE); // Border REMOVED

    // Draw WiFi button (Top-right corner) - ONLY ON MAIN SCREEN, so not here
    // _wifiBtnX = tft.width() - WIFI_BTN_WIDTH - WIFI_BTN_MARGIN_X; // Right margin
    // _wifiBtnY = WIFI_BTN_MARGIN_Y; // Top margin
    // if (wifiEnabled) { ... }
    // tft.fillRect(_wifiBtnX, _wifiBtnY, WIFI_BTN_WIDTH, WIFI_BTN_HEIGHT, ...);
    // tft.drawRect(_wifiBtnX, _wifiBtnY, WIFI_BTN_WIDTH, WIFI_BTN_HEIGHT, TFT_WHITE);
    // tft.setTextDatum(MC_DATUM); // Middle-center
    // tft.drawString("WiFi", _wifiBtnX + WIFI_BTN_WIDTH/2, _wifiBtnY + WIFI_BTN_HEIGHT/2, 2);
    // NO DRAW WIFI BUTTON HERE

    // Listar muestras (simplificado) - Mostrar Nombre y Fecha
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TL_DATUM); // Top-left
    int y_pos = 60;
    if (sampleCount == 0) {
        tft.drawString("No samples saved.", 20, y_pos);
    } else {
        tft.drawString("ID", 20, y_pos);
        tft.drawString("Date", 120, y_pos); // Ajusta la posición X según sea necesario
        y_pos += 20; // Espacio para el encabezado
        for (int i = 0; i < sampleCount && y_pos < tft.height() - 40; i++) {
            tft.drawString(samples[i].id, 20, y_pos); // Usamos ID como nombre
            tft.drawString(DEFAULT_SAMPLE_DATE, 120, y_pos); // Fecha por defecto
            y_pos += 20;
        }
    }
}

// --- Nueva función drawAboutScreen con QR ---
void Screens::drawAboutScreen(bool showQR) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString("About", tft.width()/2, 20, 2);

    // Draw menu button (3 horizontal lines) - ON ALL SCREENS WITH MENU ACCESS
    _menuBtnX = MENU_BTN_MARGIN;
    _menuBtnY = MENU_BTN_MARGIN;
    tft.fillRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, 4, TFT_WHITE); // Top line
    tft.fillRect(_menuBtnX, _menuBtnY + 8, MENU_BTN_SIZE, 4, TFT_WHITE); // Middle line
    tft.fillRect(_menuBtnX, _menuBtnY + 16, MENU_BTN_SIZE, 4, TFT_WHITE); // Bottom line
    // NO BORDER: tft.drawRect(_menuBtnX, _menuBtnY, MENU_BTN_SIZE, MENU_BTN_SIZE, TFT_WHITE); // Border REMOVED

    if (showQR) {
        // --- Generar y dibujar el código QR ---
        // El contenido del QR
        String qrContent = "https://github.com/alexmanuel27/SpectroMatsu"; // URL del repositorio

        // Buffer para el código QR - Ahora usamos la macro definida arriba
        static uint8_t qrBuffer[QR_BUFFER_SIZE]; // Versión 10 debería ser suficiente para URLs cortas
        QRCode qrcode;
        qrcode_initText(&qrcode, qrBuffer, 10, ECC_LOW, qrContent.c_str());

        // Tamaño del pixel del QR
        int qrScale = 2; // Ajusta según sea necesario - REDUCIDO
        int qrSize = qrcode.size * qrScale;
        // Calcular posición Y para centrar verticalmente considerando el título y el texto del enlace
        int titleHeight = 20; // Aproximadamente la altura del texto "About"
        int textHeight = 10;  // Aproximadamente la altura del texto del enlace (tamaño 1)
        int verticalSpacing = 10; // Espacio entre elementos
        int centerY = tft.height() / 2;
        // Colocar el QR para que el centro del área combinada (QR + texto enlace) esté en el centro de la pantalla
        int combinedHeight = qrSize + textHeight + verticalSpacing;
        int qrStartY = centerY - (combinedHeight / 2);
        int qrStartX = (tft.width() - qrSize) / 2; // Centrado horizontalmente

        // Dibujar el QR
        for (int y = 0; y < qrcode.size; y++) {
            for (int x = 0; x < qrcode.size; x++) {
                if (qrcode_getModule(&qrcode, x, y)) {
                    tft.fillRect(qrStartX + x * qrScale, qrStartY + y * qrScale, qrScale, qrScale, TFT_WHITE);
                }
                // No es necesario dibujar celdas blancas sobre el fondo negro
            }
        }

        // Opcional: Mostrar el texto del contenido del QR debajo
        tft.setTextDatum(TC_DATUM); // Top-center
        // Ajustar la Y para que esté justo debajo del QR
        int textY = qrStartY + qrSize + 5; // Espacio pequeño debajo del QR
        tft.drawString(qrContent, tft.width()/2, textY, 1); // Fuente más pequeña (tamaño 1)
    } else {
        // Si no se muestra el QR, mostrar un texto alternativo
        tft.setTextDatum(MC_DATUM); // Middle-center
        tft.drawString("SpectroMatsu v1.0\nby AlexMatsu", tft.width()/2, tft.height()/2);
    }
}
// --- Fin nueva función ---

void Screens::drawLoadingScreen(const String& message) {
    GraphDrawer::drawLoadingScreen(message);
}

// --- WiFi Button Coordinate Getters (Only for MAIN_SCREEN) ---
int Screens::getWifiBtnX() { return _wifiBtnX; }
int Screens::getWifiBtnY() { return _wifiBtnY; }
int Screens::getWifiBtnWidth() { return WIFI_BTN_WIDTH; }
int Screens::getWifiBtnHeight() { return WIFI_BTN_HEIGHT; }

// --- Menu Button Coordinate Getters (valid when menu button is drawn) ---
int Screens::getMenuBtnX() { return _menuBtnX; }
int Screens::getMenuBtnY() { return _menuBtnY; }
int Screens::getMenuBtnSize() { return MENU_BTN_SIZE; }

// --- Menu Coordinate Getters ---
int Screens::getMenuX() { return _menuX; }
int Screens::getMenuY() { return _menuY; }
int Screens::getMenuWidth() { return MENU_WIDTH; }
int Screens::getMenuHeight() { return _menuHeight; }
// int Screens::getMenuOptionY(int optionIndex) { ... } // Ya no es necesaria aquí
// int Screens::getScreenMenuIndex(ScreenState state) { ... } // Ya no es necesaria aquí

// --- Redraw Helper for WiFi Button (Only for MAIN_SCREEN) ---
// void Screens::redrawWifiButton(bool wifiEnabled) { ... } // Ya no es necesaria aquí

// Button coordinate getters (existing)
int Screens::getBtn1X() { return _btn1X; }
int Screens::getBtn1Y() { return _btn1Y; }
int Screens::getBtn2X() { return _btn2X; }
int Screens::getBtn2Y() { return _btn2Y; }
int Screens::getBtnWidth() { return BTN_WIDTH; }
int Screens::getBtnHeight() { return BTN_HEIGHT; }
int Screens::getSaveBtnX() { return _saveBtnX; }
int Screens::getSaveBtnY() { return _saveBtnY; }
int Screens::getSaveBtnWidth() { return SAVE_BTN_WIDTH; }
int Screens::getSaveBtnHeight() { return SAVE_BTN_HEIGHT; }
int Screens::getBackBtnX() { return _backBtnX; }
int Screens::getBackBtnY() { return _backBtnY; }
int Screens::getBackBtnWidth() { return SAVE_BTN_WIDTH; } // Reusing SAVE_BTN constants
int Screens::getBackBtnHeight() { return SAVE_BTN_HEIGHT; } // Reusing SAVE_BTN constants