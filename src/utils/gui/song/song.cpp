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
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(songTitle);
    
    // Artist and Album
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print("Artist: ");
    display.println(artistName);
    
    display.setCursor(0, 30);
    display.print("Album: ");
    display.println(albumName);
    
    // Duration and Playlist
    display.setCursor(0, 40);
    display.print("Duration: ");
    display.print(duration);
    display.println("s");
    
    if (playlistName != "") {
        display.setCursor(0, 50);
        display.print("Playlist: ");
        display.println(playlistName);
    }
    
    // Playing status
    display.setCursor(0, 57);
    display.print(isPlaying ? "▶ PLAYING" : "⏸ PAUSED");
    
    display.display();
}