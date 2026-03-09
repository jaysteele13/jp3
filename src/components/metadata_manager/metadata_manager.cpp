#include "metadata_manager.h"
#include "SD.h"
#include "FS.h"

MetadataManager::MetadataManager() {
    // Constructor implementation (if needed)
}

void MetadataManager::init() {
    // Initialization code (if needed)
}

void MetadataManager::readFirstSong() {
    File file = SD.open(get_path(Paths::METADATA_PATH));
    
    if (!file) {
        Serial.println("Failed to open metadata file");
        return;
    }

    Serial.println("Metadata file opened successfully - time to read first song id");
    // Step 1: Read Header (40 bytes)
    // Offset 0x14-0x17: string_table_offset (u32)
    // Offset 0x20-0x23: song_table_offset (u32)
    
    file.seek(0x14);
    uint32_t string_table_offset = file.read();
    string_table_offset |= ((uint32_t)file.read() << 8);
    string_table_offset |= ((uint32_t)file.read() << 16);
    string_table_offset |= ((uint32_t)file.read() << 24);
    
    file.seek(0x20);
    uint32_t song_table_offset = file.read();
    song_table_offset |= ((uint32_t)file.read() << 8);
    song_table_offset |= ((uint32_t)file.read() << 16);
    song_table_offset |= ((uint32_t)file.read() << 24);

    Serial.printf("String table offset: %u\n", string_table_offset);
    Serial.printf("Song table offset: %u\n", song_table_offset);

    // Step 2: Read First Song Entry (24 bytes)
    // Offset 0x00-0x03: title_string_id (u32)
    // Offset 0x04-0x07: artist_id (u32)
    // Offset 0x08-0x0B: album_id (u32)
    // Offset 0x0C-0x0F: path_string_id (u32)
    
    file.seek(song_table_offset);
    
    uint32_t title_string_id = file.read();
    title_string_id |= ((uint32_t)file.read() << 8);
    title_string_id |= ((uint32_t)file.read() << 16);
    title_string_id |= ((uint32_t)file.read() << 24);
    
    uint32_t artist_id = file.read();
    artist_id |= ((uint32_t)file.read() << 8);
    artist_id |= ((uint32_t)file.read() << 16);
    artist_id |= ((uint32_t)file.read() << 24);
    
    uint32_t album_id = file.read();
    album_id |= ((uint32_t)file.read() << 8);
    album_id |= ((uint32_t)file.read() << 16);
    album_id |= ((uint32_t)file.read() << 24);
    
    uint32_t path_string_id = file.read();
    path_string_id |= ((uint32_t)file.read() << 8);
    path_string_id |= ((uint32_t)file.read() << 16);
    path_string_id |= ((uint32_t)file.read() << 24);

    Serial.printf("First Song IDs:\n");
    Serial.printf("  title_string_id: %u\n", title_string_id);
    Serial.printf("  artist_id: %u\n", artist_id);
    Serial.printf("  album_id: %u\n", album_id);
    Serial.printf("  path_string_id: %u\n", path_string_id);

    file.close();
}