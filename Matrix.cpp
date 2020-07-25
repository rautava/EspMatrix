#include "Matrix.h"

#include "PinConfig.h"

namespace Matrix
{
    const CRGB COLOR_CORRECTION = {0xFF, 0x9F, 0xBF};
    CRGB pixels[SIZE]{};

    FastLED_NeoMatrix matrix{
        pixels,
        WIDTH,
        HEIGHT,
        TYPE};

    CLEDController *ledController{nullptr};

    void setup()
    {
        ledController = &(FastLED.addLeds<NEOPIXEL, PinConfig::MATRIX>(Matrix::pixels, Matrix::SIZE));

        FastLED.setMaxPowerInMilliWatts(500);

        //ledController->setCorrection(Matrix::COLOR_CORRECTION);

        matrix.begin();
        matrix.setBrightness(Matrix::BRIGHTNESS_MIN);
        matrix.setTextWrap(false);
    }
} // namespace Matrix
