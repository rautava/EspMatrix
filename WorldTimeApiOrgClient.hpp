#pragma once

#include <ctime>

#include <ESP8266HTTPClient.h>

class WorldTimeApiOrgClient
{
public:
  WorldTimeApiOrgClient(
      const String &continent,
      const String &city);

  std::time_t fetchTime();

private:
  std::time_t parseResponse();

  WiFiClient _wifiClient{};
  HTTPClient _httpClient{};
  String _url{};
};
