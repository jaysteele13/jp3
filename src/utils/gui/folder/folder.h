#ifndef FOLDER_H
#define FOLDER_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

// Enum for folder type
#include "../../enums/enums.h"

// Text Validator for scrolling text
#include "../../validation/text_validator.h"

// Dummy data for albums
#include "../../dummy_data/dummy.h"

// Navigation system
#include "../../navigation/screen_base.h"

class ButtonManager;  // Forward declaration

struct DisplayConfig {
    static const int LINE_HEIGHT = 12;
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int MARGIN_X = 2;
    static const int SELECTION_MARGIN = 4;
    static const int DIVIDER_MARGIN = 2;
    static const int SONG_SPACING = 4;
    static const int INDICATOR_OFFSET = 8;
};

class Folder : public ScreenBase {
    public:
        Folder(FolderType folderType, String folderName);
        SongInfo* loadSongData(int amount);
        void display(Adafruit_SSD1306 &display) override;
        void handleInput(ButtonManager& buttons) override;
        void selectNextSong();
        void selectPreviousSong();
        
        // Set folder data when navigating to this screen
        void setFolderData(FolderType type, String name);
        
        // Get the currently selected song
        SongInfo* getSelectedSong();
        
        // Reset selection and page
        void resetSelection();
        
        ScreenType getScreenType() const override { return ScreenType::FOLDER; }
        String getScreenName() const override { return "Folder: " + folderName; }

    private:
        FolderType folderType;
        String folderName;
        SongInfo* songs;
        int totalSongs;
        int currentPage;
        int selectedSongIndex;
        static const int SONGS_PER_PAGE = 2;
        
        void drawSelectionBox(Adafruit_SSD1306 &display, int x, int y, int width, int height);
        void drawHeader(Adafruit_SSD1306 &display, int &currentY);
        void drawDivider(Adafruit_SSD1306 &display, int &currentY);
        void drawSong(Adafruit_SSD1306 &display, int songIndex, int &currentY, bool isSelected);
};


#endif