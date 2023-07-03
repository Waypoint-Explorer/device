#include "Display.h"

uint16_t palette[] = {0, 1};
EPD_WaveShare42 epd(EPD_SS, EPD_RST, EPD_DC, EPD_BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);

Display::Display() {}

Display::Display(int rotation) {
    gfx.init();
    gfx.setRotation(rotation);
    gfx.fillBuffer(EPD_WHITE);
}

void Display::drawString(int xPos, int yPos, String str) {
    gfx.drawString(xPos, yPos, str);
}

void Display::paint() { gfx.commit(); }

void Display::clear() { gfx.fillBuffer(1); }
