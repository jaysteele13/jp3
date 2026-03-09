#include "metadata_manager.h"
#include "SD.h"
#include "FS.h"

MetadataManager::MetadataManager() {
    // Constructor implementation (if needed)
}

void MetadataManager::init() {
    // Initialization code (if needed)
}

String MetadataManager::readString(File& file, uint32_t string_table_offset, uint32_t string_id) {
    file.seek(string_table_offset);
    
    for (uint32_t i = 0; i <= string_id; i++) {
        uint16_t string_length = file.read();
        string_length |= ((uint16_t)file.read() << 8);
        
        if (i == string_id) {
            char* buffer = new char[string_length + 1];
            file.read((uint8_t*)buffer, string_length);
            buffer[string_length] = '\0';
            String result = String(buffer);
            delete[] buffer;
            return result;
        } else {
            file.seek(file.position() + string_length);
        }
    }
    
    return "";
}

void MetadataManager::readFirstNSongs(uint8_t n) {
    File file = SD.open(get_path(Paths::METADATA_PATH));
    
    if (!file) {
        Serial.println("Failed to open metadata file");
        return;
    }

    Serial.println("Reading songs from metadata file...");

    // Step 1: Read Header
    // Offset 0x08-0x0B: song_count (u32)
    // Offset 0x14-0x17: string_table_offset (u32)
    // Offset 0x20-0x23: song_table_offset (u32)
    
    file.seek(0x08);
    uint32_t song_count = file.read();
    song_count |= ((uint32_t)file.read() << 8);
    song_count |= ((uint32_t)file.read() << 16);
    song_count |= ((uint32_t)file.read() << 24);

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

    Serial.printf("Total songs in library: %u\n", song_count);
    Serial.printf("String table offset: %u\n", string_table_offset);
    Serial.printf("Song table offset: %u\n", song_table_offset);
    Serial.println("----------------------------------------");

    // Step 2: Read N songs (skipping deleted)
    uint8_t songs_found = 0;
    uint32_t entry_index = 0;
    
    while (songs_found < n && entry_index < song_count) {
        uint32_t entry_offset = song_table_offset + (entry_index * 24);
        
        // Read flag first (offset 0x14 within song entry)
        file.seek(entry_offset + 0x14);
        uint8_t flags = file.read();
        
        // Check if active (0x00) or deleted (0x01)
        if (flags == 0x01) {
            Serial.printf("[%u] DELETED - skipping\n", entry_index);
            entry_index++;
            continue;
        }
        
        // Read full song entry (24 bytes)
        // Offset 0x00-0x03: title_string_id (u32)
        // Offset 0x04-0x07: artist_id (u32)
        // Offset 0x08-0x0B: album_id (u32)
        // Offset 0x0C-0x0F: path_string_id (u32)
        // Offset 0x10-0x11: track_number (u16)
        // Offset 0x12-0x13: duration_sec (u16)
        // Offset 0x14: flags (u8)
        
        file.seek(entry_offset);
        
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
        
        
        // Resolve strings
        String title = readString(file, string_table_offset, title_string_id);
        String path = readString(file, string_table_offset, path_string_id);

        Serial.printf("Song %u (entry %u):\n", songs_found + 1, entry_index);
        Serial.printf("  Title: %s\n", title.c_str());
        Serial.printf("  Path: %s\n", path.c_str());
        Serial.printf("  artist_id: %u, album_id: %u\n", artist_id, album_id);
        Serial.println("----------------------------------------");

        songs_found++;
        entry_index++;
    }

    Serial.printf("Displayed %u active songs\n", songs_found);
    file.close();
}