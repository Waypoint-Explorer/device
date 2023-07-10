#include "Display.h"

uint16_t palette[] = {0, 1};
EPD_WaveShare42 epd(EPD_SS, EPD_RST, EPD_DC, EPD_BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);

Display::Display() {}

Display::Display(int16_t rotation) {
    gfx.init();
    gfx.setRotation(rotation);
    gfx.fillBuffer(EPD_WHITE);
}

void Display::drawString(int16_t xPos, int16_t yPos, String str,
                         const char *fontData) {
    gfx.setFont(fontData);
    gfx.drawString(xPos, yPos, str);
}

void Display::drawStringHCentered(int16_t yPos, String str,
                                  const char *fontData) {
    gfx.setFont(fontData);
    int16_t xPos =
        (SCREEN_WIDTH - gfx.getStringWidth(str.c_str(), str.length())) / 2;
    gfx.drawString(xPos, yPos, str);
}

void Display::paint() { gfx.commit(); }

void Display::clear() { gfx.fillBuffer(1); }

void Display::fillRectangle(int16_t xPos, int16_t yPos, int16_t width,
                            int16_t height) {
    gfx.fillRect(xPos, yPos, width, height);
}

void Display::setColor(int16_t color) { gfx.setColor(color); }

void Display::drawXBitmap(int16_t xPos, int16_t yPos, int16_t width,
                          int16_t height, const char *bitmap) {
    gfx.drawXbm(xPos, yPos, width, height, bitmap);
}