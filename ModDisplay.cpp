#include "ModDisplay.h"

#include "Color.h"
#include "Matrix.h"
#include "ProtoThread.h"

namespace
{
    using Matrix::matrix;

    constexpr auto TEXT = "Tama on testi";
    constexpr int CHAR_WIDTH_IN_PIXELS = 6;
    constexpr auto TEXT_START_X = Matrix::WIDTH - 1;

    int cursorX{TEXT_START_X};
    const int TEXT_WIDTH_IN_PIXELS = strlen(TEXT) * CHAR_WIDTH_IN_PIXELS;
} // namespace

void ModDisplay::loop()
{
    clockLoop();
}

void ModDisplay::clockLoop()
{
    THREAD_INTERVAL(500);

    auto unixTime = _modTime->getUnixTime();

    drawClock(unixTime);

    if (unixTime != 0)
    {
        drawSecond(unixTime);
    }

    matrix.show();
}

void ModDisplay::drawClock(std::time_t unixTime)
{
    static int prevSec{-1};
    const int sec = unixTime % 60;

    auto timeStr = _modTime->getFormattedTime(unixTime);

    if ((unixTime != 0) && (sec == prevSec))
    {
        timeStr.replace(":", " ");
    }

    matrix.clear();
    matrix.setCursor(1, 0);
    matrix.setTextColor(Color::WHITE);
    matrix.print(timeStr);

    prevSec = sec;
}

/*
void ModDisplay::drawSecond(std::time_t unixTime)
{
    int sec = unixTime % 60;
    int startX{0};
    int endX{30};

    if (sec < 30)
    {
        endX = sec;
    }
    else
    {
        startX = sec - 30;
    }

    for (int x = startX; x < endX; ++x)
    {
        matrix.drawPixel(1 + x, 7, Color::BLUE);
    }
}
*/

void ModDisplay::drawSecond(std::time_t unixTime)
{
    const int sec = unixTime % 60;
    const int x = (sec < 30) ? sec : 60 - sec;
    auto color = (sec % 15 == 0) ? Color::PURPLE : Color::BLUE;
    matrix.drawPixel(x, 7, color);
}

void ModDisplay::scrollLoop()
{
    THREAD_INTERVAL(70);

    matrix.clear();
    matrix.setCursor(cursorX, 0);
    matrix.setTextColor(Color::GREEN);
    matrix.print(TEXT);
    matrix.show();

    --cursorX;

    if (cursorX <= (-TEXT_WIDTH_IN_PIXELS))
    {
        cursorX = TEXT_START_X;
    }
}
