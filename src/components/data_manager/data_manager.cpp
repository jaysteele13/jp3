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
    char buffer[DATA_MANAGER_BUFFER_SIZE];
    if (album_count > 0) {
        getAlbumName(0, buffer, sizeof(buffer));
        Serial.printf("First album (sorted): %s\n", buffer);
        getAlbumArtistName(0, buffer, sizeof(buffer));
        Serial.printf("First album artist: %s\n", buffer);
        
        // Test getAlbums windowed access
        CategoryInfo* albums = nullptr;
        int numAlbums = getAlbums(0, 2, albums);
        if (numAlbums > 0) {
            Serial.printf("getAlbums returned: %d\n", numAlbums);
            Serial.printf("getAlbums[0]: %s - %s\n", albums[0].categoryName.c_str(), albums[0].artistName.c_str());
            if (numAlbums > 1) {
                Serial.printf("getAlbums[1]: %s - %s\n", albums[1].categoryName.c_str(), albums[1].artistName.c_str());
            }
            delete[] albums;
        }
    }
    if (artist_count > 0) {
        getArtistName(0, buffer, sizeof(buffer));
        Serial.printf("First artist (sorted): %s\n", buffer);
        
        // Test getArtists windowed access
        CategoryInfo* artists = nullptr;
        int numArtists = getArtists(0, 2, artists);
        if (numArtists > 0) {
            Serial.printf("getArtists returned: %d\n", numArtists);
            Serial.printf("getArtists[0]: %s\n", artists[0].categoryName.c_str());
            if (numArtists > 1) {
                Serial.printf("getArtists[1]: %s\n", artists[1].categoryName.c_str());
            }
            delete[] artists;
        }
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
    char nameA[DATA_MANAGER_BUFFER_SIZE];
    char nameB[DATA_MANAGER_BUFFER_SIZE];
    
    getAlbumName(indexA, nameA, sizeof(nameA));
    getAlbumName(indexB, nameB, sizeof(nameB));
    
    return strcmp(nameA, nameB);
}

int DataManager::compareArtistNames(uint32_t indexA, uint32_t indexB) {
    char nameA[DATA_MANAGER_BUFFER_SIZE];
    char nameB[DATA_MANAGER_BUFFER_SIZE];
    
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

int DataManager::getAlbums(uint16_t startIndex, uint8_t count, CategoryInfo*& out) {
    if (!metadataManager) return -1;
    if (startIndex >= album_count) return -1;
    
    // Allocate array
    out = new CategoryInfo[count];
    if (!out) return -1;
    
    char buffer[DATA_MANAGER_BUFFER_SIZE];
    uint8_t itemsReturned = 0;
    
    for (uint8_t i = 0; i < count; i++) {
        uint16_t idx = startIndex + i;
        if (idx >= album_count) break;
        
        // Get album name into buffer, copy to String
        if (getAlbumName(idx, buffer, sizeof(buffer))) {
            out[i].categoryName = String(buffer);
        } else {
            out[i].categoryName = String("Unknown");
        }
        
        // Get artist name into buffer, copy to String
        if (getAlbumArtistName(idx, buffer, sizeof(buffer))) {
            out[i].artistName = String(buffer);
        } else {
            out[i].artistName = String("Unknown");
        }
        
        itemsReturned++;
    }
    return itemsReturned;
}

int DataManager::getArtists(uint16_t startIndex, uint8_t count, CategoryInfo*& out) {
    if (!metadataManager) return -1;
    if (startIndex >= artist_count) return -1;
    
    // Allocate array
    out = new CategoryInfo[count];
    if (!out) return -1;
    
    char buffer[DATA_MANAGER_BUFFER_SIZE];
    uint8_t itemsReturned = 0;
    
    for (uint8_t i = 0; i < count; i++) {
        uint16_t idx = startIndex + i;
        if (idx >= artist_count) break;
        
        // Get artist name into buffer, copy to String
        if (getArtistName(idx, buffer, sizeof(buffer))) {
            out[i].categoryName = String(buffer);
        } else {
            out[i].categoryName = String("Unknown");
        }
        
        // Artists don't have a parent name in this context, leave empty
        out[i].artistName = String("");
        
        itemsReturned++;
    }
    return itemsReturned;
}
