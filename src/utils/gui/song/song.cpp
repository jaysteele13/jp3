#include "song.h"
#include "../../validation/text_validator.h"

Song::Song(String title, int duration, String album, String artist, String playlist) :
    songTitle(title),
    duration(duration),
    isPlaying(false),
    albumName(album),
    artistName(artist),
    playlistName(playlist) {}

void Song::play() {
    isPlaying = true;
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
    TextValidator::displayScrollingText(display, songTitle, startX, currentY, 1, 128, 0);
    currentY += lineHeight;
    
    // Progress bar
    display.drawRect(0, currentY, 128, 4, SSD1306_WHITE);
    display.fillRect(0, currentY, 64, 4, SSD1306_WHITE); // 50% progress example
    currentY += 8;
    
    // Artist with scrolling (lineId 1) - force scroll test
    TextValidator::displayScrollingText(display, artistName, startX, currentY, 2, 128, 1);
    currentY += (lineHeight*2);
    
    // Album with scrolling (lineId 2)
    TextValidator::displayScrollingText(display, albumName, startX, currentY, 1, 128, 2);
    currentY += lineHeight;

    // Playlist with scrolling (lineId 3)
    TextValidator::displayScrollingText(display, playlistName, startX, currentY, 1, 128, 3);
    currentY += lineHeight;

    // Playing status
    display.setCursor(startX, currentY);
    display.print(isPlaying ? "PLAYING" : "PAUSED");
    
    display.display();
}