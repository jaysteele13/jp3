#ifndef ICON_STACK_H
#define ICON_STACK_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

// Bitmaps
#include "bitmaps/playlistBitmap.h"

// ANIMATION CONFIG
struct AnimConfig {
    
    static const int PLAYLIST_CENTRE_X = 40;
    static const int PLAYLIST_CENTRE_Y = 8;
    static const int FRAME_WIDTH = 48;
    static const int FRAME_HEIGHT = 48;
    static const int PLAYLIST_FRAME_DELAY = 42;
    static const int PLAYLIST_FRAME_COUNT = (sizeof(playlist_bitmap) / sizeof(playlist_bitmap[0]));
};

class IconStack {
    public:
        static void animatePlaylistBitmap(Adafruit_SSD1306 &display);
        static void animateAlbumBitmap(Adafruit_SSD1306 &display);
        static void animateArtistBitmap(Adafruit_SSD1306 &display);
        static void animateAllSongsBitmap(Adafruit_SSD1306 &display);
    private:
        static int currentFrame;
        static unsigned long lastFrameTime;

};

#endif