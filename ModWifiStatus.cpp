#include "ModWifiStatus.hpp"

#include <ESP8266WiFi.h>

#include "Color.hpp"
#include "Display.hpp"
#include "ProtoThread.h"

namespace
{
    constexpr int WIFI_STATUS_X = 31;
    constexpr int WIFI_STATUS_Y = 7;
} // namespace

void ModWifiStatus::show()
{
    THREAD_INTERVAL(500);

    switch (WiFi.status())
    {
    case WL_CONNECTED:
        break;
    case WL_IDLE_STATUS:
        Display::matrix.drawPixel(WIFI_STATUS_X, WIFI_STATUS_Y, Color::YELLOW);
        break;
    default:
        Display::matrix.drawPixel(WIFI_STATUS_X, WIFI_STATUS_Y, Color::RED);
    }
}
