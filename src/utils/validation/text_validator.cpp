#include "text_validator.h"

void TextValidator::displayTruncatedText(Adafruit_SSD1306 &display, String text, int x, int y, int textSize, int maxWidth) {
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    
    int textWidth = getTextWidth(text, textSize);
    
    if (textWidth <= maxWidth) {
        // Text fits, just display it normally
        display.setCursor(x, y);
        display.println(text);
        return;
    }
    
    // Text needs truncation - calculate max characters that fit
    int charWidth = 6 * textSize;
    int ellipsisWidth = 3 * charWidth; // "..." takes 3 characters
    int maxChars = (maxWidth - ellipsisWidth) / charWidth;
    
    if (maxChars < 1) {
        maxChars = 1; // Show at least 1 character
    }
    
    display.setCursor(x, y);
    display.print(text.substring(0, maxChars));
    display.print("...");
}

bool TextValidator::needsTruncation(String text, int textSize, int maxWidth) {
    return getTextWidth(text, textSize) > maxWidth;
}

int TextValidator::getTextWidth(String text, int textSize) {
    // Approximate character width: 6 pixels for size 1, scaled by textSize
    return text.length() * 6 * textSize;
}