#include "file_manager.h"


FileManager::FileManager() : spi(VSPI) {}

void FileManager::initSD() {
    Serial.begin(115200);
    delay(500);
    spi.begin(CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
    
    if (!SD.begin(CS_PIN, spi, 4000000)) {
        Serial.println("Card Mount Failed");
        return;
    }
    
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}
  
void FileManager::init(MetadataManager& metadataManager) {
    validateSDCard(metadataManager);
}

void FileManager::listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
     file = root.openNextFile();
}
}

bool FileManager::validateSDCard(MetadataManager& metadataManager) {
    
    // Once initialised filePath should have a global variable called basePAth. This will avoid having ot pass this. basePath once found should 
    // always be constant. Should be hardcoded as the jp3 folder in the microSD card will always be the same.
    // base path should be "/"

    // now have Paths to look through base path.

    // Perhaps fucntionise below
    // Check if the base path exists
    if (!SD.exists(get_path(Paths::BASE_PATH))) {
        Serial.println("Base path does not exist");
        return false;
    }

    // check if metadata exists
    if (!SD.exists(get_path(Paths::METADATA_PATH))) {
        Serial.println("Metadata file does not exist");
        return false;
    }

    // check if music path exists
    if (!SD.exists(get_path(Paths::MUSIC_PATH))) {
        Serial.println("Music path does not exist");
        return false;
    }

    // check if playlists path exists
    if (!SD.exists(get_path(Paths::PLAYLISTS_PATH))) {
        Serial.println("Playlists path does not exist");
        return false;
    }

    // We now should check individually if the library.bin exists and metadata. Likewise for playlists Currenly this can be empty)


    Serial.println("All paths are valid");

    // We should now validate the library.bin file. In order to do this we must make a seperate function that can read the first song.
    // if this fails then the library.bin file is not valid. Also check the size of the library bin file and make sure it is not empty

    File metadataFile = SD.open(get_path(Paths::METADATA_PATH));
    if (!metadataFile) {
        Serial.println("Failed to open metadata file");
        return false;
    }
    if (metadataFile.size() == 0) {
        Serial.println("Metadata file is empty");
        return false;
    }

    // once this is read we should pass this file into the metadataManager
    metadataManager.readFirstNSongs(5);

    return true;
}

// These should really be pointers - will pseudo once I understand microSD library
String FileManager::GetBasePath() {
    return "Test GetBasePath()";
}

String FileManager::ReadFile(String* basePath) {
    return "Test ReadFile() function";
}



