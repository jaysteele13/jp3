#include "metadata_manager.h"

uint32_t MetadataManager::string_offsets[MAX_STRINGS];
bool MetadataManager::offset_index_valid = false;

MetadataManager::MetadataManager() {
    // Constructor implementation (if needed)
}

void MetadataManager::init() {
    File file = SD.open(get_path(Paths::METADATA_PATH));
    
    if (!file) {
        Serial.println("Failed to open metadata file during init");
        return;
    }

    uint32_t string_table_offset = readTableOffset(file, Offsets::STRING_TABLE_OFFSET);
    buildStringOffsetIndex(file, string_table_offset);
    
    file.close();
}

void MetadataManager::buildStringOffsetIndex(File& file, uint32_t string_table_offset) {
    file.seek(string_table_offset);
    
    uint32_t current_offset = string_table_offset;
    uint32_t index = 0;
    
    while (index < MAX_STRINGS) {
        string_offsets[index] = current_offset;
        
        uint16_t string_length = file.read();
        string_length |= ((uint16_t)file.read() << 8);
        
        if (string_length == 0) {
            break;
        }
        
        current_offset += 2 + string_length;
        file.seek(current_offset);
        index++;
    }
    
    offset_index_valid = true;
    Serial.printf("Built string offset index with %u entries\n", index);
}



uint32_t MetadataManager::readTableOffset(File& file, Offsets offset) {
    file.seek(get_offset(offset));
    uint32_t table_offset = file.read();
    table_offset |= ((uint32_t)file.read() << 8);
    table_offset |= ((uint32_t)file.read() << 16);
    table_offset |= ((uint32_t)file.read() << 24);
    return table_offset;
}

CategoryInfo MetadataManager::getArtistDataByID(File& file, uint32_t artist_id) {
    CategoryInfo info = {};

    uint32_t string_table_offset = readTableOffset(file, Offsets::STRING_TABLE_OFFSET);

    Serial.printf("Getting artist data for artist_id: %u\n", artist_id);

    int artist_string_id = readStringId(file, Offsets::ARTIST_TABLE_OFFSET, artist_id);
    if (artist_string_id < 0) {
        Serial.printf("Failed to get artist string ID for artist_id: %u\n", artist_id);
        return info;
    }

    info.categoryName = readString(file, string_table_offset, artist_string_id);
    

    return info;
}

CategoryInfo MetadataManager::getAlbumDataByID(File& file, uint32_t album_id) {
    CategoryInfo info = {};

    // Could this be minimised?
    uint32_t string_table_offset = readTableOffset(file, Offsets::STRING_TABLE_OFFSET);
    uint32_t album_table_offset = readTableOffset(file, Offsets::ALBUM_TABLE_OFFSET);

    int album_string_id = readStringId(file, Offsets::ALBUM_TABLE_OFFSET, album_id);
    if (album_string_id < 0) {
        Serial.printf("Failed to get album string ID for album_id: %u\n", album_id);
        return info;
    }

    info.categoryName = readString(file, string_table_offset, album_string_id);
    Serial.printf("Album name: %s\n", info.categoryName.c_str());

    int artist_id = getArtistIdFromAlbum(file, album_table_offset, album_id);
    if (artist_id >= 0) {
        int artist_string_id = readStringId(file, Offsets::ARTIST_TABLE_OFFSET, artist_id);
        if (artist_string_id >= 0) {
            info.artistName = readString(file, string_table_offset, artist_string_id);
            Serial.printf("Artist name from album: %s\n", info.artistName.c_str());
        }
    }

    return info;
}




String MetadataManager::readString(File& file, uint32_t string_table_offset, uint32_t string_id) {
    if (string_id >= MAX_STRINGS) {
        Serial.printf("Error: string_id %u exceeds MAX_STRINGS %d\n", string_id, MAX_STRINGS);
        return "";
    }
    
    file.seek(string_offsets[string_id]);
    
    uint16_t string_length = file.read();
    string_length |= ((uint16_t)file.read() << 8);
    
    char* buffer = new char[string_length + 1];
    file.read((uint8_t*)buffer, string_length);
    buffer[string_length] = '\0';
    String result = String(buffer);
    delete[] buffer;
    return result;
}
// Retrieves string id of entry in table
int MetadataManager::readStringId(File& file, Offsets table_type, uint32_t entry_id) {
    uint8_t entry_size;
    if (table_type == Offsets::ARTIST_TABLE_OFFSET) {
        entry_size = 8;
    } else if (table_type == Offsets::ALBUM_TABLE_OFFSET) {
        entry_size = 16;
    } else if (table_type == Offsets::SONG_TABLE_OFFSET) {
        entry_size = 24;
    } else {
        Serial.printf("Unknown table type\n");
        return -1;
    }
    
    uint32_t table_offset = readTableOffset(file, table_type);
    Serial.printf("Reading string ID from table type: %d, table offset: %u, entry ID: %u, entry size: %u\n", (int)table_type, table_offset, entry_id, entry_size);
    
    uint32_t entry_position = table_offset + (entry_id * entry_size);
    file.seek(entry_position);
    
    uint32_t string_id = file.read();
    string_id |= ((uint32_t)file.read() << 8);
    string_id |= ((uint32_t)file.read() << 16);
    string_id |= ((uint32_t)file.read() << 24);
    
    return static_cast<int>(string_id);
}

int MetadataManager::getArtistIdFromAlbum(File& file, uint32_t album_table_offset, uint32_t album_id) {
    uint32_t artist_id_offset = album_table_offset + (album_id * 16) + 0x04;
    file.seek(artist_id_offset);
    
    uint32_t artist_id = file.read();
    artist_id |= ((uint32_t)file.read() << 8);
    artist_id |= ((uint32_t)file.read() << 16);
    artist_id |= ((uint32_t)file.read() << 24);
    
    return static_cast<int>(artist_id);
}



// Ishould make functions to:
/*
 - Grab Artist Data by ID
 - Grab Album Data by ID
 - Grab SongData by ID (which includes title, artist_id, album_id, path, track number, duration (this will have to retroactively call Artist and Album))
 - In Paths maybe HardCode some scenarios in the header. For example:

 0x00    | 4     | magic bytes              | "LIB1" (4 bytes)
0x04    | 4     | version                  | u32, little-endian
0x08    | 4     | song_count               | u32, little-endian
0x0C    | 4     | artist_count             | u32, little-endian
0x10    | 4     | album_count              | u32, little-endian
0x14    | 4     | string_table_offset      | u32, little-endian (typically 40)
0x18    | 4     | artist_table_offset      | u32, little-endian
0x1C    | 4     | album_table_offset       | u32, little-endian
0x20    | 4     | song_table_offset        | u32, little-endian
0x24    | 4     | reserved                 | 4 bytes

we will be using the 0x14 as string_table_offset alot, likewise for the others

*/
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
    
    // Address in Bin to seek out the Song Count.
    file.seek(0x08);
    uint32_t song_count = file.read();
    song_count |= ((uint32_t)file.read() << 8);
    song_count |= ((uint32_t)file.read() << 16);
    song_count |= ((uint32_t)file.read() << 24);

    file.seek(get_offset(Offsets::STRING_TABLE_OFFSET)); // Seeks out the String Table Offset in the Bin.
    uint32_t string_table_offset = file.read();
    string_table_offset |= ((uint32_t)file.read() << 8);
    string_table_offset |= ((uint32_t)file.read() << 16);
    string_table_offset |= ((uint32_t)file.read() << 24);
    
    file.seek(get_offset(Offsets::SONG_TABLE_OFFSET)); // Seeks out the Song Table offset in the Bin.
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

        uint32_t pos_before_artist = file.position();
        CategoryInfo artist_info = getArtistDataByID(file, artist_id);
        file.seek(pos_before_artist);
        Serial.printf("Artist name: %s\n", artist_info.categoryName.c_str());
        
        uint32_t album_id = file.read();
        album_id |= ((uint32_t)file.read() << 8);
        album_id |= ((uint32_t)file.read() << 16);
        album_id |= ((uint32_t)file.read() << 24);

        // Temporary bad and dirty
        pos_before_artist = file.position();
        CategoryInfo album_info = getAlbumDataByID(file, album_id);
        file.seek(pos_before_artist);

        uint32_t path_string_id = file.read();

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