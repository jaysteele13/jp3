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
    
    // Split text into segments for scrolling
    String* lines = splitTextIntoLines(text, textSize, maxWidth);
    int totalSegments = getLineCount(lines);
    
    if (totalSegments <= 1) {
        // No meaningful segments, display original text
        display.setCursor(x, y);
        display.println(text);
        return;
    }
    
    // Text needs scrolling - update scroll offset for segment switching
    unsigned long currentTime = millis();
    if (currentTime - lastScrollTime[lineId] > SCROLL_SPEED * 3) { // Slower switching between segments
        scrollOffsets[lineId]++;
        if (scrollOffsets[lineId] >= totalSegments) {
            scrollOffsets[lineId] = 0; // Reset to first segment
        }
        lastScrollTime[lineId] = currentTime;
    }
    
    // Display only the current segment on the same line
    display.setCursor(x, y);
    if (lines[scrollOffsets[lineId]].length() > 0) {
        display.println(lines[scrollOffsets[lineId]]);
    }
}

void TextValidator::resetScrollOffsets() {
    for (int i = 0; i < LINE_SUPPORT_AMOUNT; i++) {
        scrollOffsets[i] = 0;
        lastScrollTime[i] = 0;
    }
}

// Split Text into array of lines if too long for display
String* TextValidator::splitTextIntoLines(String text, int textSize, int maxWidth) {
    static String lines[LINE_SUPPORT_AMOUNT]; // Support up to 10 lines
    // Clear previous contents to avoid stale data in the static array
    for (int i = 0; i < LINE_SUPPORT_AMOUNT; i++) {
        lines[i] = "";
    }
    int lineCount = 0;
    String currentLine = "";
    
    for (unsigned int i = 0; i < text.length(); i++) {
        char c = text.charAt(i);
        currentLine += c;
        
        if (getTextWidth(currentLine, textSize) > maxWidth) {
            // Remove last character and store line
            currentLine.remove(currentLine.length() - 1);
            lines[lineCount++] = currentLine;
            currentLine = String(c); // Start new line with current character
        }
    }
    
    // Add any remaining text as the last line
    if (currentLine.length() > 0 && lineCount < LINE_SUPPORT_AMOUNT) {
        lines[lineCount++] = currentLine;
    }
    
    return lines;
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