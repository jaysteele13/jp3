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
    Serial.printf("Album count: %d, Artist count: %d\n", getAlbumCount(), getArtistCount());
    
    // Test string resolution
    char buffer[64];
    if (getAlbumCount() > 0) {
        getAlbumName(0, buffer, sizeof(buffer));
        Serial.printf("First album: %s\n", buffer);
        getAlbumArtistName(0, buffer, sizeof(buffer));
        Serial.printf("First album artist: %s\n", buffer);
    }
    if (getArtistCount() > 0) {
        getArtistName(0, buffer, sizeof(buffer));
        Serial.printf("First artist: %s\n", buffer);
    }
}

int DataManager::getAlbumCount() {
    if (!metadataManager) return 0;
    return metadataManager->getAlbumEntryCount();
}

int DataManager::getArtistCount() {
    if (!metadataManager) return 0;
    return metadataManager->getArtistEntryCount();
}

bool DataManager::getAlbumName(uint32_t index, char* buffer, size_t buffer_size) {
    if (!metadataManager) {
        buffer[0] = '\0';
        return false;
    }
    
    AlbumEntry* entry = metadataManager->getAlbumEntry(index);
    if (!entry) {
        buffer[0] = '\0';
        return false;
    }
    
    return metadataManager->readStringById(entry->name_string_id, buffer, buffer_size);
}

bool DataManager::getAlbumArtistName(uint32_t index, char* buffer, size_t buffer_size) {
    if (!metadataManager) {
        buffer[0] = '\0';
        return false;
    }
    
    AlbumEntry* albumEntry = metadataManager->getAlbumEntry(index);
    if (!albumEntry) {
        buffer[0] = '\0';
        return false;
    }
    
    ArtistEntry* artistEntry = metadataManager->getArtistEntry(albumEntry->artist_id);
    if (!artistEntry) {
        buffer[0] = '\0';
        return false;
    }
    
    return metadataManager->readStringById(artistEntry->name_string_id, buffer, buffer_size);
}

bool DataManager::getArtistName(uint32_t index, char* buffer, size_t buffer_size) {
    if (!metadataManager) {
        buffer[0] = '\0';
        return false;
    }
    
    ArtistEntry* entry = metadataManager->getArtistEntry(index);
    if (!entry) {
        buffer[0] = '\0';
        return false;
    }
    
    return metadataManager->readStringById(entry->name_string_id, buffer, buffer_size);
}
