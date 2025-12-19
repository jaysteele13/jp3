#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

#include "enums/enums.h"

class Utils {
public:
    // Utility functions can be added here
    static FolderType categoryTypeToFolderType(CategoryType categoryType);
    static CategoryType folderTypeToCategoryType(FolderType folderType);

};

#endif