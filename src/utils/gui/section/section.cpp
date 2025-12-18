#include "section.h"
#include "../../../components/button_manager/button_manager.h"

void Section::handleInput(ButtonManager& buttons) {
    if (buttons.checkDownPressed()) {
        Serial.println("Section: Down button pressed - navigating to next screen");
        nextPage();
    }
    
    if (buttons.checkUpPressed()) {
        Serial.println("Section: Up button pressed - navigating to previous screen");
        previousPage();
    }
}

Section::Section() : selectedFolderIndex(0), currentPage(0) {}

static const int FRAME_WIDTH = 24;
static const int FRAME_HEIGHT = 24;
static const int FRAME_COUNT = 34;
static const int FRAME_DELAY = 100;


void Section::nextPage() {
    if (currentPage < TOTAL_SCREENS - 1) {
        currentPage++;
    }
}

void Section::previousPage() {
    if (currentPage > 0) {
        currentPage--;
    }
}


void Section::drawCurrentScreen(Adafruit_SSD1306 &display) {
    switch (currentPage) {
        case 0:
            IconStack::animatePlaylistBitmap(display);
            break;
        case 1:
            IconStack::animateAlbumBitmap(display);
            break;
        case 2:
            IconStack::animateArtistBitmap(display);
            break;
        case 3:
            IconStack::animateAllSongsBitmap(display);
            break;
    }
}

void Section::display(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    // Draw stacked icons on the left side
    drawCurrentScreen(display);

    display.display();
}