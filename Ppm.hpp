#pragma once

#include <list>
#include <stddef.h>
#include <stdint.h>

class Ppm
{
public:
    using PixelList = std::list<uint32_t>;

    Ppm() = default;

    static Ppm fromString(const char *data, size_t len);
    static Ppm fromFile(const char *filename);

    int width() const { return _width; }
    int height() const { return _height; }
    PixelList pixels() const { return _pixels; }

private:
    int _width{0};
    int _height{0};
    int _maxColor{0};
    PixelList _pixels;
};
