#include "Arduino.h"
#include "components/gui_manager/gui_manager.h"
#include "components/file_manager/file_manager.h"
#include "components/metadata_manager/metadata_manager.h"
#include "components/data_manager/data_manager.h"

GUIManager gui;
Section section;
FileManager fileManager;
MetadataManager metadataManager;
DataManager dataManager;


void setup() {
  gui.begin();
  fileManager.initSD();
  
  if (!fileManager.validate()) {
    Serial.println("ERROR: SD card validation failed");
    return;
  }
  
  metadataManager.setFileManager(&fileManager);
  metadataManager.init();
  
#ifdef TEST_STAGE1
  Serial.println("=== STAGE 1 TEST ===");
  Serial.printf("Total songs in library: %d\n", metadataManager.getSongCount());
  
  int amount_of_songs_to_load = 30;
  SongEntry testEntries[amount_of_songs_to_load];
  int count = metadataManager.loadAllSongEntries(0, amount_of_songs_to_load, testEntries);
  Serial.printf("First %d songs loaded\n", count);
  char titleBuffer[64];
  for (int i = 0; i < count; i++) {
    metadataManager.readStringById(testEntries[i].title_string_id, titleBuffer, sizeof(titleBuffer));
    Serial.printf("  Song %d: title: %s, title_id=%u, artist_id=%u, album_id=%u\n", 
      i, titleBuffer, testEntries[i].title_string_id, 
      testEntries[i].artist_id, testEntries[i].album_id);
  }
  Serial.println("=== TEST COMPLETE ===\n");
#endif
  
  dataManager.setMetadataManager(&metadataManager);
  dataManager.init();
  
  gui.setDataManager(&dataManager);
  
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
