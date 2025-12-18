#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"

GUIManager gui;
Song testSong("Still Crazy After all of these Years", 240, "Still Crazy After all of these Years", "Paul Simon", "");
Folder testFolder(FolderType::ALBUMS, "50 Ways to Leave Your Lover");
Section section;
Category category(CategoryType::ALBUMS);

void setup() {
  gui.begin();
  // Set initial Screen
  // gui.displaySong(testSong);
  // gui.displayFolder(testFolder);
  // gui.displaySection(section);
  gui.displayCategory(category);  
}

void loop() {
  gui.update();  // Non-blocking update with built-in timing
}
