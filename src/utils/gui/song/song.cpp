#include "song.h"

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
    
    // Title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(songTitle);
    
    // Progress bar
    display.drawRect(0, 14, 128, 4, SSD1306_WHITE);
    display.fillRect(0, 14, 64, 4, SSD1306_WHITE); // 50% progress example
    
    // Artist
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.println(artistName);
    
    // Album (teal color - using white as monochrome approximation)
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 36);
    display.println(albumName);
    
    // Playlist (pink color - using white as monochrome approximation)
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 46);
    display.println(playlistName);
    
    // Playing status
    display.setCursor(0, 57);
    display.print(isPlaying ? "PLAYING" : "PAUSED");
    
    display.display();
}