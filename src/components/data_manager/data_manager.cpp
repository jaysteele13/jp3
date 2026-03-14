#include "data_manager.h"
#include "../metadata_manager/metadata_manager.h"

DataManager::DataManager() : metadataManager(nullptr) {
}

void DataManager::setMetadataManager(MetadataManager* mm) {
    metadataManager = mm;
}

void DataManager::init() {
    if (!metadataManager) {
        Serial.println("ERROR: DataManager - MetadataManager not set");
        return;
    }
    Serial.println("DataManager initialized");
    Serial.printf("Album count from MetadataManager: %d\n and artist count: %d\n", getAlbumCount(), getArtistCount());
}

int DataManager::getAlbumCount() {
    if (!metadataManager) return 0;
    return metadataManager->getAlbumEntryCount();
}

int DataManager::getArtistCount() {
    if (!metadataManager) return 0;
    return metadataManager->getArtistEntryCount();
}
