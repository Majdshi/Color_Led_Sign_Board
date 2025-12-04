#ifndef HEBREW_FONT_H
#define HEBREW_FONT_H

#include <Arduino.h>

// Very small 8x8 Hebrew letters (basic demo font)
const uint8_t simpleHebrewFont[][8] PROGMEM = {
  // ש
  {0b11111110,
   0b01001010,
   0b01001010,
   0b01001010,
   0b01001010,
   0b01001010,
   0b11111110,
   0b00000000},

  // ל
  {0b00000010,
   0b00000010,
   0b00000010,
   0b00000010,
   0b00000010,
   0b00000010,
   0b11111110,
   0b00000000},

  // ו
  {0b00001000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00000000,
   0b00000000},

  // ם
  {0b11111110,
   0b01000010,
   0b01000010,
   0b01000010,
   0b01000010,
   0b01000010,
   0b11111110,
   0b00000000}
};

// The Hebrew word "שלום" is composed of indices 0–3 above
const uint8_t wordShalom[4] = {0, 1, 2, 3};

#endif
