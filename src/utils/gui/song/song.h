#ifndef SONG_H
#define SONG_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "../../navigation/screen_base.h"

class ButtonManager;  // Forward declaration

class Song : public ScreenBase {
public:
    Song(String title, int duration, String album, String artist, String playlist = "");

    void play();
    void stop();
    void pause();
    void resume();
    void display(Adafruit_SSD1306 &display) override;
    void handleInput(ButtonManager& buttons) override;
    ScreenType getScreenType() const override { return ScreenType::SONG; }
    String getScreenName() const override { return songTitle; }
    SongInfo setSongData(SongInfo songInfo);

private:
    SongInfo songInfo;
    bool isPlaying;
   
};

#endif