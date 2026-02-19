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
#include <Wire.h>

// Wile have to pass MicroSD Card Read and Add plenty of logs

class FileManager {
    private:
        // str* currentPath = nullptr; // Probably do not need
        // Probably will have to pass active instance of the microSD card Reader
        
    public:
        FileManager();
        void init();
        String GetBasePath();
        String ReadFile(String* basePath); // is String efficient? Plan with AI Have very good Prompt.
        
    };

#endif