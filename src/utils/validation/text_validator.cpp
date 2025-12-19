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
    
    // Security: Calculate safe boundaries to prevent overflow
    int maxDisplayX = x + maxWidth;
    int safeMaxChars = (maxWidth / charWidth) + 1; // +1 for safety margin
    int scrollOffset = scrollOffsets[lineId];
    
    // Display first part (the tail of the text)
    if (scrollOffset < textWidth) {
        int firstStartChar = scrollOffset / charWidth;
        int firstEndChar = firstStartChar + safeMaxChars;
        
        // Security: Boundary checks
        if (firstEndChar > text.length()) {
            firstEndChar = text.length();
        }
        if (firstStartChar >= text.length()) {
            firstStartChar = text.length() - 1;
        }
        
        if (firstStartChar < text.length() && firstEndChar > firstStartChar) {
            String firstPart = text.substring(firstStartChar, firstEndChar);
            int firstCursorX = x - (scrollOffset % charWidth);
            
            // Security: Ensure cursor doesn't go negative or beyond bounds
            if (firstCursorX >= x - charWidth && firstCursorX < maxDisplayX) {
                display.setCursor(firstCursorX, y);
                
                // Security: Calculate exact text width and truncate if needed
                int firstPartWidth = firstPart.length() * charWidth;
                int availableWidth = maxDisplayX - firstCursorX;
                if (firstPartWidth > availableWidth) {
                    int truncationPoint = availableWidth / charWidth;
                    if (truncationPoint < firstPart.length()) {
                        firstPart = firstPart.substring(0, truncationPoint);
                    }
                }
                
                display.print(firstPart);
            }
        }
    }
    
    // Display second part (the head of text that wraps around)
    int scrolledOffWidth = scrollOffset;
    int visibleFirstWidth = (textWidth > scrolledOffWidth) ? (textWidth - scrolledOffWidth) : 0;
    
    // Security: Only show second part if enough space with proper gap
    if (scrolledOffWidth > SCROLL_LOOP_GAP && visibleFirstWidth > 0) {
        int availableSpace = maxWidth - visibleFirstWidth - SCROLL_LOOP_GAP;
        
        if (availableSpace > charWidth) { // Need at least one character space
            int secondCursorX = x + visibleFirstWidth + SCROLL_LOOP_GAP;
            
            // Security: Ensure second part starts within display bounds
            if (secondCursorX < maxDisplayX - charWidth) {
                int maxSecondChars = availableSpace / charWidth;
                int secondEndChar = maxSecondChars;
                
                // Security: Boundary checks
                if (secondEndChar > text.length()) {
                    secondEndChar = text.length();
                }
                
                if (secondEndChar > 0) {
                    String secondPart = text.substring(0, secondEndChar);
                    
                    // Security: Final boundary check before display
                    int secondPartWidth = secondPart.length() * charWidth;
                    if (secondCursorX + secondPartWidth <= maxDisplayX) {
                        display.setCursor(secondCursorX, y);
                        display.print(secondPart);
                    }
                }
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