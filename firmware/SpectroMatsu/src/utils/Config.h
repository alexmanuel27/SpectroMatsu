// src/utils/Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h> // Para String

// --- Constantes para estructuras de datos ---
// Definir GRAPH_WAVELENGTH_MIN y GRAPH_WAVELENGTH_MAX ANTES de usarlas en SampleData
#define GRAPH_WAVELENGTH_MIN 200
#define GRAPH_WAVELENGTH_MAX 900
#define GRAPH_ABSORBANCE_MIN 0.0f
#define GRAPH_ABSORBANCE_MAX 3.0f

// --- Estructuras de Datos ---
struct SpectrumPoint {
  float wavelength;
  float absorbance;
};

// Definir MAX_SPECTRUM_POINTS basado en las constantes ya definidas
#define MAX_SPECTRUM_POINTS (GRAPH_WAVELENGTH_MAX - GRAPH_WAVELENGTH_MIN + 1) // 701 puntos (200-900nm en pasos de 1)

struct SampleData {
  String id;
  String timestamp;
  // Usar MAX_SPECTRUM_POINTS en lugar de la operación aritmética directa
  SpectrumPoint spectrum[MAX_SPECTRUM_POINTS]; // Array fijo, 701 puntos (200-900nm)
  int numPoints; // Número real de puntos en este espectro (de 0 a MAX_SPECTRUM_POINTS)
};

// --- Pin Definitions ---
#define PIN_LCD_BL 38
#define PIN_LCD_D0 39
#define PIN_LCD_D1 40
#define PIN_LCD_D2 41
#define PIN_LCD_D3 42
#define PIN_LCD_D4 45
#define PIN_LCD_D5 46
#define PIN_LCD_D6 47
#define PIN_LCD_D7 48
#define PIN_POWER_ON 15
#define PIN_LCD_RES 5
#define PIN_LCD_CS 6
#define PIN_LCD_DC 7
#define PIN_LCD_WR 8
#define PIN_LCD_RD 9
#define PIN_BUTTON_1 0
#define PIN_BUTTON_2 14
#define BOARD_I2C_SCL 17
#define BOARD_I2C_SDA 18
#define BOARD_TOUCH_IRQ 16
#define BOARD_TOUCH_RST 21

// --- Display & Button Dimensions ---
#define BTN_WIDTH 120
#define BTN_HEIGHT 50
#define SAVE_BTN_WIDTH 60
#define SAVE_BTN_HEIGHT 30
#define SAVE_BTN_MARGIN_X 10
#define SAVE_BTN_MARGIN_Y 10
#define WIFI_BTN_WIDTH 60
#define WIFI_BTN_HEIGHT 30
#define WIFI_BTN_MARGIN_X 10
#define WIFI_BTN_MARGIN_Y 10
#define MENU_BTN_SIZE 30
#define MENU_BTN_MARGIN 10
#define MENU_WIDTH 120
#define MENU_ITEM_HEIGHT 40
#define MENU_ITEM_MARGIN 5
#define MENU_INDICATOR_SIZE 10
#define MENU_INDICATOR_MARGIN 5

// --- RAM Storage ---
#define MAX_SAMPLES 10 // Máximo de muestras que se pueden guardar en RAM (ajusta según necesidad)

// --- WiFi Credentials ---
#define WIFI_SSID "SpectroMatsu"
#define WIFI_PASS "12345678"

// --- Default Sample Date ---
#define DEFAULT_SAMPLE_DATE "1/1/1959"

// --- Screen States ---
enum ScreenState {
    MAIN_SCREEN,
    REFERENCE_SCREEN,
    SAMPLE_SCREEN,
    GRAPH_SCREEN,
    SAVE_SCREEN, // Nueva pantalla para ingresar nombre
    SETTINGS_SCREEN,
    SAMPLES_SCREEN,
    ABOUT_SCREEN,
    MENU_VISIBLE // State to know if the menu is open
};

#endif // CONFIG_H