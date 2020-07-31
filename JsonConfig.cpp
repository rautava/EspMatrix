#include "JsonConfig.hpp"

#include <FS.h>

JsonConfig *JsonConfig::_instance{nullptr};
DynamicJsonDocument JsonConfig::_jsonDoc{512};

JsonConfig &JsonConfig::instance()
{
    if (!_instance)
    {
        _instance = new JsonConfig();
    }

    return *_instance;
}

JsonConfig::JsonConfig()
{
    File configFile = SPIFFS.open("/config.json", "r");
    auto error = deserializeJson(_jsonDoc, configFile);
    configFile.close();

    if (error)
    {
        Serial.printf("Invalid config: %d\n", error.code());
        return;
    }
}

String JsonConfig::clockContinent()
{
    return _jsonDoc["clock"]["continent"].as<String>();
}

String JsonConfig::clockCity()
{
    return _jsonDoc["clock"]["city"].as<String>();
}

String JsonConfig::weatherApiKey()
{
    return _jsonDoc["weather"]["apikey"].as<String>();
}

String JsonConfig::weatherCity()
{
    return _jsonDoc["weather"]["city"].as<String>();
}

String JsonConfig::wifiSsid()
{
    return _jsonDoc["wifi"]["ssid"].as<String>();
}

String JsonConfig::wifiPassword()
{
    return _jsonDoc["wifi"]["password"].as<String>();
}

String JsonConfig::wifiHostname()
{
    return _jsonDoc["wifi"]["hostname"].as<String>();
}
