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

void Display::drawString(int xPos, int yPos, String str, const char *fontData) {
    gfx.setFont(fontData);
    gfx.drawString(xPos, yPos, str);
}

void Display::drawStringHCentered(int yPos, String str, const char *fontData) {
    gfx.setFont(fontData);
    int xPos =
        (SCREEN_WIDTH - gfx.getStringWidth(str.c_str(), str.length())) / 2;
    gfx.drawString(xPos, yPos, str);
}

void Display::paint() { gfx.commit(); }

void Display::clear() { gfx.fillBuffer(1); }

void Display::fillRectangle(int xPos, int yPos, int width, int height) {
    gfx.fillRect(xPos, yPos, width, height);
}

void Display::setColor(int color) { gfx.setColor(color); }

void Display::drawXBitmap(int xPos, int yPos, int width, int height,
                          const char *bitmap) {
    gfx.drawXbm(xPos, yPos, width, height, bitmap);
}