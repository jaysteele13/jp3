#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "../../utils/gui/song/song.h"
#include "../../utils/gui/folder/folder.h"
#include "../../utils/gui/section/section.h"
#include "../../utils/gui/category/category.h"
#include "../../utils/navigation/navigation_controller.h"
#include "../../utils/navigation/navigation_result.h"
#include "../button_manager/button_manager.h"

/**
 * GUIManager orchestrates the display and input for the OLED GUI system.
 * 
 * Uses NavigationController for screen stack management with animations.
 * Handles non-blocking updates with fixed timing interval (50ms).
 * All navigation methods return NavResult for error handling.
 */
class GUIManager {
private:
    Adafruit_SSD1306* display;
    ButtonManager buttonManager;
    NavigationController navigator;
    
    // Non-blocking timing
    unsigned long lastUpdateTime;
    static const unsigned long UPDATE_INTERVAL = 50; // ms
    
public:
    GUIManager();
    bool begin();
    void clear();
    void update();

    // Navigation methods - all return NavResult for error handling
    NavResult pushScreen(ScreenBase* screen, TransitionType animation = TransitionType::INSTANT);
    NavResult popScreen(TransitionType animation = TransitionType::INSTANT);
    
    // Convenience methods for common navigation
    NavResult displayCategory(Category* category);
    NavResult displaySection(Section* section);
    NavResult displayFolder(Folder* folder);
    NavResult displaySong(Song* song);
    
    // Navigation queries
    bool canGoBack() const;
    size_t getStackDepth() const;
    ScreenBase* getCurrentScreen() const;
    
    // Button access for external navigation handling
    ButtonManager& getButtonManager();
};

#endif