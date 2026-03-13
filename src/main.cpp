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
  fileManager.initSD();
  
  if (!fileManager.validate()) {
    Serial.println("ERROR: SD card validation failed");
    return;
  }
  
  metadataManager.setFileManager(&fileManager);
  metadataManager.init();
  
  metadataManager.readFirstNSongs(5);
  
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
