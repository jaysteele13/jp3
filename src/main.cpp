#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"

GUIManager gui;
Song testSong("Still Crazy After all of these Years", 240, "", "Paul Simon", "Still Crazy After all of these Years");

void setup() {
  gui.begin();
  gui.displaySong(testSong);
}

void loop() {
  gui.displaySong(testSong);
}
