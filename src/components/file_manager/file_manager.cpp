#include "file_manager.h"


FileManager::FileManager() : spi(VSPI) {}
  
void FileManager::init() {
    // This should be defined else where just for testing
    Serial.begin(115200);
    delay(500);
    spi.begin(CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
    // init MicroSD 
    if (!SD.begin(CS_PIN,spi,4000000)) {
    Serial.println("Card Mount Failed");
    return;
  }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    listDir(SD, "/", 0);
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



// These should really be pointers - will pseudo once I understand microSD library
String FileManager::GetBasePath() {
    return "Test GetBasePath()";
}

String FileManager::ReadFile(String* basePath) {
    return "Test ReadFile() function";
}



