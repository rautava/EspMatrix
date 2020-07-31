#pragma once

#define FASTLED_INTERNAL
#include <FastLED_NeoMatrix.h>

namespace Display
{
    constexpr uint16_t WIDTH = 32;
    constexpr uint16_t HEIGHT = 8;
    constexpr int SIZE = WIDTH * HEIGHT;
    constexpr uint8_t TYPE = NEO_MATRIX_TOP | NEO_MATRIX_LEFT | NEO_MATRIX_COLUMNS | NEO_MATRIX_ZIGZAG;
    constexpr int BRIGHTNESS_MIN = 2;
    constexpr int BRIGHTNESS_MAX = 12;

    extern const CRGB COLOR_CORRECTION;
    extern CRGB pixels[SIZE];
    extern FastLED_NeoMatrix matrix;

    void setup();
} // namespace Display
