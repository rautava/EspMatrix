#include "WorldTimeApiOrgClient.hpp"

#include <ArduinoJson.h>

namespace
{
  static const String HOST{"worldtimeapi.org"};
  static const int PORT = 80;
  static const String BASE_URL{"/api/timezone/{CONTINENT}/{CITY}"};
  static const String CONTINENT_PLACEHOLDER{"{CONTINENT}"};
  static const String CITY_PLACEHOLDER{"{CITY}"};
} // namespace

WorldTimeApiOrgClient::WorldTimeApiOrgClient(
    const String &continent,
    const String &city)
    : _wifiClient{},
      _httpClient{},
      _url{BASE_URL}
{
  _url.replace(CONTINENT_PLACEHOLDER, continent);
  _url.replace(CITY_PLACEHOLDER, city);
}

std::time_t WorldTimeApiOrgClient::fetchTime()
{
  bool success{false};
  std::time_t unixTime{0};
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
    unixTime = parseResponse();
  }

  _httpClient.end();

  return unixTime;
}

std::time_t WorldTimeApiOrgClient::parseResponse()
{
  std::time_t unixTime{0};
  DynamicJsonDocument json{1024};
  auto error = deserializeJson(json, _httpClient.getStream());

  if (error)
  {
    Serial.printf("Parse failed: %d\n", error.code());
    return unixTime;
  }

  if (json.containsKey("unixtime"))
  {
    unixTime = json["unixtime"].as<std::time_t>();
    unixTime += json["raw_offset"].as<std::time_t>();

    if (json["dst"].as<bool>())
    {
      unixTime += json["dst_offset"].as<std::time_t>();
    }
  }
  else
  {
    Serial.println("Incompatible json");
  }

  return unixTime;
}
