#pragma once

#include <ESP8266HTTPClient.h>

class OpenWeatherMapOrgClient
{
public:
  OpenWeatherMapOrgClient(
      const String &apiKey,
      const String &city);

  void fetchWeather();

  int getWeatherId()
  {
    return _weatherId;
  }

  double getTemperature()
  {
    return _temperature;
  };

private:
  void parseResponse();

  WiFiClient _wifiClient{};
  HTTPClient _httpClient{};
  String _url{};

  int _weatherId{0};
  double _temperature{0};
};
