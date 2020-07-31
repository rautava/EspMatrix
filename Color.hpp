#pragma once

#include <stdint.h>

namespace Color
{
    constexpr uint16_t BLACK = 0x0000;
    constexpr uint16_t WHITE = 0xFFFF;

    constexpr uint16_t RED = 0x1F << 11;
    constexpr uint16_t GREEN = 0x2F << 5;
    constexpr uint16_t BLUE = 0x1F;

    constexpr uint16_t YELLOW = RED | GREEN;
    constexpr uint16_t CYAN = GREEN | BLUE;
    constexpr uint16_t PURPLE = BLUE | RED;
} // namespace Color
