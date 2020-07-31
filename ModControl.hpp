#pragma once

#include "ModuleIf.hpp"

class ModControl : public ModuleIf
{
public:
    void setup() override;
    void loop() override;

private:
    void switchVisibleModule(int step);

    int _currentIndex{0};
    int _prevLeftButton{0};
    int _prevMiddleButton{0};
    int _prevRightButton{0};
};
