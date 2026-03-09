#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "FS.h"

// Custom
#include "utils/enums/paths.h"

class MetadataManager {
    private:
        String readString(File& file, uint32_t string_table_offset, uint32_t string_id);

    public:
        MetadataManager(); 
        void init();

        void readFirstNSongs(uint8_t n);
};



#endif