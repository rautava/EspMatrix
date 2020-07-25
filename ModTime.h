#pragma once

#include <ctime>

#include <Arduino.h>

#include "ModuleIf.h"

class ModTime : public ModuleIf
{
public:
    void loop() override;

    std::time_t getUnixTime()
    {
        return _unixTime;
    }

    static String getFormattedTime(std::time_t unixTime);

private:
    void netLoop();
    void clockLoop();

    std::time_t _unixTime{0};
};
