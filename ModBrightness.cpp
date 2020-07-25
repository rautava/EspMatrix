#include "ModBrightness.h"

#include <list>

#include "Matrix.h"
#include "PinConfig.h"
#include "ProtoThread.h"

namespace
{
    constexpr int ADC_MIN{100};
    constexpr int ADC_MAX{1024};
    constexpr float COEFFICIENT{0.6321}; // (1-1/e)
    constexpr int MEASUREMENTS_COUNT_MAX{5};

    std::list<int> measurements{};

    int targetBrightness{Matrix::BRIGHTNESS_MIN};
    float currentBrightness{Matrix::BRIGHTNESS_MIN};

    int getBrightness(int adcValue)
    {
        if (adcValue >= ADC_MAX)
        {
            return Matrix::BRIGHTNESS_MAX;
        }
        else if (adcValue <= ADC_MIN)
        {
            return Matrix::BRIGHTNESS_MIN;
        }
        else
        {
            float adcRange = (float)ADC_MAX - (float)ADC_MIN;
            float brightnessRange = (float)Matrix::BRIGHTNESS_MAX - (float)Matrix::BRIGHTNESS_MIN;
            float x = (float)(adcValue - ADC_MIN) / adcRange;
            float y = (float)Matrix::BRIGHTNESS_MIN + x * x * brightnessRange;
            return round(y);
        }
    }

    int getMovingAverage()
    {
        const int adcValue = analogRead(PinConfig::PHOTO_SENSOR);
        measurements.push_back(adcValue);

        if (measurements.size() > MEASUREMENTS_COUNT_MAX)
        {
            measurements.pop_front();
        }

        int sum{0};

        for (auto value : measurements)
        {
            sum += value;
        }

        return round((float)sum / measurements.size());
    }
} // namespace

void ModBrightness::loop()
{
    THREAD_INTERVAL(500);

    auto adcValue = getMovingAverage();
    targetBrightness = getBrightness(adcValue);

    float diff = (float)targetBrightness - currentBrightness;
    currentBrightness += COEFFICIENT * diff;

    Matrix::matrix.setBrightness(round(currentBrightness));
}
