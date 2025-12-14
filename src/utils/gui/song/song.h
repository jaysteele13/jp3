#ifndef SONG_H
#define SONG_H

#include "Arduino.h"

class Song {
public:
    Song(String title, int duration, String album, String artist, String playlist = "");

    void play();
    void stop();
    void pause();
    void resume();

private:
    String songTitle;
    int duration;
    bool isPlaying;
    String albumName;
    String artistName;
    String playlistName;
};