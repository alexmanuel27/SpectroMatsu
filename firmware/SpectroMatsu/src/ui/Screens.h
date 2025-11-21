// src/ui/Screens.h
#ifndef SCREENS_H
#define SCREENS_H

#include <Arduino.h>
#include "../utils/Config.h" // Incluimos Config.h para ScreenState, SpectrumPoint, SampleData

class Screens {
public:
    static void drawMainScreen(bool wifiEnabled);
    static void drawReferenceScreen();
    static void drawSampleScreen();
    static void drawGraphScreen();
    static void drawSaveScreen(const String& sampleName);
    // --- CORREGIDO: Removida la declaración duplicada ---
    // static void drawSettingsScreen(bool wifiEnabled, const String& systemInfo = ""); // Añadimos parámetro systemInfo
    // static void drawAboutScreen(bool showQR = true); // showQR para poder ocultarlo si es necesario
    // --- FIN CORRECCIÓN ---
    static void drawSettingsScreen(bool wifiEnabled, const String& systemInfo = ""); // Añadimos parámetro systemInfo
    // --- Nueva función para dibujar el QR ---
    static void drawAboutScreen(bool showQR = true); // showQR para poder ocultarlo si es necesario
    // --- Fin nueva función ---
    static void drawSamplesScreen(bool wifiEnabled, SampleData* samples, int sampleCount); // CORREGIDO: Añadida declaración faltante
    static void drawLoadingScreen(const String& message);

    static int getBtn1X();
    static int getBtn1Y();
    static int getBtn2X();
    static int getBtn2Y();
    static int getBtnWidth();
    static int getBtnHeight();
    static int getSaveBtnX();
    static int getSaveBtnY();
    static int getSaveBtnWidth();
    static int getSaveBtnHeight();
    static int getBackBtnX();
    static int getBackBtnY();
    static int getBackBtnWidth();
    static int getBackBtnHeight();
    // --- WiFi Button Coordinates (Solo en MAIN_SCREEN) ---
    static int getWifiBtnX();
    static int getWifiBtnY();
    static int getWifiBtnWidth();
    static int getWifiBtnHeight();
    // --- Menu Button Coordinates (valid when menu button is drawn) ---
    static int getMenuBtnX();
    static int getMenuBtnY();
    static int getMenuBtnSize();
    // --- Menu Coordinates ---
    static int getMenuX();
    static int getMenuY();
    static int getMenuWidth();
    static int getMenuHeight();
    // --- Get Menu Option Index for a Screen State ---
    static int getScreenMenuIndex(ScreenState state);

    // --- Menu Drawing Helper ---
    // static void drawMenu(bool visible, ScreenState currentScreen); // Ya no es necesaria aquí
    // --- Redraw Helper for Main Screen ---
    static void redrawMenuButton(); // Nueva función para redibujar solo el botón de menú
    // --- Redraw Helper for WiFi Button ---
    // static void redrawWifiButton(bool wifiEnabled); // Ya no es necesaria aquí

private:
    // Private helper to calculate button positions
    static int calculateBtnX(int offset);
    static int _btn1X, _btn1Y, _btn2X, _btn2Y; // Cached button positions
    static int _saveBtnX, _saveBtnY; // Cached save button position
    static int _backBtnX, _backBtnY; // Cached back button position
    static int _wifiBtnX, _wifiBtnY; // Cached WiFi button position
    // --- Cached Menu Button Coordinates (only for main screen) ---
    static int _menuBtnX, _menuBtnY;
    // --- Cached Menu Coordinates ---
    static int _menuX, _menuY, _menuHeight; // Calculated dynamically
};

#endif // SCREENS_H