#include "ModControl.hpp"

#include "PinConfig.hpp"
#include "ProtoThread.h"

extern ModuleIf::List displayModules;
extern ModuleIf *visibleModule;

void ModControl::setup()
{
    pinMode(PinConfig::BUTTON_LEFT, INPUT_PULLUP);
    pinMode(PinConfig::BUTTON_MIDDLE, INPUT_PULLUP);
    pinMode(PinConfig::BUTTON_RIGHT, INPUT_PULLUP);
}

void ModControl::loop()
{
    auto leftButton = digitalRead(PinConfig::BUTTON_LEFT);
    auto middleButton = digitalRead(PinConfig::BUTTON_MIDDLE);
    auto rightButton = digitalRead(PinConfig::BUTTON_RIGHT);

    if (leftButton != _prevLeftButton)
    {
        _prevLeftButton = leftButton;

        if (leftButton == 0)
        {
            switchVisibleModule(-1);
        }
    }

    if (middleButton != _prevMiddleButton)
    {
        _prevMiddleButton = middleButton;
    }

    if (rightButton != _prevRightButton)
    {
        _prevRightButton = rightButton;

        if (rightButton == 0)
        {
            switchVisibleModule(1);
        }
    }
}

void ModControl::switchVisibleModule(int step)
{
    _currentIndex = (_currentIndex + displayModules.size() + step) % displayModules.size();
    visibleModule = displayModules.at(_currentIndex);
}
