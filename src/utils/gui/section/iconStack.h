#ifndef ICON_STACK_H
#define ICON_STACK_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

// Bitmaps
#include "bitmaps/playlistBitmap.h"
#include "bitmaps/artistBitmap.h"
#include "bitmaps/albumBitmap.h"
#include "bitmaps/allSongsBitmap.h"

// ANIMATION CONFIG
struct AnimConfig {
    
    static const int CENTRE_X = 40;
    static const int CENTRE_Y = 8;
    static const int FRAME_WIDTH = 48;
    static const int FRAME_HEIGHT = 48;
    static const int FRAME_DELAY = 42;
    static const int PLAYLIST_FRAME_COUNT = (sizeof(playlist_bitmap) / sizeof(playlist_bitmap[0]));
    static const int ARTIST_FRAME_COUNT = (sizeof(artist_bitmap) / sizeof(artist_bitmap[0]));
    static const int ALBUM_FRAME_COUNT = (sizeof(album_bitmap) / sizeof(album_bitmap[0]));
    static const int ALL_SONGS_FRAME_COUNT = (sizeof(all_songs_bitmap) / sizeof(all_songs_bitmap[0]));
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
        static void animateBitmap(Adafruit_SSD1306 &display, const uint8_t (*bitmap)[288], int frameCount);

};

#endif