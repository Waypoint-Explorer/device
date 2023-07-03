#ifndef __DISPLAY__
#define __DISPLAY__

#include "Arduino.h"
#include "EPD_WaveShare.h"
#include "EPD_WaveShare_42.h"
#include "MiniGrafx.h"
#include "utility/Logger.h"

#define SCREEN_WIDTH 400.0
#define SCREEN_HEIGHT 300.0
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
    Display();

    /**
     * @brief Contstructor to initialize display
     * @param rotation : Rotation of the display
     */
    Display(int rotation);

    /**
     * @brief Function that draw a string in x and y position
     * @param xPos : X position
     * @param yPos : Y position
     * @param str  : String to be written
     */
    void drawString(int xPos, int yPos, String str);

    /**
     * @brief Function that commits the draws to display
     */
    void paint();

    /**
     * @brief Function that clear the display
     */
    void clear();

    /**
     * @brief Function to get display as string
     * @return String of position
     */
    String toString();

    /**
     * @brief Function for debug display
     */
    void log();

   private:
};

#endif