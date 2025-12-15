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
    
    // Character-by-character scrolling
    unsigned long currentTime = millis();
    if (currentTime - lastScrollTime[lineId] > SCROLL_SPEED) {
        scrollOffsets[lineId]++;
        // Reset when text has completely scrolled past the display
        if (scrollOffsets[lineId] > textWidth + maxWidth) {
            scrollOffsets[lineId] = 0; // Reset to start
        }
        lastScrollTime[lineId] = currentTime;
    }
    
    // Calculate which characters to display based on scroll position
    int charWidth = 7 * textSize; // Approximate character width
    int startChar = scrollOffsets[lineId] / charWidth;
    int visibleChars = maxWidth / charWidth;
    
    // Ensure we don't go beyond text length
    if (startChar >= text.length()) {
        startChar = 0;
        scrollOffsets[lineId] = 0;
    }
    
    // Extract the visible portion of text
    int endChar = startChar + visibleChars + 1;
    if (endChar > text.length()) {
        endChar = text.length();
    }
    String visibleText = text.substring(startChar, endChar);
    
    // Calculate cursor position for smooth scrolling
    int pixelOffset = scrollOffsets[lineId] % charWidth;
    int cursorX = x - pixelOffset;
    
    // Display the visible text
    display.setCursor(cursorX, y);
    display.println(visibleText);
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

// Helper function to count non-empty lines
int TextValidator::getLineCount(String* lines) {
    int count = 0;
    for (int i = 0; i < LINE_SUPPORT_AMOUNT; i++) {
        if (lines[i].length() > 0) {
            count++;
        }
    }
    return count;
}

bool TextValidator::needsScrolling(String text, int textSize, int maxWidth) {
    return getTextWidth(text, textSize) > maxWidth;
}

int TextValidator::getTextWidth(String text, int textSize) {
    // Approximate character width: 6 pixels for size 1, scaled by textSize
    return text.length() * 6 * textSize;
}