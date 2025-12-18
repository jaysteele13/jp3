#include "gui_manager.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include <Wire.h>
#include "../../utils/navigation/navigation_state.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SDA_PIN 21
#define SCL_PIN 22

GUIManager::GUIManager() 
    : lastUpdateTime(0), navigator(SCREEN_WIDTH, SCREEN_HEIGHT),
      cachedCategory(nullptr), cachedFolder(nullptr), 
      cachedSong(nullptr), cachedSection(nullptr) {
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

bool GUIManager::begin() {
    Serial.begin(115200);
    delay(500);

    Wire.begin(/*SDA=*/SDA_PIN, /*SCL=*/SCL_PIN);
    buttonManager.begin();

    if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    }
    
    Serial.println(F("GUIManager initialized successfully"));
    return true;
}

void GUIManager::clear() {
    display->clearDisplay();
}

void GUIManager::update() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        // Handle input from current screen
        ScreenBase* currentScreen = navigator.current();
        if (currentScreen) {
            currentScreen->handleInput(buttonManager);
        }
        
        // Check for navigation button presses
        handleForwardNavigation();
        handleBackNavigation();
        
        // Update non-rendering state
        navigator.update();
        
        // Render to display
        navigator.render(*display);
        
        lastUpdateTime = currentTime;
    }
}

NavResult GUIManager::pushScreen(ScreenBase* screen, TransitionType animation) {
    if (!screen) {
        Serial.println("ERROR: GUIManager::pushScreen - Null screen!");
        return NavResult::NULL_SCREEN;
    }
    
    NavResult result = navigator.push(screen, animation, 300);
    
    if (result != NavResult::SUCCESS) {
        Serial.print("ERROR: Navigation failed - ");
        Serial.println(navResultToString(result));
    }
    
    return result;
}

NavResult GUIManager::popScreen(TransitionType animation) {
    NavResult result = navigator.pop(animation, 300);
    
    if (result != NavResult::SUCCESS) {
        Serial.print("ERROR: Pop failed - ");
        Serial.println(navResultToString(result));
    }
    
    return result;
}

NavResult GUIManager::displayCategory(Category* category) {
    return pushScreen(category, TransitionType::INSTANT);
}

NavResult GUIManager::displaySection(Section* section) {
    return pushScreen(section, TransitionType::INSTANT);
}

NavResult GUIManager::displayFolder(Folder* folder) {
    return pushScreen(folder, TransitionType::INSTANT);
}

NavResult GUIManager::displaySong(Song* song) {
    return pushScreen(song, TransitionType::INSTANT);
}

bool GUIManager::canGoBack() const {
    return navigator.canGoBack();
}

size_t GUIManager::getStackDepth() const {
    return navigator.getStackDepth();
}

ScreenBase* GUIManager::getCurrentScreen() const {
    return navigator.current();
}

ButtonManager& GUIManager::getButtonManager() {
    return buttonManager;
}

void GUIManager::setScreenCache(Section* section, Category* category, Folder* folder, Song* song) {
    cachedSection = section;
    cachedCategory = category;
    cachedFolder = folder;
    cachedSong = song;
}

void GUIManager::handleForwardNavigation() {
    if (!buttonManager.checkSelectPressed()) {
        return;
    }
    
    ScreenBase* current = navigator.current();
    if (!current) {
        Serial.println("ERROR: No current screen!");
        return;
    }
    
    // Determine current screen type and navigate forward
    ScreenType currentType = NavigationState::getScreenLevel(current);
    ScreenType nextType = NavigationState::getNextLevel(currentType);
    
    if (currentType == nextType) {
        // Already at leaf node (Song), can't go deeper
        Serial.println("INFO: Already at deepest level (Song)");
        return;
    }
    
    // Navigate to appropriate next screen based on current type
    NavResult result = NavResult::INVALID_TRANSITION;
    
    switch (currentType) {
        case ScreenType::SECTION:
            // Section -> Category
            if (cachedCategory) {
                Serial.println("NAV: Section -> Category (Select pressed)");
                result = displayCategory(cachedCategory);
            }
            break;
            
        case ScreenType::CATEGORY:
            // Category -> Folder
            if (cachedFolder) {
                Serial.println("NAV: Category -> Folder (Select pressed)");
                result = displayFolder(cachedFolder);
            }
            break;
            
        case ScreenType::FOLDER:
            // Folder -> Song
            if (cachedSong) {
                Serial.println("NAV: Folder -> Song (Select pressed)");
                result = displaySong(cachedSong);
            }
            break;
            
        case ScreenType::SONG:
            // Can't go deeper
            Serial.println("INFO: Already at Song (deepest level)");
            result = NavResult::SUCCESS;
            break;
            
        default:
            Serial.println("ERROR: Unknown screen type!");
            result = NavResult::INVALID_TRANSITION;
            break;
    }
    
    if (result != NavResult::SUCCESS) {
        Serial.print("ERROR: Forward navigation failed - ");
        Serial.println(navResultToString(result));
    }
}

void GUIManager::handleBackNavigation() {
    if (!buttonManager.checkBackPressed()) {
        return;
    }
    
    if (!canGoBack()) {
        Serial.println("INFO: Already at root screen");
        return;
    }
    
    NavResult result = popScreen();
    if (result != NavResult::SUCCESS) {
        Serial.print("ERROR: Back navigation failed - ");
        Serial.println(navResultToString(result));
    }
}
