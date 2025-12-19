#ifndef SONG_H
#define SONG_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "../../navigation/screen_base.h"

#include "../../enums/enums.h"

class ButtonManager;  // Forward declaration

class Song : public ScreenBase {
public:
    Song(const SongInfo* songInfo);

    void play();
    void stop();
    void pause();
    void resume();
    void display(Adafruit_SSD1306 &display) override;
    void handleInput(ButtonManager& buttons) override;
    ScreenType getScreenType() const override { return ScreenType::SONG; }
    String getScreenName() const override { return songInfo->songName; }
    void setSongData(const SongInfo* songInfo);

private:
    const SongInfo* songInfo;
    bool isPlaying;
   
};

#endif