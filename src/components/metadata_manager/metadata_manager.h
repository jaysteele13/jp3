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

class MetadataManager {
    private:
        static uint32_t string_offsets[MAX_STRINGS];
        static bool offset_index_valid;
        FileManager* fileManager;
        
        void readString(File& file, uint32_t string_id, char* buffer, size_t buffer_size);
        int getArtistIdFromAlbum(File& file, uint32_t album_table_offset, uint32_t album_id);
        uint32_t readTableOffset(File& file, Offsets offset);
        void buildStringOffsetIndex(File& file, uint32_t string_table_offset);
        int readStringId(File& file, Offsets table_type, uint32_t entry_id);

    public:
        MetadataManager(); 
        void setFileManager(FileManager* fm);
        void init();
        bool isOffsetIndexReady() { return offset_index_valid; }

        // Gather Types needed to return data
        SongInfo getSongDataByID(uint32_t song_id);
        CategoryInfo getArtistDataByID(uint32_t artist_id);
        CategoryInfo getAlbumDataByID(uint32_t album_id);
        
        int readStringId(Offsets table_type, uint32_t entry_id);
       
        void readFirstNSongs(uint8_t n);
};



#endif