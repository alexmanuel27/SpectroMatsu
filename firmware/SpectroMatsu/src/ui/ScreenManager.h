// src/ui/ScreenManager.h
#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Arduino.h>
#include "../utils/Config.h" // For ScreenState and SampleData
// --- Incluir WebServer ---
#include <WebServer.h>
// --- Fin inclusión ---

class ScreenManager {
public:
    ScreenManager();
    void init();
    void update(); // Main update loop
    void processTouch(int x, int y); // Called by TouchHandler

private:
    ScreenState _currentScreen;
    ScreenState _previousScreen; // To return from menu
    bool _menuVisible;
    bool _wifiEnabled; // Nuevo estado para WiFi
    unsigned long _loadingStartTime; // For 3s loading delay
    static const unsigned long LOADING_DURATION = 3000; // 3 seconds in milliseconds

    // --- Debounce Variables ---
    unsigned long _lastTouchProcessedTime;
    static const unsigned long TOUCH_DEBOUNCE_DELAY = 200; // 200 ms de debounce

    // --- RAM Storage for Samples ---
    SampleData _samples[MAX_SAMPLES]; // Array de muestras en RAM
    int _sampleCount; // Cuántas muestras tenemos guardadas actualmente

    // --- Web Server Instance ---
    WebServer _server; // Instancia del servidor web
    bool _webServerStarted; // Bandera para saber si el servidor está corriendo

    bool _isTouchDebounced(); // Función para comprobar debounce
    void _resetTouchDebounce(); // Función para reiniciar el contador de debounce

    // --- Nuevas funciones para manejar el servidor web ---
    void _startWebServer();
    void _stopWebServer();
    void _handleRoot(); // Handler para la página principal
    void _handleSamples(); // Handler para listar muestras
    void _handleDownload(); // Handler para descargar una muestra específica
    void _handleTurnOffAP(); // Handler para apagar el AP
    // --- Nueva declaración ---
    void _handleDownloadAll(); // Handler para descargar todas las muestras
    // --- Fin nueva declaración ---
    // --- Fin nuevas funciones ---

    void _switchToScreen(ScreenState newState);
    void _toggleMenu();
    void _toggleWiFi(); // Nueva función para manejar WiFi
    void _takeAndSaveSample(const String& sampleName); // Nueva función para guardar muestra con nombre
    String generateSampleName(); // Genera un nombre de muestra (simulado)
    String getSystemInfo(); // Obtiene información del sistema como un string
    void _handleMainScreenTouch(int x, int y);
    void _handleGraphScreenTouch(int x, int y);
    void _handleSaveScreenTouch(int x, int y);
    void _handleSettingsScreenTouch(int x, int y);
    void _handleSamplesScreenTouch(int x, int y);
    void _handleAboutScreenTouch(int x, int y);
    void _handleMenuTouch(int x, int y);
    bool _handleMenuButtonTouch(int x, int y);
    bool _handleWifiButtonTouch(int x, int y); // Nueva función para manejar toque en WiFi
    bool _handleSaveButtonTouch(int x, int y); // Nueva función para manejar toque en SAVE
    bool isPointInRect(int16_t px, int16_t py, int16_t rx, int16_t ry, int16_t rw, int16_t rh);

    // --- Nuevas funciones auxiliares declaradas aquí ---
    void _drawMenu(ScreenState currentScreen);
    void _redrawCurrentScreen();
    int _getScreenMenuIndex(ScreenState state);
    int _getMenuOptionY(int optionIndex);
    // --- Fin nuevas funciones auxiliares ---
};

#endif // SCREENMANAGER_H