#include "ModClock.hpp"

#include <ESP8266WiFi.h>

#include "Color.hpp"
#include "JsonConfig.hpp"
#include "Display.hpp"
#include "ProtoThread.h"

using Display::matrix;

namespace
{
    constexpr unsigned long NET_INTERVAL{60 * 60 * 1000};
    constexpr unsigned long NET_INTERVAL_AT_START{2500};
    constexpr unsigned long CLOCK_INTERVAL{1000};

    constexpr int CLOCK_X = 1;
    constexpr int CLOCK_Y = 0;
    constexpr int CLOCK_SECOND_X = 0;
    constexpr int CLOCK_SECOND_Y = 7;
} // namespace

void ModClock::setup()
{
    _netInterval = NET_INTERVAL_AT_START;

    auto &config = JsonConfig::instance();
    _continent = config.clockContinent();
    _city = config.clockCity();

    if (_continent && _city)
    {
        _client = std::unique_ptr<WorldTimeApiOrgClient>(new WorldTimeApiOrgClient(_continent, _city));
    }
}

void ModClock::loop()
{
    clockTickLoop();
    netLoop();
}

void ModClock::show()
{
    THREAD_INTERVAL(500);

    const auto unixTime = _unixTime;

    drawClock(unixTime);

    if (unixTime != 0)
    {
        drawSecond(unixTime);
    }
}

void ModClock::netLoop()
{
    THREAD_INTERVAL(_netInterval);

    if (!_client ||
        (WiFi.status() != WL_CONNECTED))
    {
        return;
    }

    auto unixTime = _client.get()->fetchTime();

    if (unixTime != 0)
    {
        _unixTime = unixTime;

        if (!_initDone)
        {
            _initDone = true;
            _netInterval = NET_INTERVAL;
        }
    }
}

void ModClock::clockTickLoop()
{
    THREAD_INTERVAL(CLOCK_INTERVAL);

    if (_unixTime != 0)
    {
        ++_unixTime;
    }
}

void ModClock::drawClock(std::time_t unixTime)
{
    static int prevSec{-1};
    const int sec = unixTime % 60;

    auto timeStr = getFormattedTime(unixTime);

    if ((unixTime != 0) && (sec == prevSec))
    {
        timeStr.replace(":", " ");
    }

    matrix.clear();
    matrix.setCursor(CLOCK_X, CLOCK_Y);
    matrix.setTextColor(Color::WHITE);
    matrix.print(timeStr);

    prevSec = sec;
}

void ModClock::drawSecond(std::time_t unixTime)
{
    const int sec = unixTime % 60;
    const int x = (sec < 30) ? sec : 60 - sec;
    auto color = (sec % 15 == 0) ? Color::PURPLE : Color::BLUE;
    matrix.drawPixel(CLOCK_SECOND_X + x, CLOCK_SECOND_Y, color);
}

String ModClock::getFormattedTime(std::time_t unixTime)
{
    if (unixTime != 0)
    {
        auto tm = *std::localtime(&unixTime);
        char buffer[6]{0};

        if (std::strftime(buffer, sizeof(buffer), "%H:%M", &tm))
        {
            if (buffer[0] == '0')
            {
                buffer[0] = ' ';
            }

            for (int i = 1; i < 5; ++i)
            {
                if (buffer[i] == '0')
                {
                    buffer[i] = 'O';
                }
            }

            return String(buffer);
        }
    }

    return F("--:--");
}
