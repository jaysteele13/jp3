#ifndef SECTION_H
#define SECTION_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
// Enum for folder type
#include "../../enums/enums.h"

// Images for patterns
#include "iconStack.h"

// Navigation system
#include "../../navigation/screen_base.h"

class ButtonManager;  // Forward declaration

class Section : public ScreenBase {
    public:
        Section();
        void display(Adafruit_SSD1306 &display) override;
        void handleInput(ButtonManager& buttons) override;
        void nextPage();
        void previousPage();
        
        // Get the currently selected folder type
        FolderType getSelectedFolderType() const;
        
        ScreenType getScreenType() const override { return ScreenType::SECTION; }
        String getScreenName() const override { return "Section"; }
    private:
        int selectedFolderIndex;
        int currentPage;
        static const int TOTAL_SCREENS = 4;
        
        void drawCurrentScreen(Adafruit_SSD1306 &display);
};


#endif