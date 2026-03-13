#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
// INIT File Reading PINS

// Assume all are GPIO - where they are on ESP32
#define CS_PIN 5 // D5
#define MOSI_PIN 23 // D23
#define CLK_PIN 18 // D18
#define MISO_PIN 19 // D19

// Plan out what functions this manager should have. This will leverage the factory method perhaps. Ask around.

// Must Create a test env where for now we try to read a file name.

// Must make the base class with init and ReadFile, and List Root Path
#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "FS.h"

// Include custom Libraries for File Navigation
#include "utils/enums/paths.h"

class FileManager {
    private:
        SPIClass spi;
        bool validatePaths();
        
    public:
        FileManager();
        void initSD();
        bool validate();
        String GetBasePath();
        void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
        String ReadFile(String* basePath);
        File openMetadataFile();
        
    };

#endif