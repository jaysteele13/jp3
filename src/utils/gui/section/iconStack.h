#ifndef ICON_STACK_H
#define ICON_STACK_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

class IconStack {
    public:
        static void drawPlaylistStack(Adafruit_SSD1306 &display) {
            //id: 0 pixel 9 
            display.drawPixel(54, 44, SSD1306_WHITE);
            display.drawPixel(54, 45, SSD1306_WHITE);
            display.drawPixel(54, 46, SSD1306_WHITE);
            display.drawPixel(54, 47, SSD1306_WHITE);
            display.drawPixel(54, 48, SSD1306_WHITE);
            display.drawPixel(55, 48, SSD1306_WHITE);
            display.drawPixel(55, 49, SSD1306_WHITE);
            display.drawPixel(55, 50, SSD1306_WHITE);
            display.drawPixel(55, 51, SSD1306_WHITE);
            display.drawPixel(55, 52, SSD1306_WHITE);
            display.drawPixel(55, 53, SSD1306_WHITE);
            display.drawPixel(55, 54, SSD1306_WHITE);
            display.drawPixel(55, 55, SSD1306_WHITE);
            display.drawPixel(56, 55, SSD1306_WHITE);
            display.drawPixel(56, 56, SSD1306_WHITE);
            display.drawPixel(56, 57, SSD1306_WHITE);
            display.drawPixel(56, 58, SSD1306_WHITE);
            display.drawPixel(57, 58, SSD1306_WHITE);
            display.drawPixel(57, 59, SSD1306_WHITE);
            display.drawPixel(57, 60, SSD1306_WHITE);
            display.drawPixel(57, 61, SSD1306_WHITE);
            display.drawPixel(58, 61, SSD1306_WHITE);
            display.drawPixel(59, 61, SSD1306_WHITE);
            display.drawPixel(60, 61, SSD1306_WHITE);
            display.drawPixel(61, 61, SSD1306_WHITE);
            display.drawPixel(61, 60, SSD1306_WHITE);
            display.drawPixel(62, 60, SSD1306_WHITE);
            display.drawPixel(62, 59, SSD1306_WHITE);
            display.drawPixel(62, 58, SSD1306_WHITE);
            display.drawPixel(62, 57, SSD1306_WHITE);
            display.drawPixel(62, 56, SSD1306_WHITE);
            display.drawPixel(62, 55, SSD1306_WHITE);
            display.drawPixel(62, 54, SSD1306_WHITE);
            display.drawPixel(62, 53, SSD1306_WHITE);
            display.drawPixel(61, 53, SSD1306_WHITE);
            display.drawPixel(61, 52, SSD1306_WHITE);
            display.drawPixel(61, 51, SSD1306_WHITE);
            display.drawPixel(61, 50, SSD1306_WHITE);
            display.drawPixel(61, 49, SSD1306_WHITE);
            display.drawPixel(60, 48, SSD1306_WHITE);
            display.drawPixel(60, 47, SSD1306_WHITE);
            display.drawPixel(60, 46, SSD1306_WHITE);
            display.drawPixel(59, 45, SSD1306_WHITE);
            display.drawPixel(59, 44, SSD1306_WHITE);
            display.drawPixel(59, 43, SSD1306_WHITE);
            display.drawPixel(58, 42, SSD1306_WHITE);
            display.drawPixel(58, 41, SSD1306_WHITE);
            display.drawPixel(58, 40, SSD1306_WHITE);
            display.drawPixel(57, 40, SSD1306_WHITE);
            display.drawPixel(57, 39, SSD1306_WHITE);
            //id: 1 pixel 10 
            display.drawPixel(55, 48, SSD1306_WHITE);
            display.drawPixel(56, 48, SSD1306_WHITE);
            display.drawPixel(57, 48, SSD1306_WHITE);
            display.drawPixel(57, 47, SSD1306_WHITE);
            display.drawPixel(58, 47, SSD1306_WHITE);
            //id: 2 pixel 11 
            display.drawPixel(56, 53, SSD1306_WHITE);
            display.drawPixel(57, 53, SSD1306_WHITE);
            display.drawPixel(58, 52, SSD1306_WHITE);
            display.drawPixel(59, 51, SSD1306_WHITE);
            display.drawPixel(60, 51, SSD1306_WHITE);
            display.drawPixel(61, 50, SSD1306_WHITE);
            //id: 3 pixel 12 
            display.drawPixel(56, 57, SSD1306_WHITE);
            display.drawPixel(57, 57, SSD1306_WHITE);
            display.drawPixel(58, 56, SSD1306_WHITE);
            display.drawPixel(60, 56, SSD1306_WHITE);
            display.drawPixel(60, 55, SSD1306_WHITE);
            display.drawPixel(61, 55, SSD1306_WHITE);
            //id: 4 pixel 13 
            display.drawPixel(57, 39, SSD1306_WHITE);
            display.drawPixel(57, 38, SSD1306_WHITE);
            display.drawPixel(57, 37, SSD1306_WHITE);
            display.drawPixel(57, 36, SSD1306_WHITE);
            display.drawPixel(58, 36, SSD1306_WHITE);
            display.drawPixel(58, 35, SSD1306_WHITE);
            display.drawPixel(58, 34, SSD1306_WHITE);
            display.drawPixel(59, 33, SSD1306_WHITE);
            display.drawPixel(59, 32, SSD1306_WHITE);
            display.drawPixel(59, 31, SSD1306_WHITE);
            display.drawPixel(59, 30, SSD1306_WHITE);
            display.drawPixel(60, 29, SSD1306_WHITE);
            display.drawPixel(60, 28, SSD1306_WHITE);
            display.drawPixel(59, 28, SSD1306_WHITE);
            display.drawPixel(58, 28, SSD1306_WHITE);
            display.drawPixel(57, 28, SSD1306_WHITE);
            display.drawPixel(56, 28, SSD1306_WHITE);
            display.drawPixel(55, 29, SSD1306_WHITE);
            display.drawPixel(54, 29, SSD1306_WHITE);
            display.drawPixel(53, 29, SSD1306_WHITE);
            display.drawPixel(52, 29, SSD1306_WHITE);
            display.drawPixel(52, 30, SSD1306_WHITE);
            display.drawPixel(51, 30, SSD1306_WHITE);
            display.drawPixel(50, 30, SSD1306_WHITE);
            display.drawPixel(49, 31, SSD1306_WHITE);
            display.drawPixel(48, 31, SSD1306_WHITE);
            display.drawPixel(47, 31, SSD1306_WHITE);
            display.drawPixel(47, 32, SSD1306_WHITE);
            display.drawPixel(46, 33, SSD1306_WHITE);
            display.drawPixel(45, 34, SSD1306_WHITE);
            display.drawPixel(45, 35, SSD1306_WHITE);
            display.drawPixel(45, 36, SSD1306_WHITE);
            display.drawPixel(45, 37, SSD1306_WHITE);
            display.drawPixel(45, 38, SSD1306_WHITE);
            display.drawPixel(46, 38, SSD1306_WHITE);
            display.drawPixel(47, 38, SSD1306_WHITE);
            display.drawPixel(48, 38, SSD1306_WHITE);
            display.drawPixel(49, 38, SSD1306_WHITE);
            display.drawPixel(49, 39, SSD1306_WHITE);
            display.drawPixel(50, 39, SSD1306_WHITE);
            display.drawPixel(50, 40, SSD1306_WHITE);
            display.drawPixel(51, 40, SSD1306_WHITE);
            display.drawPixel(52, 41, SSD1306_WHITE);
            display.drawPixel(52, 42, SSD1306_WHITE);
            display.drawPixel(53, 43, SSD1306_WHITE);
            display.drawPixel(53, 44, SSD1306_WHITE);
            display.drawPixel(54, 44, SSD1306_WHITE);
            display.drawPixel(55, 43, SSD1306_WHITE);
            display.drawPixel(55, 42, SSD1306_WHITE);
            display.drawPixel(56, 41, SSD1306_WHITE);
            display.drawPixel(56, 40, SSD1306_WHITE);
            display.drawPixel(57, 40, SSD1306_WHITE);
            //id: 5 pixel 16 
            display.drawPixel(47, 37, SSD1306_WHITE);
            display.drawPixel(46, 37, SSD1306_WHITE);
            display.drawPixel(45, 37, SSD1306_WHITE);
            display.drawPixel(44, 37, SSD1306_WHITE);
            display.drawPixel(43, 37, SSD1306_WHITE);
            display.drawPixel(42, 36, SSD1306_WHITE);
            display.drawPixel(41, 36, SSD1306_WHITE);
            display.drawPixel(39, 36, SSD1306_WHITE);
            display.drawPixel(37, 36, SSD1306_WHITE);
            display.drawPixel(36, 36, SSD1306_WHITE);
            display.drawPixel(35, 35, SSD1306_WHITE);
            display.drawPixel(34, 35, SSD1306_WHITE);
            display.drawPixel(33, 35, SSD1306_WHITE);
            display.drawPixel(32, 35, SSD1306_WHITE);
            display.drawPixel(30, 35, SSD1306_WHITE);
            display.drawPixel(29, 34, SSD1306_WHITE);
            display.drawPixel(28, 34, SSD1306_WHITE);
            display.drawPixel(27, 33, SSD1306_WHITE);
            display.drawPixel(26, 33, SSD1306_WHITE);
            display.drawPixel(25, 33, SSD1306_WHITE);
            display.drawPixel(22, 32, SSD1306_WHITE);
            display.drawPixel(21, 32, SSD1306_WHITE);
            display.drawPixel(21, 31, SSD1306_WHITE);
            display.drawPixel(20, 31, SSD1306_WHITE);
            display.drawPixel(19, 30, SSD1306_WHITE);
            display.drawPixel(18, 29, SSD1306_WHITE);
            display.drawPixel(18, 28, SSD1306_WHITE);
            display.drawPixel(17, 27, SSD1306_WHITE);
            display.drawPixel(17, 26, SSD1306_WHITE);
            display.drawPixel(16, 26, SSD1306_WHITE);
            display.drawPixel(16, 25, SSD1306_WHITE);
            display.drawPixel(16, 24, SSD1306_WHITE);
            display.drawPixel(15, 24, SSD1306_WHITE);
            display.drawPixel(15, 23, SSD1306_WHITE);
            display.drawPixel(15, 22, SSD1306_WHITE);
            display.drawPixel(14, 22, SSD1306_WHITE);
            display.drawPixel(14, 21, SSD1306_WHITE);
            display.drawPixel(14, 20, SSD1306_WHITE);
            display.drawPixel(14, 19, SSD1306_WHITE);
            display.drawPixel(14, 18, SSD1306_WHITE);
            display.drawPixel(13, 17, SSD1306_WHITE);
            display.drawPixel(13, 16, SSD1306_WHITE);
            display.drawPixel(13, 15, SSD1306_WHITE);
            display.drawPixel(13, 14, SSD1306_WHITE);
            display.drawPixel(13, 12, SSD1306_WHITE);
            display.drawPixel(13, 10, SSD1306_WHITE);
            display.drawPixel(13, 9, SSD1306_WHITE);
            display.drawPixel(13, 8, SSD1306_WHITE);
            display.drawPixel(13, 7, SSD1306_WHITE);
            display.drawPixel(13, 6, SSD1306_WHITE);
            display.drawPixel(13, 5, SSD1306_WHITE);
            display.drawPixel(13, 4, SSD1306_WHITE);
            display.drawPixel(13, 3, SSD1306_WHITE);
            display.drawPixel(13, 2, SSD1306_WHITE);
            display.drawPixel(13, 1, SSD1306_WHITE);
            display.drawPixel(61, 26, SSD1306_WHITE);
            display.drawPixel(61, 25, SSD1306_WHITE);
            display.drawPixel(61, 23, SSD1306_WHITE);
            display.drawPixel(61, 22, SSD1306_WHITE);
            display.drawPixel(61, 20, SSD1306_WHITE);
            display.drawPixel(61, 19, SSD1306_WHITE);
            display.drawPixel(60, 17, SSD1306_WHITE);
            display.drawPixel(60, 16, SSD1306_WHITE);
            display.drawPixel(59, 15, SSD1306_WHITE);
            display.drawPixel(59, 14, SSD1306_WHITE);
            display.drawPixel(57, 13, SSD1306_WHITE);
            display.drawPixel(56, 11, SSD1306_WHITE);
            display.drawPixel(56, 10, SSD1306_WHITE);
            display.drawPixel(55, 9, SSD1306_WHITE);
            display.drawPixel(53, 7, SSD1306_WHITE);
            display.drawPixel(51, 6, SSD1306_WHITE);
            display.drawPixel(51, 5, SSD1306_WHITE);
            display.drawPixel(50, 5, SSD1306_WHITE);
            display.drawPixel(49, 4, SSD1306_WHITE);
            display.drawPixel(48, 4, SSD1306_WHITE);
            display.drawPixel(47, 3, SSD1306_WHITE);
            display.drawPixel(46, 3, SSD1306_WHITE);
            display.drawPixel(45, 3, SSD1306_WHITE);
            display.drawPixel(43, 3, SSD1306_WHITE);
            display.drawPixel(42, 2, SSD1306_WHITE);
            display.drawPixel(41, 2, SSD1306_WHITE);
            display.drawPixel(40, 2, SSD1306_WHITE);
            display.drawPixel(39, 1, SSD1306_WHITE);
            display.drawPixel(38, 1, SSD1306_WHITE);
            display.drawPixel(37, 1, SSD1306_WHITE);
            display.drawPixel(36, 1, SSD1306_WHITE);
            display.drawPixel(35, 0, SSD1306_WHITE);
            display.drawPixel(34, 0, SSD1306_WHITE);
            display.drawPixel(32, 0, SSD1306_WHITE);
            display.drawPixel(31, 0, SSD1306_WHITE);
            display.drawPixel(13, 0, SSD1306_WHITE);
            display.drawPixel(12, 0, SSD1306_WHITE);
            //id: 6 pixel 17 
            display.drawPixel(13, 2, SSD1306_WHITE);
            display.drawPixel(14, 2, SSD1306_WHITE);
            display.drawPixel(15, 2, SSD1306_WHITE);
            display.drawPixel(16, 1, SSD1306_WHITE);
            display.drawPixel(17, 1, SSD1306_WHITE);
            display.drawPixel(18, 1, SSD1306_WHITE);
            display.drawPixel(19, 1, SSD1306_WHITE);
            display.drawPixel(20, 0, SSD1306_WHITE);
            display.drawPixel(21, 0, SSD1306_WHITE);
            display.drawPixel(22, 0, SSD1306_WHITE);
            display.drawPixel(22, 0, SSD1306_WHITE);
            display.drawPixel(23, 0, SSD1306_WHITE);
            display.drawPixel(29, 0, SSD1306_WHITE);
            display.drawPixel(30, 0, SSD1306_WHITE);
            display.drawPixel(31, 0, SSD1306_WHITE);
            display.drawPixel(32, 0, SSD1306_WHITE);
            //id: 7 pixel 18 
            display.drawPixel(25, 0, SSD1306_WHITE);
            display.drawPixel(26, 0, SSD1306_WHITE);
            display.drawPixel(27, 0, SSD1306_WHITE);
            display.drawPixel(28, 0, SSD1306_WHITE);
            display.drawPixel(25, 1, SSD1306_WHITE);
            display.drawPixel(24, 1, SSD1306_WHITE);
            display.drawPixel(24, 0, SSD1306_WHITE);
            //id: 8 circle 19 
            display.drawCircle(16, 9, 1, SSD1306_WHITE);
            //id: 9 circle 20 
            display.drawCircle(24, 5, 1, SSD1306_WHITE);
            //id: 10 circle 21 
            display.drawCircle(34, 4, 1, SSD1306_WHITE);
            //id: 11 circle 22 
            display.drawCircle(21, 13, 2, SSD1306_WHITE);
            //id: 12 circle 23 
            display.drawCircle(31, 11, 2, SSD1306_WHITE);
            //id: 13 circle 24 
            display.drawCircle(42, 9, 2, SSD1306_WHITE);
            //id: 14 circle 25 
            display.drawCircle(24, 21, 2, SSD1306_WHITE);
            //id: 15 circle 26 
            display.drawCircle(33, 18, 3, SSD1306_WHITE);
            //id: 16 circle 27 
            display.drawCircle(43, 15, 2, SSD1306_WHITE);
            //id: 17 circle 28 
            display.drawCircle(31, 28, 1, SSD1306_WHITE);
            //id: 18 circle 29 
            display.drawCircle(39, 23, 2, SSD1306_WHITE);
            //id: 19 circle 30 
            display.drawCircle(48, 22, 2, SSD1306_WHITE);
            //id: 20 circle 31 
            display.drawCircle(54, 17, 2, SSD1306_WHITE);
            //id: 21 circle 32 
            display.drawCircle(100, 18, 7, SSD1306_WHITE);
            //id: 22 line 33 
            display.drawLine(81, 11, 90, 10, SSD1306_WHITE);
            //id: 23 line 34 
            display.drawLine(86, 18, 89, 18, SSD1306_WHITE);
            //id: 24 line 35 
            display.drawLine(87, 24, 91, 23, SSD1306_WHITE);
        }

        static void drawAlbumStack(Adafruit_SSD1306 &display) {}
        static void drawArtistStack(Adafruit_SSD1306 &display) {}
        static void drawAllSongsStack(Adafruit_SSD1306 &display) {}
};



#endif