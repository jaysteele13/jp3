#include "gui_manager.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include <Wire.h>
#include "../../utils/gui/song/song.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C   // Match the working example

// PINS
#define SDA_PIN 21
#define SCL_PIN 22

GUIManager::GUIManager() {
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

bool GUIManager::begin() {
    Serial.begin(115200);
    delay(500);

    Wire.begin(/*SDA=*/SDA_PIN, /*SCL=*/SCL_PIN);

    if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    } else {
        Serial.println(F("SSD1306 allocation succeeded"));
    }

    // TESTING DISPLAY ONLY
    // display->clearDisplay();
    // display->setTextSize(1);
    // display->setTextColor(SSD1306_WHITE);
    // display->setCursor(0, 0);
    // // PLACEHOLDER TEXT
    // display->println("TEST OLED");
    // display->display();

    return true;
}

void GUIManager::clear() {
    display->clearDisplay();
}

void GUIManager::update() {
    display->display();
}

void GUIManager::setText(int x, int y, const char* text) {
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(x, y);
    display->println(text);
    display->display();  // Needed or text will not appear
}

void GUIManager::showSplashScreen() {
    // Just redisplay whatever is already rendered
    display->display();
    delay(2000);
}

void GUIManager::displaySong(Song& song) {
    song.display(*display);
}
