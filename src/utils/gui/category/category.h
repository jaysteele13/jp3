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
    static const int DIVIDER_MARGIN = 2;
    static const int SONG_SPACING = 4;
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
        bool screenActive;

    private:
        CategoryType categoryType;
        String categoryName;
        CategoryInfo* categories;
        int totalCategories;
        
        void drawHeader(Adafruit_SSD1306 &display, int &currentY);
};

#endif