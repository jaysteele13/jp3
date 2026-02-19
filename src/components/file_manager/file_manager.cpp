#include "file_manager.h"


FileManager::FileManager() 
{
    // What Paths
    // currentPath? Base path - how does the reading Library work?
}

void FileManager::init() {
    Serial.begin(115200);
    delay(500);

    // init MicroSD 
}

// These should really be pointers - will pseudo once I understand microSD library
String FileManager::GetBasePath() {
    return "Test GetBasePath()";
}

String FileManager::ReadFile(String* basePath) {
    return "Test ReadFile() function";
}



