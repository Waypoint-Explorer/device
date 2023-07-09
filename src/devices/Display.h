#ifndef __DISPLAY__
#define __DISPLAY__

#include "Arduino.h"
#include "EPD_WaveShare.h"
#include "EPD_WaveShare_42.h"
#include "MiniGrafx.h"
#include "utility/Logger.h"

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 400
#define BITS_PER_PIXEL 1
#define EPD_BLACK 0
#define EPD_WHITE 1

#define EPD_BUSY 34
#define EPD_SS 25
#define EPD_RST 33
#define EPD_DC 32
#define EPD_SCK 18
#define EPD_MOSI 23

/* Display class definition */
class Display {
   public:
    /**
     * @brief Contstructor to initialize display
     */
    Display();

    /**
     * @brief Contstructor to initialize display
     * @param rotation : Rotation of the display
     */
    Display(int rotation);

    /**
     * @brief Function that draw a string with font, in x and y position
     * @param xPos     : X position
     * @param yPos     : Y position
     * @param str      : String to be written
     * @param fontData : Font to be used
     */
    void drawString(int xPos, int yPos, String str,
                    const char *fontData = ArialMT_Plain_16);

    /**
     * @brief Function that draw a string with font, horizontally centered and
     * in y position
     * @param yPos     : Y position
     * @param str      : String to be written
     * @param fontData : Font to be used
     */
    void drawStringHCentered(int yPos, String str,
                             const char *fontData = ArialMT_Plain_16);

    /**
     * @brief Function that fill a rectagle in x and y position width*height
     * @param xPos   : X position
     * @param yPos   : Y position
     * @param width  : Width of the rectangle
     * @param height : Height of the rectangle
     */
    void fillRectangle(int xPos, int yPos, int width, int height);

    /**
     * @brief Function that set color of the pixels (black or white)
     * @param color : Color integer
     */
    void setColor(int color);

    /**
     * @brief Function that draw a width*height xBitmap in x and y position
     * @param xPos    : X position
     * @param yPos    : Y position
     * @param width   : Width of the bitmap
     * @param height  : Height of the bitmap
     * @param xBitmap : xBitmap to draw
     */
    void drawXBitmap(int xPos, int yPos, int width, int height,
                     const char *xBitmap);

    /**
     * @brief Function that commits the draws to display
     */
    void paint();

    /**
     * @brief Function that clear the display
     */
    void clear();

   private:
};

#endif