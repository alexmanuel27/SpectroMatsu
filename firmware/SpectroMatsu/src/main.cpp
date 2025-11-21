// src/main.cpp
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <TouchDrvCSTXXX.hpp>
#include "ui/ScreenManager.h"
#include "touch/TouchHandler.h"

// Instancias globales (necesarias para que los módulos puedan acceder a ellas)
TFT_eSPI tft = TFT_eSPI();
TouchDrvCSTXXX touch = TouchDrvCSTXXX();

ScreenManager screenManager;
TouchHandler touchHandler(&screenManager);

void setup() {
    Serial.begin(115200);
    // No iniciamos WiFi aquí si solo usamos AP y lo manejamos internamente en ScreenManager
    // WiFi.begin(); // Comentado
    screenManager.init();
    touchHandler.init();
    Serial.println("System initialized.");
}

void loop() {
    touchHandler.handleTouch();
    screenManager.update();
}