#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "hebrew_font.h"
#include "hebrew_english_16px.h"
#include "hebrew_only_16px.h"
#define PANEL_WIDTH 64
#define PANEL_HEIGHT 64
#define PANELS_NUMBER 2
#define PANEL_CHAIN PANELS_NUMBER

// Define pin E (required for 64px high panels)
#define PIN_E 18

// Matrix configuration
MatrixPanel_I2S_DMA *dma_display = nullptr;

void setup() {
  HUB75_I2S_CFG mxconfig(
    PANEL_WIDTH,
    PANEL_HEIGHT,
    PANEL_CHAIN
  );

  mxconfig.gpio.e = PIN_E;
  mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(80);
  dma_display->clearScreen();

  drawHebrewWord(40, 24); // draw center-ish
}

void loop() {
  // Static display, nothing here
}

void drawHebrewLetter(int16_t x, int16_t y, uint8_t index, uint16_t color) {
  for (uint8_t row = 0; row < 8; row++) {
    uint8_t line = pgm_read_byte(&simpleHebrewFont[index][row]);
    for (uint8_t col = 0; col < 8; col++) {
      if (line & (0x80 >> col)) {
        dma_display->drawPixel(x + col, y + row, color);
      }
    }
  }
}

void drawHebrewWord(int16_t startX, int16_t startY) {
  uint16_t color = dma_display->color565(255, 255, 0); // yellow
  int16_t x = startX;

  // Draw Right-to-Left
  for (int i = 3; i >= 0; i--) {
    drawHebrewLetter(x, startY, wordShalom[i], color);
    x += 10; // space between letters
  }
}
