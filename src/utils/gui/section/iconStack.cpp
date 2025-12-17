#include "iconStack.h"

// // Initialize static members
// IconStack::Ball IconStack::balls[12];
// IconStack::WindLine IconStack::windLines[3];
// bool IconStack::animationInitialized = false;

int IconStack::currentFrame = 0;
unsigned long IconStack::lastFrameTime = 0;

void IconStack::animatePlaylistBitmap(Adafruit_SSD1306 &display) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastFrameTime >= AnimConfig::PLAYLIST_FRAME_DELAY) {
        display.clearDisplay();
        display.drawBitmap(AnimConfig::PLAYLIST_CENTRE_X, AnimConfig::PLAYLIST_CENTRE_Y, playlist_bitmap[currentFrame], AnimConfig::FRAME_WIDTH, AnimConfig::FRAME_HEIGHT, SSD1306_WHITE);
        display.display();
        
        currentFrame = (currentFrame + 1) % AnimConfig::PLAYLIST_FRAME_COUNT;
        lastFrameTime = currentTime;
    }
}

void IconStack::animateAlbumBitmap(Adafruit_SSD1306 &display) {
    // Placeholder for album bitmap animation
}

void IconStack::animateArtistBitmap(Adafruit_SSD1306 &display) {
    // Placeholder for artist bitmap animation
}

void IconStack::animateAllSongsBitmap(Adafruit_SSD1306 &display) {
    // Placeholder for all songs bitmap animation
}