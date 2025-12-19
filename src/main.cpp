#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"

GUIManager gui;
Section section;


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
  gui.update();  // Handles all input, navigation, and rendering
}
