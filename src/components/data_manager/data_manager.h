#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "Arduino.h"
#include "utils/enums/enums.h"

class MetadataManager;

#define MAX_ALBUMS 2000
#define MAX_ARTISTS 2000
#define DATA_MANAGER_BUFFER_SIZE 64
#define MAX_SONGS_PER_PAGE 10

class DataManager {
private:
    MetadataManager* metadataManager;
    
    // Sort indices - reordering these instead of moving actual entries
    uint32_t album_indices[MAX_ALBUMS];
    uint32_t artist_indices[MAX_ARTISTS];
    uint32_t album_count;
    uint32_t artist_count;
    SortMode currentAlbumSort;
    SortMode currentArtistSort;
    
    void sortAlbumIndices(SortMode mode);
    void sortArtistIndices(SortMode mode);
    int compareAlbumNames(uint32_t indexA, uint32_t indexB);
    int compareArtistNames(uint32_t indexA, uint32_t indexB);

public:
    DataManager();
    void setMetadataManager(MetadataManager* mm);
    void init();

    // Album methods
    int getAlbumCount();
    bool getAlbumName(uint32_t index, char* buffer, size_t buffer_size);
    bool getAlbumArtistName(uint32_t index, char* buffer, size_t buffer_size);
    void setAlbumSortMode(SortMode mode);
    SortMode getAlbumSortMode() { return currentAlbumSort; }

    // Artist methods
    int getArtistCount();
    bool getArtistName(uint32_t index, char* buffer, size_t buffer_size);
    void setArtistSortMode(SortMode mode);
    SortMode getArtistSortMode() { return currentArtistSort; }
    
    // Windowed access - returns CategoryInfo array (caller must delete[])
    // Returns: -1 = error, 0+ = number of items returned
    int getAlbums(uint16_t startIndex, uint8_t count, CategoryInfo*& out);
    int getArtists(uint16_t startIndex, uint8_t count, CategoryInfo*& out);
    
    // Song methods
    int getSongCount();
    int getSongsByAlbum(uint16_t sortedAlbumIndex, uint16_t startIndex, uint8_t count, SongInfo*& out);
    int getSongsByArtist(uint16_t sortedArtistIndex, uint16_t startIndex, uint8_t count, SongInfo*& out);
    int getAllSongs(uint16_t startIndex, uint8_t count, SongInfo*& out);
};

#endif
