#include "song.h"
#include "../../validation/text_validator.h"
#include "../../../components/button_manager/button_manager.h"

void Song::handleInput(ButtonManager& buttons) {
    // Song screen can have playback controls via buttons
    if (buttons.checkDownPressed()) {
        Serial.println("Song: Down button pressed");
    }
    
    if (buttons.checkUpPressed()) {
        Serial.println("Song: Up button pressed");
    }
}

Song::Song(const SongInfo* info) :
    songInfo(info),
    isPlaying(false) {}

void Song::play() {
    isPlaying = true;
}

void Song::setSongData(const SongInfo* info) {
    songInfo = info;
}

void Song::stop() {
    isPlaying = false;
}

void Song::pause() {
    isPlaying = false;
}

void Song::resume() {
    isPlaying = true;
}

void Song::display(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    //Line Height in the hopes text doesn't overlap
    const int lineHeight = 12;
    const int startX = 0;
    int currentY = 0;
    
    // Don't reset scroll offsets - let them accumulate for smooth scrolling
    
    // Title with scrolling (lineId 0)
    TextValidator::displayScrollingText(display, songInfo->songName, startX, currentY, 1, 0, 128, 0);
    currentY += lineHeight;
    
    // Progress bar
    display.drawRect(0, currentY, 128, 4, SSD1306_WHITE);
    display.fillRect(0, currentY, 64, 4, SSD1306_WHITE); // 50% progress example
    currentY += 8;
    
    // Artist with scrolling (lineId 1) - force scroll test
    TextValidator::displayScrollingText(display, songInfo->artistName, startX, currentY, 2, 0, 128, 1);
    currentY += (lineHeight * 2);
    
    if(isPlaying) {
        TextValidator::displayPlayIcon(display, startX, currentY);
    }
    else {
        TextValidator::displayPauseIcon(display, startX, currentY);
    }
    display.display();
}