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
  gui.displayCategory(&category);
}

void loop() {
  gui.update();  // Non-blocking update with built-in timing
  
  // Future: Add logic to navigate forward/back via additional buttons
  // Example:
  // if (someBackButtonPressed && gui.canGoBack()) {
  //     gui.popScreen();
  // }
  // if (selectButtonPressed) {
  //     gui.displayFolder(&testFolder);  // Navigate forward
  // }
}
