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
    currentFolder = nullptr;
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
    display->display();
}

void GUIManager::displaySong(Song& song) {
            song.display(*display);
        }

void GUIManager::displayFolder(Folder& folder) {
            currentFolder = &folder;
            folder.display(*display);
        }

// This will need to be made more genric in the future!
void GUIManager::handleFolderInput() {
    if (!currentFolder) return;
    
    if (buttonManager.checkDownPressed()) {
        Serial.println("Handling down button press - selecting next song");
        currentFolder->selectNextSong();
        currentFolder->display(*display);
    }
    
    if (buttonManager.checkUpPressed()) {
        Serial.println("Handling up button press - selecting previous song");
        currentFolder->selectPreviousSong();
        currentFolder->display(*display);
    }
}