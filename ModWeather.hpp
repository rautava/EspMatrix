#pragma once

#include <memory>

#include <Arduino.h>

#include "ModuleIf.hpp"
#include "OpenWeatherMapOrgClient.hpp"
#include "Ppm.hpp"

class ModWeather : public ModuleIf
{
public:
    void setup() override;
    void loop() override;
    void show() override;

    const char *getTemperature();

private:
    bool _initDone{false};
    unsigned long _interval{0};

    String _apiKey{};
    String _city{};
    std::unique_ptr<OpenWeatherMapOrgClient> _client{};

    Ppm _sunPpm{};
};
