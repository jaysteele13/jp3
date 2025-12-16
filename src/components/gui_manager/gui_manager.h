#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "../../utils/gui/song/song.h"
#include "../../utils/gui/folder/folder.h"

class GUIManager {
private:
    Adafruit_SSD1306* display;
    
public:
    GUIManager();
    bool begin();
    void clear();
    void update();

    // GUI SCREENS
    void displaySong(Song& song);
    void displayFolder(Folder& folder);
};

#endif