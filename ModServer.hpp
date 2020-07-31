#pragma once

#include "ModuleIf.hpp"

class ModServer : public ModuleIf
{
public:
    void setup() override;
    void loop() override;

private:
    bool setupDone{false};
};
