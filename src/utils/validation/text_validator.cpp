#include "text_validator.h"

int TextValidator::scrollOffsets[LINE_SUPPORT_AMOUNT] = {0};
unsigned long TextValidator::lastScrollTime[LINE_SUPPORT_AMOUNT] = {0};

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
    
    // True circular scrolling - text wraps from right to left continuously
    unsigned long currentTime = millis();
    if (currentTime - lastScrollTime[lineId] > SCROLL_SPEED) {
        scrollOffsets[lineId]++;
        // Loop after full text width plus gap for seamless circular effect
        if (scrollOffsets[lineId] > textWidth + SCROLL_LOOP_GAP) {
            scrollOffsets[lineId] = 0; // Reset to start
        }
        lastScrollTime[lineId] = currentTime;
    }
    
    int charWidth = 6 * textSize; // Use exact character width from getTextWidth calculation
    
    // Calculate exact positions for both parts of text
    int firstPartPixelOffset = scrollOffsets[lineId] % charWidth;
    int firstCursorX = x - firstPartPixelOffset;
    
    // Calculate how much of the text has scrolled off the left edge
    int scrolledOffWidth = scrollOffsets[lineId];
    int visibleFirstWidth = textWidth - scrolledOffWidth;
    
    // Display first part (the tail of the text)
    if (scrolledOffWidth < textWidth) {
        int firstStartChar = scrollOffsets[lineId] / charWidth;
        int maxFirstChars = (maxWidth / charWidth) + 2; // Buffer for smooth rendering
        int firstEndChar = firstStartChar + maxFirstChars;
        
        if (firstEndChar > text.length()) {
            firstEndChar = text.length();
        }
        
        if (firstStartChar < text.length()) {
            String firstPart = text.substring(firstStartChar, firstEndChar);
            display.setCursor(firstCursorX, y);
            display.print(firstPart);
        }
    }
    
    // Display second part (the head of text that wraps around)
    if (scrolledOffWidth > SCROLL_LOOP_GAP) {
        int availableSpace = maxWidth - visibleFirstWidth - SCROLL_LOOP_GAP;
        if (availableSpace > 0) {
            int secondCursorX = x + visibleFirstWidth + SCROLL_LOOP_GAP;
            int maxSecondChars = availableSpace / charWidth;
            int secondEndChar = maxSecondChars;
            
            if (secondEndChar > text.length()) {
                secondEndChar = text.length();
            }
            
            if (secondEndChar > 0) {
                String secondPart = text.substring(0, secondEndChar);
                display.setCursor(secondCursorX, y);
                display.print(secondPart);
            }
        }
    }
}
void TextValidator::displayPlayIcon(Adafruit_SSD1306 &display, int x, int y) {
    // Simple play icon (triangle)
    display.fillTriangle(x, y, x, y + 10, x + 8, y + 5, SSD1306_WHITE);
}

void TextValidator::displayPauseIcon(Adafruit_SSD1306 &display, int x, int y) {
    // Simple pause icon (two vertical bars)
    display.fillRect(x, y, 2, 10, SSD1306_WHITE);
    display.fillRect(x + 4, y, 2, 10, SSD1306_WHITE);
}

void TextValidator::resetScrollOffsets() {
    for (int i = 0; i < LINE_SUPPORT_AMOUNT; i++) {
        scrollOffsets[i] = 0;
        lastScrollTime[i] = 0;
    }
}


int TextValidator::getTextWidth(String text, int textSize) {
    // Approximate character width: 6 pixels for size 1, scaled by textSize
    return text.length() * 6 * textSize;
}