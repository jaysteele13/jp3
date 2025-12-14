#include "text_validator.h"

int TextValidator::scrollOffsets[10] = {0};
unsigned long TextValidator::lastScrollTime[10] = {0};

void TextValidator::displayScrollingText(Adafruit_SSD1306 &display, String text, int x, int y, int textSize, int maxWidth, int lineId) {
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    
    int textWidth = getTextWidth(text, textSize);
    
    if (textWidth <= maxWidth) {
        // Text fits, just display it normally
        display.setCursor(x, y);
        display.println(text);
        return;
    }
    
    // Text needs scrolling - update scroll offset
    unsigned long currentTime = millis();
    if (currentTime - lastScrollTime[lineId] > SCROLL_SPEED) {
        scrollOffsets[lineId]++;
        if (scrollOffsets[lineId] > textWidth + maxWidth) {
            scrollOffsets[lineId] = 0; // Reset to start
        }
        lastScrollTime[lineId] = currentTime;
    }
    
    // Set cursor and display scrolled text
    display.setCursor(x - scrollOffsets[lineId], y);
    display.println(text);
}

void TextValidator::resetScrollOffsets() {
    for (int i = 0; i < 10; i++) {
        scrollOffsets[i] = 0;
        lastScrollTime[i] = 0;
    }
}

bool TextValidator::needsScrolling(String text, int textSize, int maxWidth) {
    return getTextWidth(text, textSize) > maxWidth;
}

int TextValidator::getTextWidth(String text, int textSize) {
    // Approximate character width: 6 pixels for size 1, scaled by textSize
    return text.length() * 6 * textSize;
}