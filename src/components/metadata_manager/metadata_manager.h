#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "FS.h"

// Custom
#include "utils/enums/paths.h"
#include "utils/enums/enums.h"

#define MAX_STRINGS 6000

class MetadataManager {
    private:
        static uint32_t string_offsets[MAX_STRINGS];
        static bool offset_index_valid;
        void readString(File& file, uint32_t string_id, char* buffer, size_t buffer_size);
        int getArtistIdFromAlbum(File& file, uint32_t album_table_offset, uint32_t album_id);
        uint32_t readTableOffset(File& file, Offsets offset);
        void buildStringOffsetIndex(File& file, uint32_t string_table_offset);

    public:
        MetadataManager(); 
        void init();
        bool isOffsetIndexReady() { return offset_index_valid; }

        // Gather Types needed to return data
        SongInfo getSongDataByID(uint32_t song_id);
        CategoryInfo getArtistDataByID(File& file, uint32_t artist_id);
        CategoryInfo getAlbumDataByID(File& file, uint32_t album_id);
        
        int readStringId(File& file, Offsets table_type, uint32_t entry_id);
       
        void readFirstNSongs(uint8_t n);
        

        // Will have to work with AI to implment this. Will use get_offset(go to album or artist offset)
        // 0x00 will be the id for the string table, in album 0x04 will be the artist ID. Once these functions are done we will have to optimise
        // how we grab string data



        // ArtistData getArtistDataByID(uint32_t artist_id);
        // AlbumData getAlbumDataByID(uint32_t album_id);
};



#endif