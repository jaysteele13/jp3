#ifndef SECTION_H
#define SECTION_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
// Enum for folder type
#include "../../enums/enums.h"

// Images for patterns
#include "iconStack.h"

class Section {
    public:
        Section();
        void display(Adafruit_SSD1306 &display);
        bool screenActive;
        void selectNextFolder();
        void selectPreviousFolder();
        void nextPage();
        void previousPage();
    private:
        int selectedFolderIndex;
        int currentPage;
        static const int FOLDERS_PER_PAGE = 4;
        static const int TOTAL_SCREENS = 4;

        void drawSelectionBox(Adafruit_SSD1306 &display, int x, int y, int width, int height);
        void drawCurrentScreen(Adafruit_SSD1306 &display);
};


#endif