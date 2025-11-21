// src/graphics/GraphDrawer.cpp
#include "GraphDrawer.h"
#include <TFT_eSPI.h>
extern TFT_eSPI tft; // Declaramos extern para acceder a la instancia global de tft
#include "../utils/Config.h" // Incluimos Config.h para usar las constantes

void GraphDrawer::drawSpectrumGraphWithRandomData() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString("Absorbance Spectrum", tft.width()/2, 10, 2);

    int margin = 35;
    int graphWidth = tft.width() - 2 * margin;
    int graphHeight = tft.height() - 70;
    int graphX = margin;
    int graphY = margin + 15;

    drawAxes(graphX, graphY, graphWidth, graphHeight);
    drawLabels(graphX, graphY, graphWidth, graphHeight);

    // Generar y dibujar puntos aleatorios
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

    // NO dibujar el botón BACK aquí, ya que ahora se dibuja en Screens::drawGraphScreen
    // El botón SAVE se dibuja en Screens::drawGraphScreen
    // drawBackButton(tft.width() - BACK_BTN_WIDTH - 10, tft.height() - BACK_BTN_HEIGHT - 10, BACK_BTN_WIDTH, BACK_BTN_HEIGHT); // <-- COMENTAR O ELIMINAR ESTA LINEA
    // Y eliminar las referencias a BACK_BTN_WIDTH y BACK_BTN_HEIGHT de esta función si no se usan para otra cosa.
    // La función drawBackButton también puede eliminarse si ya no se usa en ningún otro lugar.
}

void GraphDrawer::drawAxes(int graphX, int graphY, int graphWidth, int graphHeight) {
    tft.drawLine(graphX, graphY + graphHeight, graphX + graphWidth, graphY + graphHeight, TFT_WHITE); // X-axis
    tft.drawLine(graphX, graphY, graphX, graphY + graphHeight, TFT_WHITE); // Y-axis
}

void GraphDrawer::drawLabels(int graphX, int graphY, int graphWidth, int graphHeight) {
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM); // Middle-center
    tft.drawString("Wavelength (nm)", tft.width()/2, tft.height() - 15, 2);

    tft.setRotation(3);
    tft.drawString("Absorbance", -tft.height()/2, 15, 2);
    tft.setRotation(1); // Volver a rotación 1 para el resto

    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM); // Top-center
    tft.drawString(String(GRAPH_WAVELENGTH_MIN), graphX, graphY + graphHeight + 5, 1);
    tft.drawString(String(GRAPH_WAVELENGTH_MAX), graphX + graphWidth - 10, graphY + graphHeight + 5, 1);

    tft.setTextDatum(5); // Derecha del centro (equivale a RC_DATUM)
    tft.drawString(String(GRAPH_ABSORBANCE_MIN, 1), graphX - 5, graphY + graphHeight, 1);
    tft.drawString(String(GRAPH_ABSORBANCE_MAX, 1), graphX - 5, graphY, 1);
}

void GraphDrawer::drawLoadingScreen(const String& message) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(message, tft.width()/2, tft.height()/2, 4);

    int barWidth = 200;
    int barHeight = 10;
    int barX = (tft.width() - barWidth) / 2;
    int barY = tft.height()/2 + 30;

    tft.drawRect(barX, barY, barWidth, barHeight, TFT_WHITE);
    // Simular progreso
    for(int i = 0; i <= 100; i += 20) {
        tft.fillRect(barX, barY, i * barWidth / 100, barHeight, TFT_GREEN);
        delay(60);
    }
}

// Opcional: Si ya no se usa en ningún otro lugar, puedes eliminar esta función también.
// void GraphDrawer::drawBackButton(int x, int y, int width, int height) {
//     tft.fillRect(x, y, width, height, TFT_DARKGREEN);
//     tft.drawRect(x, y, width, height, TFT_WHITE);
//     tft.setTextColor(TFT_WHITE);
//     tft.setTextDatum(MC_DATUM);
//     tft.drawString("ATRAS", x + width/2, y + height/2, 2);
// }