#include "Display.hpp"

#include "PinConfig.hpp"

namespace Display
{
    const CRGB COLOR_CORRECTION = {0xFF, 0xAF, 0xCF};
    CRGB pixels[SIZE]{};

    FastLED_NeoMatrix matrix{
        pixels,
        WIDTH,
        HEIGHT,
        TYPE};

    CLEDController *ledController{nullptr};

    void setup()
    {
        ledController = &(FastLED.addLeds<NEOPIXEL, PinConfig::MATRIX_DISPLAY>(Display::pixels, Display::SIZE));

        FastLED.setMaxPowerInMilliWatts(500);

        ledController->setCorrection(Display::COLOR_CORRECTION);

        matrix.begin();
        matrix.setBrightness(Display::BRIGHTNESS_MIN);
        matrix.setTextWrap(false);
    }
} // namespace Display
