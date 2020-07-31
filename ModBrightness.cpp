#include "ModBrightness.hpp"

#include <list>

#include "Display.hpp"
#include "PinConfig.hpp"
#include "ProtoThread.h"

namespace
{
    constexpr int ADC_MIN{100};
    constexpr int ADC_MAX{1024};
    constexpr double COEFFICIENT{0.6321}; // (1-1/e)
    constexpr int MEASUREMENTS_COUNT_MAX{5};

    std::list<int> measurements{};

    int targetBrightness{Display::BRIGHTNESS_MIN};
    double currentBrightness{Display::BRIGHTNESS_MIN};

    int getBrightness(int adcValue)
    {
        if (adcValue >= ADC_MAX)
        {
            return Display::BRIGHTNESS_MAX;
        }
        else if (adcValue <= ADC_MIN)
        {
            return Display::BRIGHTNESS_MIN;
        }
        else
        {
            double adcRange = (double)ADC_MAX - (double)ADC_MIN;
            double brightnessRange = (double)Display::BRIGHTNESS_MAX - (double)Display::BRIGHTNESS_MIN;
            double x = (double)(adcValue - ADC_MIN) / adcRange;
            double y = (double)Display::BRIGHTNESS_MIN + x * x * brightnessRange;
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

        return round((double)sum / measurements.size());
    }
} // namespace

void ModBrightness::show()
{
    THREAD_INTERVAL(500);

    auto oldValue = round(currentBrightness);
    auto adcValue = getMovingAverage();
    targetBrightness = getBrightness(adcValue);

    double diff = (double)targetBrightness - currentBrightness;
    currentBrightness += COEFFICIENT * diff;
    auto newValue = round(currentBrightness);

    if (newValue != oldValue)
    {
        Display::matrix.setBrightness(newValue);
    }
}
