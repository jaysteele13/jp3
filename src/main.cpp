#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"
#include "utils/navigation/navigation_state.h"

GUIManager gui;

// Create screen instances (globally owned by main)
Song testSong("Still Crazy After all of these Years", 240, "Still Crazy After all of these Years", "Paul Simon", "");
Folder testFolder(FolderType::ALBUMS, "Abbey Road");
Section section;
Category category(CategoryType::PLAYLISTS);

void setup() {
  gui.begin();
  
  // Initialize with section screen (new root)
  NavResult result = gui.displaySection(&section);
  if (result != NavResult::SUCCESS) {
    Serial.print("ERROR: Failed to initialize navigation - ");
    Serial.println(navResultToString(result));
  }
}

void loop() {
  gui.update();  // Non-blocking update with built-in timing
  
  // Handle select button for forward navigation
  if (gui.getButtonManager().checkSelectPressed()) {
    ScreenBase* current = gui.getCurrentScreen();
    if (!current) {
      Serial.println("ERROR: No current screen!");
      return;
    }
    
    // Determine current screen type and navigate forward
    ScreenType currentType = NavigationState::getScreenLevel(current);
    ScreenType nextType = NavigationState::getNextLevel(currentType);
    
    if (currentType == nextType) {
      // Already at leaf node (Song), can't go deeper
      Serial.println("INFO: Already at deepest level (Song)");
      return;
    }
    
    // Navigate to appropriate next screen based on current type
    NavResult result = NavResult::INVALID_TRANSITION;
    
    switch (currentType) {
      case ScreenType::SECTION:
        // Section -> Category
        Serial.println("NAV: Section -> Category (Select pressed)");
        result = gui.displayCategory(&category);
        break;
        
      case ScreenType::CATEGORY:
        // Category -> Folder
        Serial.println("NAV: Category -> Folder (Select pressed)");
        result = gui.displayFolder(&testFolder);
        break;
        
      case ScreenType::FOLDER:
        // Folder -> Song
        Serial.println("NAV: Folder -> Song (Select pressed)");
        result = gui.displaySong(&testSong);
        break;
        
      case ScreenType::SONG:
        // Can't go deeper
        Serial.println("INFO: Already at Song (deepest level)");
        result = NavResult::SUCCESS;
        break;
        
      default:
        Serial.println("ERROR: Unknown screen type!");
        result = NavResult::INVALID_TRANSITION;
        break;
    }
    
    if (result != NavResult::SUCCESS) {
      Serial.print("ERROR: Forward navigation failed - ");
      Serial.println(navResultToString(result));
    }
  }
  
  // Handle back button navigation
  if (gui.getButtonManager().checkBackPressed()) {
    if (!gui.canGoBack()) {
      Serial.println("INFO: Already at root screen");
      return;
    }
    
    NavResult result = gui.popScreen();
    if (result != NavResult::SUCCESS) {
      Serial.print("ERROR: Back navigation failed - ");
      Serial.println(navResultToString(result));
    }
  }
}
