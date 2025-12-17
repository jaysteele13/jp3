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

GUIManager::GUIManager() : lastUpdateTime(0) {
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    currentFolder = nullptr;
    currentSection = nullptr;
    currentSong = nullptr;
    currentScreenType = ScreenType::SECTION;
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
        handleInput();
        updateDisplay();
        lastUpdateTime = currentTime;
    }
}

void GUIManager::displaySong(Song& song) {
            currentSong = &song;
            currentScreenType = ScreenType::SONG;
            song.display(*display);
        }

void GUIManager::displayFolder(Folder& folder) {
            currentFolder = &folder;
            folder.screenActive = true;
            if (currentSection) currentSection->screenActive = false;
            currentScreenType = ScreenType::FOLDER;
            folder.display(*display);
        }

void GUIManager::displaySection(Section& section) {
            currentSection = &section;
            section.screenActive = true;
            if (currentFolder) currentFolder->screenActive = false;
            currentScreenType = ScreenType::SECTION;
            section.display(*display);
        }

void GUIManager::handleInput() {
    // Debug: Log current screen state
    Serial.print("handleInput called - ScreenType: ");
    Serial.println((int)currentScreenType);
    
    // Button Logic Per Screen
    switch (currentScreenType) {
        case ScreenType::FOLDER:
            Serial.println("In FOLDER case");
            if (!currentFolder || !currentFolder->screenActive) {
                Serial.println("Folder not active - returning");
                return;
            }
            
            if (buttonManager.checkDownPressed()) {
                Serial.println("Handling down button press - selecting next song");
                currentFolder->selectNextSong();
            }
            
            if (buttonManager.checkUpPressed()) {
                Serial.println("Handling up button press - selecting previous song");
                currentFolder->selectPreviousSong();
            }
            break;
            
        case ScreenType::SECTION:
            Serial.println("In SECTION case");
            if (!currentSection || !currentSection->screenActive) {
                Serial.println("Section not active - returning");
                return;
            }
            
            Serial.println("Checking DOWN button...");
            if (buttonManager.checkDownPressed()) {
                Serial.println("Down button pressed - navigating to next screen");
                currentSection->nextPage();
            } else {
                Serial.println("Down button NOT pressed");
            }
            
            Serial.println("Checking UP button...");
            if (buttonManager.checkUpPressed()) {
                Serial.println("Up button pressed - navigating to previous screen");
                currentSection->previousPage();
            } else {
                Serial.println("Up button NOT pressed");
            }
            break;
            
        case ScreenType::SONG:
            Serial.println("In SONG case");
            // Song input handling could be added here
            break;
    }
}

void GUIManager::updateDisplay() {
    switch (currentScreenType) {
        case ScreenType::FOLDER:
            if (currentFolder) {
                currentFolder->display(*display);
            }
            break;
            
        case ScreenType::SECTION:
            if (currentSection) {
                currentSection->display(*display);
            }
            break;
            
        case ScreenType::SONG:
            if (currentSong) {
                currentSong->display(*display);
            }
            break;
    }
}

// Navigation methods
void GUIManager::navigateToSection() {
    currentScreenType = ScreenType::SECTION;
}

void GUIManager::navigateToFolder(Folder& folder) {
    currentFolder = &folder;
    folder.screenActive = true;
    if (currentSection) currentSection->screenActive = false;
    currentScreenType = ScreenType::FOLDER;
    folder.display(*display);
}

void GUIManager::navigateToSong(Song& song) {
    currentSong = &song;
    currentScreenType = ScreenType::SONG;
    song.display(*display);
}