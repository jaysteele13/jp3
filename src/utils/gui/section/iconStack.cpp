#include "iconStack.h"

int IconStack::currentFrame = 0;
unsigned long IconStack::lastFrameTime = 0;

// Generic bitmap animation helper
void IconStack::animateBitmap(Adafruit_SSD1306 &display, const uint8_t (*bitmap)[288], int frameCount) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastFrameTime >= AnimConfig::FRAME_DELAY) {
        display.clearDisplay();
        display.drawBitmap(AnimConfig::CENTRE_X, AnimConfig::CENTRE_Y, bitmap[currentFrame], AnimConfig::FRAME_WIDTH, AnimConfig::FRAME_HEIGHT, SSD1306_WHITE);
        display.display();
        
        currentFrame = (currentFrame + 1) % frameCount;
        lastFrameTime = currentTime;
    }
}

void IconStack::animatePlaylistBitmap(Adafruit_SSD1306 &display) {
    animateBitmap(display, playlist_bitmap, AnimConfig::PLAYLIST_FRAME_COUNT);
}

void IconStack::animateAlbumBitmap(Adafruit_SSD1306 &display) {
    animateBitmap(display, album_bitmap, AnimConfig::ALBUM_FRAME_COUNT);
}

void IconStack::animateArtistBitmap(Adafruit_SSD1306 &display) {
    animateBitmap(display, artist_bitmap, AnimConfig::ARTIST_FRAME_COUNT);
}

void IconStack::animateAllSongsBitmap(Adafruit_SSD1306 &display) {
    animateBitmap(display, all_songs_bitmap, AnimConfig::ALL_SONGS_FRAME_COUNT);
}