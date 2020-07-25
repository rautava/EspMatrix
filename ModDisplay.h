#pragma once

#include "ModTime.h"
#include "ModuleIf.h"

class ModDisplay : public ModuleIf
{
public:
    void loop() override;

    void setTimeModule(ModTime *modTime)
    {
        _modTime = modTime;
    }

private:
    void clockLoop();
    void drawClock(std::time_t unixTime);
    void drawSecond(std::time_t unixTime);
    void scrollLoop();

    ModTime *_modTime;
    int _currentIndex = 0;
};
