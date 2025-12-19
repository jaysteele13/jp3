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
    
    // True infinite circular scrolling - text wraps from right to left continuously without reset
    unsigned long currentTime = millis();
    if (currentTime - lastScrollTime[lineId] > SCROLL_SPEED) {
        scrollOffsets[lineId]++;
        // No reset - let offset grow infinitely, modulo operations will handle circular behavior
        lastScrollTime[lineId] = currentTime;
    }
    
    int charWidth = 6 * textSize; // Use exact character width from getTextWidth calculation
    
    // Security: Calculate safe boundaries to prevent overflow
    int maxDisplayX = x + maxWidth;
    int safeMaxChars = (maxWidth / charWidth) + 1; // +1 for safety margin
    int scrollOffset = scrollOffsets[lineId];
    
    // Use cycle position for consistent infinite loop
    int cyclePosition = scrollOffset % (textWidth + SCROLL_LOOP_GAP);
    int effectiveScrollOffset = cyclePosition;
    
    // Display first part (the tail of the text)
    if (effectiveScrollOffset < textWidth) {
        int firstStartChar = effectiveScrollOffset / charWidth;
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
            int firstCursorX = x - (effectiveScrollOffset % charWidth);
            
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
    
    // Always show second part for continuous infinite loop
    int effectiveScrolledOffWidth = effectiveScrollOffset;
    int effectiveVisibleFirstWidth = (textWidth > effectiveScrolledOffWidth) ? (textWidth - effectiveScrolledOffWidth) : 0;
    
    if (effectiveScrolledOffWidth > SCROLL_LOOP_GAP) {
        int availableSpace = maxWidth - effectiveVisibleFirstWidth - SCROLL_LOOP_GAP;
        
        if (availableSpace > 0) { // Always try to show second part, even if small
            int secondCursorX = x + effectiveVisibleFirstWidth + SCROLL_LOOP_GAP;
            
            // Show as much as can fit in the remaining space
            int maxSecondChars = availableSpace / charWidth;
            if (maxSecondChars < 1) maxSecondChars = 1; // Show at least one character
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
                } else {
                    // Truncate to fit exactly
                    int fittingChars = (maxDisplayX - secondCursorX) / charWidth;
                    if (fittingChars > 0 && fittingChars <= secondPart.length()) {
                        String truncatedPart = secondPart.substring(0, fittingChars);
                        display.setCursor(secondCursorX, y);
                        display.print(truncatedPart);
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