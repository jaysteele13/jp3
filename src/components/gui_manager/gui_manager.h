#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "../../utils/gui/song/song.h"
#include "../../utils/gui/folder/folder.h"
#include "../../utils/gui/section/section.h"
#include "../../utils/gui/category/category.h"
#include "../button_manager/button_manager.h"

enum class ScreenType {
    SONG,
    FOLDER,
    SECTION,
    CATEGORY
};

class GUIManager {
private:
    Adafruit_SSD1306* display;
    ButtonManager buttonManager;
    Folder* currentFolder;
    Section* currentSection;
    Song* currentSong;
    Category* currentCategory;
    ScreenType currentScreenType;
    
    // Non-blocking timing
    unsigned long lastUpdateTime;
    static const unsigned long UPDATE_INTERVAL = 50; // ms
    
    void handleInput();
    void updateDisplay();
    
public:
    GUIManager();
    bool begin();
    void clear();
    void update();

    // GUI SCREENS
    void displaySong(Song& song);
    void displayFolder(Folder& folder);
    void displaySection(Section& section);
    void displayCategory(Category& category);
    
    // Navigation methods
    void navigateToSection();
    void navigateToFolder(Folder& folder);
    void navigateToSong(Song& song);
};

#endif