#pragma once

#include <stdint.h>

#include "ModuleIf.hpp"

class ModWifiStatus : public ModuleIf
{
public:
    void show() override;

private:
    int16_t _prevStatus{-1};
};
