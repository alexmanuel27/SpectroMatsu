// src/graphics/GraphDrawer.h
#ifndef GRAPHDRAWER_H
#define GRAPHDRAWER_H

#include <Arduino.h>
#include "../utils/Config.h" // For GRAPH_WAVELENGTH_*, GRAPH_ABSORBANCE_*, BACK_BTN_* constants

class GraphDrawer {
public:
    // Public static functions for drawing UI elements
    static void drawSpectrumGraphWithRandomData();
    static void drawLoadingScreen(const String& message);
    static void drawBackButton(int x, int y, int width, int height);

private:
    // Private helper functions for internal drawing
    static void drawAxes(int graphX, int graphY, int graphWidth, int graphHeight);
    static void drawLabels(int graphX, int graphY, int graphWidth, int graphHeight);
};

#endif // GRAPHDRAWER_H