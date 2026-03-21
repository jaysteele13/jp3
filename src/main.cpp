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
  
#ifdef TEST_STAGE2
  Serial.println("=== STAGE 2 TEST ===");
  Serial.printf("Total songs in library: %d\n", dataManager.getSongCount());
  
  int albumCount = dataManager.getAlbumCount();
  Serial.printf("Total albums: %d\n", albumCount);
  
  if (albumCount > 0) {
    SongInfo* songs = nullptr;
    int songCount = dataManager.getSongsByAlbum(0, 0, 10, songs);
    Serial.printf("Songs in first album: %d\n", songCount);
    for (int i = 0; i < songCount; i++) {
      Serial.printf("  Song %d: title='%s', artist='%s', album='%s'\n", 
        i, songs[i].songName.c_str(), songs[i].artistName.c_str(), songs[i].albumName.c_str());
    }
    delete[] songs;
  }
  
  int artistCount = dataManager.getArtistCount();
  Serial.printf("Total artists: %d\n", artistCount);
  
  if (artistCount > 0) {
    SongInfo* songs = nullptr;
    int songCount = dataManager.getSongsByArtist(0, 0, 10, songs);
    Serial.printf("Songs by first artist: %d\n", songCount);
    for (int i = 0; i < songCount; i++) {
      Serial.printf("  Song %d: title='%s', artist='%s', album='%s'\n", 
        i, songs[i].songName.c_str(), songs[i].artistName.c_str(), songs[i].albumName.c_str());
    }
    delete[] songs;
  }
  
  SongInfo* allSongs = nullptr;
  int allSongCount = dataManager.getAllSongs(0, 10, allSongs);
  Serial.printf("All songs (first page): %d\n", allSongCount);
  for (int i = 0; i < allSongCount; i++) {
    Serial.printf("  Song %d: title='%s', artist='%s', album='%s'\n", 
      i, allSongs[i].songName.c_str(), allSongs[i].artistName.c_str(), allSongs[i].albumName.c_str());
  }
  delete[] allSongs;
  
  Serial.println("=== TEST COMPLETE ===\n");
#endif
  
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
