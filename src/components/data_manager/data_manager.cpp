#include "data_manager.h"
#include "../metadata_manager/metadata_manager.h"
#include <string.h>

// Helper function for case-insensitive string comparison
// Returns: -1 if a < b, 0 if equal, 1 if a > b
static int strcasecmp_local(const char* a, const char* b, size_t maxLen) {
    for (size_t i = 0; i < maxLen; i++) {
        char ca = a[i];
        char cb = b[i];
        
        // Convert to lowercase if uppercase
        if (ca >= 'A' && ca <= 'Z') ca += 32;
        if (cb >= 'A' && cb <= 'Z') cb += 32;
        
        if (ca != cb) return (ca < cb) ? -1 : 1;
        if (ca == '\0') return 0;
    }
    return 0;
}

// Quicksort partition for albums
static int partitionAlbums(uint32_t* indices, int low, int high, DataManager* dm) {
    uint32_t pivot = indices[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        // Compare indices[j] with pivot using DataManager
        char nameJ[DATA_MANAGER_BUFFER_SIZE];
        char nameP[DATA_MANAGER_BUFFER_SIZE];
        dm->getAlbumName(indices[j], nameJ, sizeof(nameJ));
        dm->getAlbumName(pivot, nameP, sizeof(nameP));
        
        if (strcasecmp_local(nameJ, nameP, DATA_MANAGER_BUFFER_SIZE) <= 0) {
            i++;
            uint32_t temp = indices[i];
            indices[i] = indices[j];
            indices[j] = temp;
        }
    }
    
    uint32_t temp = indices[i + 1];
    indices[i + 1] = indices[high];
    indices[high] = temp;
    return i + 1;
}

// Quicksort partition for artists
static int partitionArtists(uint32_t* indices, int low, int high, DataManager* dm) {
    uint32_t pivot = indices[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        char nameJ[DATA_MANAGER_BUFFER_SIZE];
        char nameP[DATA_MANAGER_BUFFER_SIZE];
        dm->getArtistName(indices[j], nameJ, sizeof(nameJ));
        dm->getArtistName(pivot, nameP, sizeof(nameP));
        
        if (strcasecmp_local(nameJ, nameP, DATA_MANAGER_BUFFER_SIZE) <= 0) {
            i++;
            uint32_t temp = indices[i];
            indices[i] = indices[j];
            indices[j] = temp;
        }
    }
    
    uint32_t temp = indices[i + 1];
    indices[i + 1] = indices[high];
    indices[high] = temp;
    return i + 1;
}

// Quicksort for albums
static void quicksortAlbums(uint32_t* indices, int low, int high, DataManager* dm) {
    if (low < high) {
        int pi = partitionAlbums(indices, low, high, dm);
        quicksortAlbums(indices, low, pi - 1, dm);
        quicksortAlbums(indices, pi + 1, high, dm);
    }
}

// Quicksort for artists
static void quicksortArtists(uint32_t* indices, int low, int high, DataManager* dm) {
    if (low < high) {
        int pi = partitionArtists(indices, low, high, dm);
        quicksortArtists(indices, low, pi - 1, dm);
        quicksortArtists(indices, pi + 1, high, dm);
    }
}

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
    setAlbumSortMode(SortMode::ALPHABETICAL);
    setArtistSortMode(SortMode::ALPHABETICAL);
    
    // Test string resolution
    char buffer[DATA_MANAGER_BUFFER_SIZE];
    if (album_count > 0) {
        getAlbumName(0, buffer, sizeof(buffer));
        Serial.printf("First album: %s\n", buffer);
    }
    if (artist_count > 0) {
        getArtistName(0, buffer, sizeof(buffer));
        Serial.printf("First artist: %s\n", buffer);
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
        // Quicksort for albums
        quicksortAlbums(album_indices, 0, album_count - 1, this);
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
        // Quicksort for artists
        quicksortArtists(artist_indices, 0, artist_count - 1, this);
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
