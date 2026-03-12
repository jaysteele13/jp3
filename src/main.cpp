#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"
#include "components/file_manager/file_manager.h"
#include "components/metadata_manager/metadata_manager.h"

GUIManager gui;
Section section;
FileManager fileManager;
MetadataManager metadataManager;


void setup() {
  gui.begin();
  fileManager.initSD();           // 1. Initialize SD card first
  metadataManager.init();          // 2. Build string offset index (uses SD)
  fileManager.init(metadataManager);  // 3. Validate SD + read test songs
  
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
