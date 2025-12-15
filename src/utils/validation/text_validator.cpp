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
    int cursorX = x - scrollOffsets[lineId];
    display.setCursor(cursorX, y);
    display.println(text);
    
    // Debug: Show scroll info for artist (lineId 1)
    if (lineId == 1) {
        display.setCursor(0, 55);
        display.print("S:");
        display.print(scrollOffsets[lineId]);
    }
    
    // Debug: Force artist to scroll for testing
    if (lineId == 1) { // Artist line
        display.setCursor(0, 55);
        display.print("Scroll: ");
        display.print(scrollOffsets[lineId]);
        display.print(" W:");
        display.print(textWidth);
    }
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