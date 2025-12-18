#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"

GUIManager gui;

// Create screen instances (globally owned by main)
Song testSong("Still Crazy After all of these Years", 240, "Still Crazy After all of these Years", "Paul Simon", "");
Folder testFolder(FolderType::ALBUMS, "Abbey Road");
Section section;
Category category(CategoryType::PLAYLISTS);

void setup() {
  gui.begin();
  
  // Setup screen cache for navigation
  gui.setScreenCache(&section, &category, &testFolder, &testSong);
  
  // Initialize with section screen (new root)
  NavResult result = gui.displaySection(&section);
  if (result != NavResult::SUCCESS) {
    Serial.print("ERROR: Failed to initialize navigation - ");
    Serial.println(navResultToString(result));
  }
}

void loop() {
  gui.update();  // Handles all input, navigation, and rendering
}
