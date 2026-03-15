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

struct AlbumEntry {
    uint32_t name_string_id;
    uint32_t artist_id;
    uint16_t year;
};

struct ArtistEntry {
    uint32_t name_string_id;
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
};

#endif