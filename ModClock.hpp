#pragma once

#include <ctime>
#include <memory>

#include <Arduino.h>

#include "ModuleIf.hpp"
#include "WorldTimeApiOrgClient.hpp"

class ModClock : public ModuleIf
{
public:
    void setup() override;
    void loop() override;
    void show() override;

private:
    void netLoop();
    void clockTickLoop();
    void drawClock(std::time_t unixTime);
    void drawSecond(std::time_t unixTime);
    String getFormattedTime(std::time_t unixTime);

    bool _initDone{false};
    unsigned long _netInterval{0};

    String _continent{};
    String _city{};
    std::unique_ptr<WorldTimeApiOrgClient> _client{};
    std::time_t _unixTime{0};
};
