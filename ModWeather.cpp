#include "ModWeather.hpp"

#include <map>

#include <ESP8266WiFi.h>

#include "Color.hpp"
#include "JsonConfig.hpp"
#include "Display.hpp"
#include "Ppm.hpp"
#include "ProtoThread.h"

using Display::matrix;

namespace
{
    constexpr unsigned long INTERVAL{10UL * 60 * 1000};
    constexpr unsigned long INTERVAL_AT_START{3334};
    constexpr double KELVIN_OFFSET{-273.15};

    static char TEMP_TEXT_BUFFER[8]{0};
    static const std::map<const char *, const char *> ICON_MAP{
        {"01d", "/clear_sky.ppm"},
        {"02d", "/few_clouds.ppm"},
        {"03d", "/scattered_clouds.ppm"},
        {"04d", "/broken_clouds.ppm"},
        {"09d", "/shower_rain.ppm"},
        {"10d", "/rain.ppm"},
        {"11d", "/thunderstorm.ppm"},
        {"13d", "/snow.ppm"},
        {"50d", "/mist.ppm"}};
    static const char MISSING_DATA[] = "----";
} // namespace

void ModWeather::setup()
{
    _interval = INTERVAL_AT_START;

    auto &config = JsonConfig::instance();
    _apiKey = config.weatherApiKey();
    _city = config.weatherCity();

    if (_apiKey && _city)
    {
        _client = std::unique_ptr<OpenWeatherMapOrgClient>(new OpenWeatherMapOrgClient(_apiKey, _city));
    }

    _sunPpm = Ppm::fromFile(ICON_MAP.at("01d"));
}

void ModWeather::loop()
{
    THREAD_INTERVAL(_interval);

    if (!_client ||
        (WiFi.status() != WL_CONNECTED))
    {
        return;
    }

    _interval = INTERVAL;
    _client.get()->fetchWeather();
}

void ModWeather::show()
{
    THREAD_INTERVAL(1000);

    const int w = _sunPpm.width();
    const int h = _sunPpm.height();
    auto pixel = _sunPpm.pixels().cbegin();

    matrix.clear();

    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            matrix.drawPixel(x, y, *pixel);
            ++pixel;
        }
    }

    auto tempStr = getTemperature();
    matrix.setCursor(8, 0);
    matrix.setTextColor(Color::WHITE);
    matrix.print(tempStr);
}

const char *ModWeather::getTemperature()
{
    if (_client)
    {
        auto tempValue = _client.get()->getTemperature();

        if (tempValue != 0)
        {
            int temp = round(tempValue + KELVIN_OFFSET);
            snprintf(TEMP_TEXT_BUFFER, sizeof(TEMP_TEXT_BUFFER), "%d", temp);
            return TEMP_TEXT_BUFFER;
        }
    }

    return MISSING_DATA;
}
