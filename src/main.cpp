#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"

GUIManager gui;
Song testSong("Test Song", 240, "Test aLBUM qUEEN", "Test ArtisT fAM", "Test Playlist testing for days ahhahahah");

void setup() {
  gui.begin();
  gui.displaySong(testSong);
}

void loop() {
  gui.displaySong(testSong);
}
