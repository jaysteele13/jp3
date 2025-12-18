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

GUIManager::GUIManager() 
    : lastUpdateTime(0), navigator(SCREEN_WIDTH, SCREEN_HEIGHT) {
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

bool GUIManager::begin() {
    Serial.begin(115200);
    delay(500);

    Wire.begin(/*SDA=*/SDA_PIN, /*SCL=*/SCL_PIN);
    
    // Initialise Button Manager
    buttonManager.begin();

    if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    } else {
        Serial.println(F("SSD1306 allocation succeeded"));
    }

    return true;
}

void GUIManager::clear() {
    display->clearDisplay();
}

void GUIManager::update() {
    // Adding in timing to add unpredictable delays and a controlled refresh rate
    // also bad to keep cpu at 100% usage all the time
    unsigned long currentTime = millis();
    
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        // Handle input from current screen
        ScreenBase* currentScreen = navigator.current();
        if (currentScreen) {
            currentScreen->handleInput(buttonManager);
        }
        
        // Update non-rendering state
        navigator.update();
        
        // Render to display (handles animations automatically)
        navigator.render(*display);
        
        lastUpdateTime = currentTime;
    }
}

void GUIManager::pushScreen(ScreenBase* screen, TransitionType animation) {
    if (screen) {
        navigator.push(screen, animation, 300);  // 300ms animation duration
    }
}

void GUIManager::popScreen(TransitionType animation) {
    navigator.pop(animation, 300);  // 300ms animation duration
}

void GUIManager::displayCategory(Category* category) {
    if (category) {
        pushScreen(category, TransitionType::SLIDE_LEFT);
    }
}

void GUIManager::displaySection(Section* section) {
    if (section) {
        pushScreen(section, TransitionType::SLIDE_LEFT);
    }
}

void GUIManager::displayFolder(Folder* folder) {
    if (folder) {
        pushScreen(folder, TransitionType::SLIDE_LEFT);
    }
}

void GUIManager::displaySong(Song* song) {
    if (song) {
        pushScreen(song, TransitionType::SLIDE_LEFT);
    }
}

bool GUIManager::canGoBack() const {
    return navigator.canGoBack();
}

size_t GUIManager::getStackDepth() const {
    return navigator.getStackDepth();
}

ButtonManager& GUIManager::getButtonManager() {
    return buttonManager;
}