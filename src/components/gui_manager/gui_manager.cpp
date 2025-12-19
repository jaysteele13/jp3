#include "gui_manager.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

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

NavResult GUIManager::pushScreen(ScreenBase* screen) {
    if (!screen) {
        Serial.println("ERROR: GUIManager::pushScreen - Null screen!");
        return NavResult::NULL_SCREEN;
    }
    
    NavResult result = navigator.push(screen);
    
    if (result != NavResult::SUCCESS) {
        Serial.print("ERROR: Navigation failed - ");
        Serial.println(navResultToString(result));
    }
    
    return result;
}

NavResult GUIManager::popScreen() {
    NavResult result = navigator.pop();
    
    if (result != NavResult::SUCCESS) {
        Serial.print("ERROR: Pop failed - ");
        Serial.println(navResultToString(result));
    }
    
    return result;
}

NavResult GUIManager::displayCategory(Category* category) {
    return pushScreen(category);
}

NavResult GUIManager::displaySection(Section* section) {
    return pushScreen(section);
}

NavResult GUIManager::displayFolder(Folder* folder) {
    return pushScreen(folder);
}

NavResult GUIManager::displaySong(Song* song) {
    return pushScreen(song);
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
    NavResult result = NavResult::INVALID_PARAMETERS;
    
    switch (currentType) {
        case ScreenType::SECTION: {
            // Section -> Category (HAS TO be folder type as we need to include All Songs which isn't a category type)
            Section* section = static_cast<Section*>(current);
            FolderType folderType = section->getSelectedFolderType();
            
            auto category = ScreenFactory::createCategory(folderType);
            Serial.print("NAV: Section -> Category (");
            Serial.print((int)Utils::folderTypeToCategoryType(folderType));
            Serial.println(")");
            
            result = pushScreen(category);
            break;
        }
            
        case ScreenType::CATEGORY: {
            // Category -> Folder
            Category* category = static_cast<Category*>(current);
            CategoryInfo* selected = category->getSelectedCategory();
            
            if (selected) {
                CategoryType catType = category->getCategoryType();
                FolderType folderType = Utils::categoryTypeToFolderType(catType);
                
                auto folder = ScreenFactory::createFolder(folderType, selected->categoryName);
                Serial.print("NAV: Category -> Folder (");
                Serial.print(selected->categoryName);
                Serial.println(")");
                
                result = pushScreen(folder);
            }
            break;
        }
            
        case ScreenType::FOLDER: {
            // Folder -> Song
            Folder* folder = static_cast<Folder*>(current);
            SongInfo* selected = folder->getSelectedSong();
            
            if (selected) {
                auto song = ScreenFactory::createSong(selected);
                Serial.println("NAV: Folder -> Song");
                result = pushScreen(song);
            }
            break;
        }
            
        case ScreenType::SONG:
            // Can't go deeper
            Serial.println("INFO: Already at Song (deepest level)");
            result = NavResult::SUCCESS;
            break;
            
        default:
            Serial.println("ERROR: Unknown screen type!");
            result = NavResult::INVALID_PARAMETERS;
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
