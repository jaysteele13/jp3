#ifndef CATEGORY_H
#define CATEGORY_H

#include "Arduino.h"
#include <optional>
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

// Enum for category type
#include "../../enums/enums.h"

// Include Text validator for scrolling text
#include "../../validation/text_validator.h"

// Include dummy data
#include "../../dummy_data/dummy.h"

// Include small bitmaps
#include "small_bitmaps.h"

// Navigation system
#include "../../navigation/screen_base.h"

class ButtonManager;  // Forward declaration

struct CategoryConfig {
    static const int LINE_HEIGHT = 12;
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int MARGIN_X = 2;
    static const int CENTRE_ALBUM_BITMAP_SPACING = 4;
    static const int DIVIDER_MARGIN = 2;
    static const int FOLDER_SPACING = 2;
    static const int ALBUM_SPACING = 6; // Nicer spacing for albums
    static const int INDICATOR_OFFSET = 2;
    static const int FOLDERS_PER_PAGE = 4; // Default for playlists/artists
    static const int ALBUMS_PER_PAGE = 2;
};



class Category : public ScreenBase {
    public:
        Category(CategoryType categoryType);
        void display(Adafruit_SSD1306 &display) override;
        void handleInput(ButtonManager& buttons) override;
        CategoryInfo* loadCategoryData(int amount);
        void selectNextFolder();
        void selectPreviousFolder();
        String getScreenName() const override { return categoryName; }

    private:
        CategoryType categoryType;
        String categoryName;
        CategoryInfo* categories;
        int totalCategories;
        int currentPage;
        int selectedFolderIndex;
        
        void drawFolder(Adafruit_SSD1306 &display, int folderIndex, int &currentY, bool isSelected);
        void drawSmallBitmap(Adafruit_SSD1306 &display, int x, int y);
        void drawDivider(Adafruit_SSD1306 &display, int &currentY);
        int getFoldersPerPage();
};

#endif