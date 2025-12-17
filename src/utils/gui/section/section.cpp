#include "section.h"

Section::Section() : selectedFolderIndex(0) {}

static const int FRAME_WIDTH = 24;
static const int FRAME_HEIGHT = 24;
static const int FRAME_COUNT = 34;
static const int FRAME_DELAY = 100;

void Section::selectNextFolder() {
    if (selectedFolderIndex < FOLDERS_PER_PAGE - 1) {
        selectedFolderIndex++;
    }
}

void Section::selectPreviousFolder() {
    if (selectedFolderIndex > 0) {
        selectedFolderIndex--;
    }
}

void Section::drawSelectionBox(Adafruit_SSD1306 &display, int x, int y, int width, int height) {
    display.drawRect(x, y, width, height, SSD1306_WHITE);
}



void Section::display(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    // Draw stacked icons on the left side
    IconStack::animatePlaylistBitmap(display);

    display.display();
}