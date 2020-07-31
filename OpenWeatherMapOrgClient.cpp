#include "OpenWeatherMapOrgClient.hpp"

#include <ArduinoJson.h>
#include <tinyxml2.h>

namespace
{
    static const String HOST{"api.openweathermap.org"};
    static const int PORT = 80;
    static const String BASE_URL{"/data/2.5/weather?appid={APIKEY}&q={CITY}"};
    static const String APIKEY_PLACEHOLDER{"{APIKEY}"};
    static const String CITY_PLACEHOLDER{"{CITY}"};
} // namespace

OpenWeatherMapOrgClient::OpenWeatherMapOrgClient(
    const String &apiKey,
    const String &city)
    : _url{BASE_URL}
{
    _url.replace(APIKEY_PLACEHOLDER, apiKey);
    _url.replace(CITY_PLACEHOLDER, city);
}

void OpenWeatherMapOrgClient::fetchWeather()
{
    bool success{false};
    _httpClient.useHTTP10(true);

    if (_httpClient.begin(_wifiClient, HOST, PORT, _url))
    {
        auto code = _httpClient.GET();
        success = (code >= 0);

        if (!success)
        {
            Serial.printf("Error: %d %s\n", code, _url.c_str());
        }
    }
    else
    {
        Serial.println("HTTP client start failed.");
    }

    if (success)
    {
        parseResponse();
    }

    _httpClient.end();
}

void OpenWeatherMapOrgClient::parseResponse()
{
    DynamicJsonDocument jsonDoc{1024};
    auto error = deserializeJson(jsonDoc, _httpClient.getStream());

    if (error)
    {
        Serial.printf("Parse failed: %d\n", error.code());
        return;
    }

    _weatherId = jsonDoc["weather"][0]["id"].as<int>();
    _temperature = jsonDoc["main"]["temp"].as<double>();
}
