#ifndef SONG_H
#define SONG_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

class Song {
public:
    Song(String title, int duration, String album, String artist, String playlist = "");

    void play();
    void stop();
    void pause();
    void resume();
    void display(Adafruit_SSD1306 &display);

private:
    String songTitle;
    int duration;
    bool isPlaying;
    String albumName;
    String artistName;
    String playlistName;
};