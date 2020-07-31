#include "Ppm.hpp"

#include <cstdlib>
#include <string.h>

#include <Arduino.h>
#include <FS.h>

namespace
{
    static char BUFFER[1024]{0};

    inline bool isSpace(char c)
    {
        return (c == ' ') || (c == '\n') || (c == '\r');
    }

    int getNextNumber(const char *&startIter, const char *&endIter, const char *end)
    {
        while (isSpace(*startIter) && (startIter < end))
        {
            ++startIter;
        }

        endIter = startIter;

        while (!isSpace(*endIter) && (endIter < end))
        {
            ++endIter;
        }

        char buffer[4]{0};
        int count = endIter - startIter;
        memcpy(buffer, startIter, (count > 3) ? 3 : count);
        int num = atoi(buffer);

        startIter = endIter;

        return num;
    }
} // namespace

Ppm Ppm::fromString(const char *data, size_t len)
{
    Ppm ppm{};

    if (len > 3 &&
        (data[0] == 'P') &&
        ((data[1] == '3') || (data[1] == '6')) &&
        isSpace(data[2]))
    {
        const bool isBinary = (data[1] == '6');
        const char *startIter = data + 3;
        const char *endIter = startIter;
        const char *end = startIter + len;

        ppm._width = getNextNumber(startIter, endIter, end);
        ppm._height = getNextNumber(startIter, endIter, end);
        ppm._maxColor = getNextNumber(startIter, endIter, end);

        if (isBinary)
        {
            while (isSpace(*startIter) && (startIter < end))
            {
                ++startIter;
            }

            while ((startIter + 2) < end)
            {
                int r = *(startIter++);
                int g = *(startIter++);
                int b = *(startIter++);
                uint32_t rgb = ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
                ppm._pixels.push_back(rgb);
            }
        }
        else
        {
            while (startIter < end)
            {
                int r = getNextNumber(startIter, endIter, end);
                int g = getNextNumber(startIter, endIter, end);
                int b = getNextNumber(startIter, endIter, end);
                uint32_t rgb = ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
                ppm._pixels.push_back(rgb);
            }
        }
    }

    return ppm;
}

Ppm Ppm::fromFile(const char *filename)
{
    File file = SPIFFS.open(filename, "r");
    auto size = file.size();

    if (size <= sizeof(BUFFER))
    {
        file.readBytes(BUFFER, size);
        file.close();
        return fromString(BUFFER, size);
    }
    else
    {
        file.close();
        return Ppm{};
    }
}