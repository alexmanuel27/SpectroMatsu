// src/touch/TouchHandler.h
#ifndef TOUCHHANDLER_H
#define TOUCHHANDLER_H

#include <Arduino.h>
#include "../ui/ScreenManager.h" // Forward declare ScreenManager

class ScreenManager; // Forward declaration

class TouchHandler {
public:
    TouchHandler(ScreenManager* sm);
    void init();
    void handleTouch();

private:
    ScreenManager* _screenManager; // Pointer to manage screen state
    int16_t _x[5], _y[5]; // Raw touch coordinates

    void transformTouchCoords(int16_t rawX, int16_t rawY, int& mappedX, int& mappedY);
    bool isPointInRect(int16_t px, int16_t py, int16_t rx, int16_t ry, int16_t rw, int16_t rh);
};

#endif // TOUCHHANDLER_H