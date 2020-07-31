#pragma once

#include <vector>

class ModuleIf
{
public:
    ModuleIf() = default;
    virtual ~ModuleIf() = 0;

    virtual void setup(){};
    virtual void loop(){};
    virtual void show(){};

    using List = std::vector<ModuleIf *>;
};
