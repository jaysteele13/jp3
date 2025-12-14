#ifndef TEXT_VALIDATOR_H
#define TEXT_VALIDATOR_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

class TextValidator {
public:
    static void displayTruncatedText(Adafruit_SSD1306 &display, String text, int x, int y, int textSize, int maxWidth = 128);
    static bool needsTruncation(String text, int textSize, int maxWidth = 128);
    
private:
    static int getTextWidth(String text, int textSize);
};

#endif