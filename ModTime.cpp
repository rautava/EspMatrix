#include "ModTime.h"

#include "ProtoThread.h"
#include "WorldTimeApiOrgClient.h"

namespace
{
    constexpr unsigned long INTERVAL{20 * 60 * 1000};
    constexpr unsigned long INTERVAL_AT_START{5000};
    constexpr unsigned long CLOCK_INTERVAL{1000};
    const String CONTINENT{"Europe"};
    const String CITY{"Helsinki"};

    bool initState{true};
    unsigned long netInterval{INTERVAL_AT_START};

    WorldTimeApiOrgClient client{CONTINENT, CITY};
} // namespace

void ModTime::loop()
{
    netLoop();
    clockLoop();
}

String ModTime::getFormattedTime(std::time_t unixTime)
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

void ModTime::netLoop()
{
    THREAD_INTERVAL(netInterval);

    auto unixTime = client.fetchTime();

    if (unixTime != 0)
    {
        _unixTime = unixTime;

        if (initState)
        {
            initState = false;
            netInterval = INTERVAL;
        }
    }
}

void ModTime::clockLoop()
{
    THREAD_INTERVAL(CLOCK_INTERVAL);

    if (_unixTime != 0)
    {
        ++_unixTime;
    }
}
