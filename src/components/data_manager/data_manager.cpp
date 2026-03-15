#include "data_manager.h"
#include "../metadata_manager/metadata_manager.h"
#include <string.h>

DataManager::DataManager() : metadataManager(nullptr), album_count(0), artist_count(0),
    currentAlbumSort(SortMode::FILE_ORDER), currentArtistSort(SortMode::FILE_ORDER) {
    // Initialize indices to sequential order
    for (uint32_t i = 0; i < MAX_ALBUMS; i++) {
        album_indices[i] = i;
    }
    for (uint32_t i = 0; i < MAX_ARTISTS; i++) {
        artist_indices[i] = i;
    }
}

void DataManager::setMetadataManager(MetadataManager* mm) {
    metadataManager = mm;
}

void DataManager::init() {
    if (!metadataManager) {
        Serial.println("ERROR: DataManager - MetadataManager not set");
        return;
    }
    
    album_count = metadataManager->getAlbumEntryCount();
    artist_count = metadataManager->getArtistEntryCount();
    
    Serial.println("DataManager initialized");
    Serial.printf("Album count: %u, Artist count: %u\n", album_count, artist_count);
    
    // Default sort is alphabetical
    setAlbumSortMode(SortMode::FILE_ORDER);
    setArtistSortMode(SortMode::FILE_ORDER);
    
    // Test string resolution
    char buffer[64];
    if (album_count > 0) {
        getAlbumName(0, buffer, sizeof(buffer));
        Serial.printf("First album (sorted): %s\n", buffer);
        getAlbumArtistName(0, buffer, sizeof(buffer));
        Serial.printf("First album artist: %s\n", buffer);
    }
    if (artist_count > 0) {
        getArtistName(0, buffer, sizeof(buffer));
        Serial.printf("First artist (sorted): %s\n", buffer);
    }
}

int DataManager::getAlbumCount() {
    return album_count;
}

int DataManager::getArtistCount() {
    return artist_count;
}

void DataManager::setAlbumSortMode(SortMode mode) {
    if (mode == currentAlbumSort) return;
    currentAlbumSort = mode;
    sortAlbumIndices(mode);
}

void DataManager::setArtistSortMode(SortMode mode) {
    if (mode == currentArtistSort) return;
    currentArtistSort = mode;
    sortArtistIndices(mode);
}

int DataManager::compareAlbumNames(uint32_t indexA, uint32_t indexB) {
    char nameA[64];
    char nameB[64];
    
    getAlbumName(indexA, nameA, sizeof(nameA));
    getAlbumName(indexB, nameB, sizeof(nameB));
    
    return strcmp(nameA, nameB);
}

int DataManager::compareArtistNames(uint32_t indexA, uint32_t indexB) {
    char nameA[64];
    char nameB[64];
    
    getArtistName(indexA, nameA, sizeof(nameA));
    getArtistName(indexB, nameB, sizeof(nameB));
    
    return strcmp(nameA, nameB);
}

void DataManager::sortAlbumIndices(SortMode mode) {
    if (mode == SortMode::FILE_ORDER) {
        // Reset to sequential order
        for (uint32_t i = 0; i < album_count; i++) {
            album_indices[i] = i;
        }
        Serial.println("Album sort: FILE_ORDER");
        return;
    }
    
    if (mode == SortMode::ALPHABETICAL) {
        // Bubble sort for now - simple and works
        for (uint32_t i = 0; i < album_count - 1; i++) {
            for (uint32_t j = 0; j < album_count - i - 1; j++) {
                if (compareAlbumNames(album_indices[j], album_indices[j + 1]) > 0) {
                    // Swap
                    uint32_t temp = album_indices[j];
                    album_indices[j] = album_indices[j + 1];
                    album_indices[j + 1] = temp;
                }
            }
        }
        Serial.println("Album sort: ALPHABETICAL");
        return;
    }
    
    // Other sort modes can be added later (NEWEST, OLDEST)
    Serial.printf("Album sort mode %d not implemented, using FILE_SORT\n", (int)mode);
}

void DataManager::sortArtistIndices(SortMode mode) {
    if (mode == SortMode::FILE_ORDER) {
        for (uint32_t i = 0; i < artist_count; i++) {
            artist_indices[i] = i;
        }
        Serial.println("Artist sort: FILE_ORDER");
        return;
    }
    
    if (mode == SortMode::ALPHABETICAL) {
        for (uint32_t i = 0; i < artist_count - 1; i++) {
            for (uint32_t j = 0; j < artist_count - i - 1; j++) {
                if (compareArtistNames(artist_indices[j], artist_indices[j + 1]) > 0) {
                    uint32_t temp = artist_indices[j];
                    artist_indices[j] = artist_indices[j + 1];
                    artist_indices[j + 1] = temp;
                }
            }
        }
        Serial.println("Artist sort: ALPHABETICAL");
        return;
    }
    
    Serial.printf("Artist sort mode %d not implemented, using DEFAULT\n", (int)mode);
}

bool DataManager::getAlbumName(uint32_t index, char* buffer, size_t buffer_size) {
    if (!metadataManager || index >= album_count) {
        buffer[0] = '\0';
        return false;
    }
    
    // Use sorted index to get the actual album entry
    uint32_t sortedIndex = album_indices[index];
    AlbumEntry* entry = metadataManager->getAlbumEntry(sortedIndex);
    if (!entry) {
        buffer[0] = '\0';
        return false;
    }
    
    return metadataManager->readStringById(entry->name_string_id, buffer, buffer_size);
}

bool DataManager::getAlbumArtistName(uint32_t index, char* buffer, size_t buffer_size) {
    if (!metadataManager || index >= album_count) {
        buffer[0] = '\0';
        return false;
    }
    
    uint32_t sortedIndex = album_indices[index];
    AlbumEntry* albumEntry = metadataManager->getAlbumEntry(sortedIndex);
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
    if (!metadataManager || index >= artist_count) {
        buffer[0] = '\0';
        return false;
    }
    
    uint32_t sortedIndex = artist_indices[index];
    ArtistEntry* entry = metadataManager->getArtistEntry(sortedIndex);
    if (!entry) {
        buffer[0] = '\0';
        return false;
    }
    
    return metadataManager->readStringById(entry->name_string_id, buffer, buffer_size);
}
