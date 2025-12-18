#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"

GUIManager gui;

// Create screen instances
Song testSong("Still Crazy After all of these Years", 240, "Still Crazy After all of these Years", "Paul Simon", "");
Folder testFolder(FolderType::ALBUMS, "Abbey Road");
Section section;
Category category(CategoryType::PLAYLISTS);

void setup() {
  gui.begin();
  
  // Initialize the navigation stack with the category screen
  // The category screen is the starting screen
  gui.displaySection(&section);
}

void loop() {
  gui.update();  // Non-blocking update with built-in timing
  
  // Handle select button for forward navigation through the hierarchy
  // Navigation flow: Category -> Section -> Folder -> Song
  if (gui.getButtonManager().checkSelectPressed()) {
    size_t depth = gui.getStackDepth();
    
    // Example navigation based on current screen depth
    // Depth 1: Category screen
    // Depth 2: Section screen
    // Depth 3: Folder screen
    // Depth 4: Song screen
    
    if (depth == 1) {
      // From Category, navigate to Section
      Serial.println("Main: Select pressed on Category - navigating to Section");
      gui.displaySection(&section);
    } 
    else if (depth == 2) {
      // From Section, navigate to Folder
      Serial.println("Main: Select pressed on Section - navigating to Folder");
      gui.displayFolder(&testFolder);
    } 
    else if (depth == 3) {
      // From Folder, navigate to Song
      Serial.println("Main: Select pressed on Folder - navigating to Song");
      gui.displaySong(&testSong);
    }
  }
  
  // Handle back button navigation (GPIO32)
  if (gui.getButtonManager().checkBackPressed() && gui.canGoBack()) {
    Serial.println("Main: Back button pressed - navigating back");
    gui.popScreen();
  }
}
