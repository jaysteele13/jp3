#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "Arduino.h"
#include "utils/enums/enums.h"

class MetadataManager;

class DataManager {
private:
    MetadataManager* metadataManager;

public:
    DataManager();
    void setMetadataManager(MetadataManager* mm);
    void init();

    // Album methods
    int getAlbumCount();
    bool getAlbumName(uint32_t index, char* buffer, size_t buffer_size);
    bool getAlbumArtistName(uint32_t index, char* buffer, size_t buffer_size);

    // Artist methods
    int getArtistCount();
    bool getArtistName(uint32_t index, char* buffer, size_t buffer_size);
};

#endif
