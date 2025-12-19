#ifndef TEXT_VALIDATOR_H
#define TEXT_VALIDATOR_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

class TextValidator {
public:
    static void displayScrollingText(Adafruit_SSD1306 &display, String text, int x, int y, int textSize, int maxWidth = 128, int lineId = 0);
    static void displayPlayIcon(Adafruit_SSD1306 &display, int x, int y);
    static void displayPauseIcon(Adafruit_SSD1306 &display, int x, int y);
    static void resetScrollOffsets();
    static bool needsScrolling(String text, int textSize, int maxWidth = 128);
    static int getTextWidth(String text, int textSize);
    
private:
    static const int LINE_SUPPORT_AMOUNT = 5; // Maximum number of lines supported
    static void drawClippedText(Adafruit_SSD1306 &display,const String &text,int startX, int y,int charWidth,int maxWidth);
    static int scrollOffsets[LINE_SUPPORT_AMOUNT]; // Support up to 10 different scrolling lines
    static unsigned long lastScrollTime[LINE_SUPPORT_AMOUNT]; // Separate timing for each line
    static const int SCROLL_SPEED = 50; // milliseconds between scroll updates
    static const int SCROLL_LOOP_GAP = 40; // pixels of gap between end and start when looping
    
};

#endif