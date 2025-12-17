#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "../../utils/gui/song/song.h"
#include "../../utils/gui/folder/folder.h"
#include "../../utils/gui/section/section.h"
#include "../button_manager/button_manager.h"

class GUIManager {
private:
    Adafruit_SSD1306* display;
    ButtonManager buttonManager;
    Folder* currentFolder;
    Section* currentSection;
    
public:
    GUIManager();
    bool begin();
    void clear();
    void update();

    // GUI SCREENS
    void displaySong(Song& song);
    void displayFolder(Folder& folder);
    void displaySection(Section& section);
    void handleFolderInput();
    void handleSectionInput();
};

#endif