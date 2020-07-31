#pragma once

#include <ArduinoJson.h>

class JsonConfig
{
public:
    static JsonConfig &instance();

    String clockContinent();
    String clockCity();

    String weatherApiKey();
    String weatherCity();

    String wifiSsid();
    String wifiPassword();
    String wifiHostname();

private:
    JsonConfig();

    static JsonConfig *_instance;
    static DynamicJsonDocument _jsonDoc;
};
