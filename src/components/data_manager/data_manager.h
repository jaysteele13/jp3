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

    // Artist methods
    int getArtistCount();
};

#endif
