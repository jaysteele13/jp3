#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "FS.h"

// Custom
#include "utils/enums/paths.h"
#include "utils/enums/enums.h"

class FileManager;

#define MAX_STRINGS 6000
#define MAX_ALBUMS 2000
#define MAX_ARTISTS 2000
#define MAX_SONGS_PER_QUERY 50

struct AlbumEntry {
    static constexpr size_t SIZE = 16;
    static constexpr size_t RESERVED_BYTES = 6;
    uint32_t name_string_id;
    uint32_t artist_id;
    uint16_t year;
    uint8_t reserved[6];
};

struct ArtistEntry {
    static constexpr size_t SIZE = 8;
    static constexpr size_t RESERVED_BYTES = 4;
    uint32_t name_string_id;
    uint8_t reserved[4];
};

struct SongEntry {
    static constexpr size_t SIZE = 24;
    static constexpr size_t RESERVED_BYTES = 7;
    uint32_t title_string_id;
    uint32_t artist_id;
    uint32_t album_id;
    uint32_t path_string_id;
    uint8_t flags;  // 0x00 = active, 0x01 = deleted
    uint8_t reserved[3];
};

class MetadataManager {
    private:
        static uint32_t string_offsets[MAX_STRINGS];
        static bool offset_index_valid;
        FileManager* fileManager;
        
        // Album and artist entries in RAM
        static AlbumEntry album_entries[MAX_ALBUMS];
        static ArtistEntry artist_entries[MAX_ARTISTS];
        static uint32_t album_count;
        static uint32_t artist_count;
        
        uint32_t readTableOffset(File& file, Offsets offset);
        void buildStringOffsetIndex(File& file, uint32_t string_table_offset);
        void loadAlbumEntries(File& file);
        void loadArtistEntries(File& file);

    public:
        MetadataManager(); 
        void setFileManager(FileManager* fm);
        void init();
        bool isOffsetIndexReady() { return offset_index_valid; }

        // Album entry access
        int getAlbumEntryCount() { return album_count; }
        AlbumEntry* getAlbumEntry(uint32_t index);
        
        // Artist entry access
        int getArtistEntryCount() { return artist_count; }
        ArtistEntry* getArtistEntry(uint32_t index);
        
        // String resolution (uses pre-built offset index)
        bool readStringById(uint32_t string_id, char* buffer, size_t buffer_size);
        
        // Song entry access (no RAM storage, on-demand scanning)
        int getSongCount();
        int loadSongEntriesByAlbum(uint32_t albumId, uint16_t startIndex, uint8_t count, SongEntry* out);
        int loadSongEntriesByArtist(uint32_t artistId, uint16_t startIndex, uint8_t count, SongEntry* out);
        int loadAllSongEntries(uint16_t startIndex, uint8_t count, SongEntry* out);
};

#endif