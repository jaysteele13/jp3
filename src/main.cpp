#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"
#include "components/file_manager/file_manager.h"

GUIManager gui;
Section section;
FileManager fileManager;


void setup() {
  gui.begin();
  fileManager.init();
  
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
