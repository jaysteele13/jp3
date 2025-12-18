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

struct CategoryConfig {
    static const int LINE_HEIGHT = 12;
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int MARGIN_X = 2;
    static const int SELECTION_MARGIN = 4;
    static const int DIVIDER_MARGIN = 8;
    static const int FOLDER_SPACING = 4;
    static const int INDICATOR_OFFSET = 8;
};

struct CategoryInfo {
            String categoryName;
            String artistName;
};

class Category {
    public:
        Category(CategoryType categoryType);
        void display(Adafruit_SSD1306 &display);
        CategoryInfo* loadCategoryData(int amount);
        void selectNextFolder();
        void selectPreviousFolder();
        bool screenActive;

    private:
        CategoryType categoryType;
        String categoryName;
        CategoryInfo* categories;
        int totalCategories;
        int currentPage;
        int selectedFolderIndex;
        static const int FOLDERS_PER_PAGE = 4;
        
        void drawHeader(Adafruit_SSD1306 &display, int &currentY);
        void drawDivider(Adafruit_SSD1306 &display, int &currentY);
        void drawFolder(Adafruit_SSD1306 &display, int folderIndex, int &currentY, bool isSelected);
        void drawPointer(Adafruit_SSD1306 &display, int x, int y);
        bool shouldShowHeader();
        int getFoldersPerPage();
        int getHeaderHeight();
        int getStartFolderIndex();
};

#endif