#include "text_validator.h"

int TextValidator::scrollOffsets[LINE_SUPPORT_AMOUNT] = {0};
unsigned long TextValidator::lastScrollTime[LINE_SUPPORT_AMOUNT] = {0};

void TextValidator::drawClippedText(
    Adafruit_SSD1306 &display,
    const String &text,
    int startX,
    int y,
    int charWidth,
    int maxWidth
) {
    int firstChar = 0;
    if (startX < 0) {
        firstChar = (-startX) / charWidth;
        startX += firstChar * charWidth;
    }

    int maxChars = (maxWidth - (startX - 0)) / charWidth;
    if (maxChars <= 0 || firstChar >= text.length()) return;

    int endChar = firstChar + maxChars;
    if (endChar > text.length()) endChar = text.length();

    display.setCursor(startX, y);
    display.print(text.substring(firstChar, endChar));
}


void TextValidator::displayScrollingText(Adafruit_SSD1306 &display, String text, int x, int y, int textSize, int maxWidth, int lineId) {
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);

    const int charWidth = 6 * textSize;
    const int textWidth = text.length() * charWidth;

    if (textWidth <= maxWidth) {
        display.setCursor(x, y);
        display.print(text);
        return;
    }

    unsigned long now = millis();
    if (now - lastScrollTime[lineId] >= SCROLL_SPEED) {
        scrollOffsets[lineId]++;
        lastScrollTime[lineId] = now;
    }

    const int totalWidth = textWidth + SCROLL_LOOP_GAP;
    int offset = scrollOffsets[lineId] % totalWidth;

    // Draw TWO clipped passes
    drawClippedText(display, text, x - offset, y, charWidth, maxWidth);
    drawClippedText(display, text, x - offset + totalWidth, y, charWidth, maxWidth);
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