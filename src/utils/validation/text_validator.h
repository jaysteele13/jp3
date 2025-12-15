#ifndef TEXT_VALIDATOR_H
#define TEXT_VALIDATOR_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

class TextValidator {
public:
    static void displayScrollingText(Adafruit_SSD1306 &display, String text, int x, int y, int textSize, int maxWidth = 128, int lineId = 0);
    static void resetScrollOffsets();
    static bool needsScrolling(String text, int textSize, int maxWidth = 128);
    
private:
    static int getTextWidth(String text, int textSize);
    static int scrollOffsets[10]; // Support up to 10 different scrolling lines
    static unsigned long lastScrollTime[10]; // Separate timing for each line
    static const int SCROLL_SPEED = 100; // milliseconds between scroll updates
};

#endif