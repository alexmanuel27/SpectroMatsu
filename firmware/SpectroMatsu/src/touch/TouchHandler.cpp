// src/touch/TouchHandler.cpp
#include "TouchHandler.h"
#include <TouchDrvCSTXXX.hpp>
#include <TFT_eSPI.h>
extern TFT_eSPI tft; // Global display instance
extern TouchDrvCSTXXX touch; // Global touch sensor instance

TouchHandler::TouchHandler(ScreenManager* sm) : _screenManager(sm) {}

void TouchHandler::init() {
    touch.setPins(BOARD_TOUCH_RST, BOARD_TOUCH_IRQ);
    if (!touch.begin(Wire, CST328_SLAVE_ADDRESS, BOARD_I2C_SDA, BOARD_I2C_SCL)) {
        Serial.println("Failed init CST328 Device!");
        if (!touch.begin(Wire, CST816_SLAVE_ADDRESS, BOARD_I2C_SDA, BOARD_I2C_SCL)) {
            Serial.println("Failed init CST816 Device!");
            while (1) {
                Serial.println("Not find touch device!");
                delay(1000);
            }
        }
    }
    touch.disableAutoSleep();
}

void TouchHandler::handleTouch() {
    uint8_t touched = touch.getPoint(_x, _y, touch.getSupportTouchPoint());
    if (touched) {
        for (int i = 0; i < touched; ++i) {
            Serial.print("Touch Raw X["); Serial.print(i); Serial.print("]:"); Serial.print(_x[i]);
            Serial.print(" Y["); Serial.print(i); Serial.print("]:"); Serial.print(_y[i]); Serial.println();

            int mappedX, mappedY;
            transformTouchCoords(_x[i], _y[i], mappedX, mappedY);

            Serial.print("Mapped Touch X:"); Serial.print(mappedX); Serial.print(" Y:"); Serial.println(mappedY);

            _screenManager->processTouch(mappedX, mappedY); // Forward to ScreenManager
        }
    }
    delay(5);
}

void TouchHandler::transformTouchCoords(int16_t rawX, int16_t rawY, int& mappedX, int& mappedY) {
    int touch_width = 320;   // Touch panel native width
    int touch_height = 170;  // Touch panel native height
    int tft_width = tft.width();  // Display width (170)
    int tft_height = tft.height(); // Display height (320)

    // Normalize raw touch to display size
    int adjusted_x = map(rawX, 0, touch_width, 0, tft_width);
    int adjusted_y = map(rawY, 0, touch_height, 0, tft_height);

    // Apply rotation transformation: Touch (0) → Display (1)
    // X_display = Y_touch
    // Y_display = Height - 1 - X_touch
    mappedX = adjusted_y;
    mappedY = tft_height - 1 - adjusted_x;
}

bool TouchHandler::isPointInRect(int16_t px, int16_t py, int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    return (px >= rx && px <= rx + rw && py >= ry && py <= ry + rh);
}