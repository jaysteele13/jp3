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
    
    // Title with scrolling (lineId 0)
    TextValidator::displayScrollingText(display, songTitle, 0, 0, 1, 128, 0);
    
    // Progress bar
    display.drawRect(0, 14, 128, 4, SSD1306_WHITE);
    display.fillRect(0, 14, 64, 4, SSD1306_WHITE); // 50% progress example
    
    // Artist with scrolling (lineId 1)
    TextValidator::displayScrollingText(display, artistName, 0, 20, 2, 128, 1);
    
    // Album with scrolling (lineId 2)
    TextValidator::displayScrollingText(display, albumName, 0, 36, 1, 128, 2);
    
    // Playlist with scrolling (lineId 3)
    TextValidator::displayScrollingText(display, playlistName, 0, 46, 1, 128, 3);
    
    // Playing status
    display.setCursor(0, 57);
    display.print(isPlaying ? "PLAYING" : "PAUSED");
    
    display.display();
}