#pragma once

#include "ModuleIf.h"

class ModServer : public ModuleIf
{
public:
    void setup() override;
    void loop() override;
};
