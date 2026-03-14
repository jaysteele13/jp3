#include "metadata_manager.h"
#include "../file_manager/file_manager.h"

uint32_t MetadataManager::string_offsets[MAX_STRINGS];
bool MetadataManager::offset_index_valid = false;

AlbumEntry MetadataManager::album_entries[MAX_ALBUMS];
ArtistEntry MetadataManager::artist_entries[MAX_ARTISTS];
uint32_t MetadataManager::album_count = 0;
uint32_t MetadataManager::artist_count = 0;
bool MetadataManager::entries_loaded = false;

MetadataManager::MetadataManager() : fileManager(nullptr) {
}

void MetadataManager::setFileManager(FileManager* fm) {
    fileManager = fm;
}

void MetadataManager::init() {
    if (!fileManager) {
        Serial.println("ERROR: FileManager not set for MetadataManager");
        return;
    }
    
    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("Failed to open metadata file during init");
        return;
    }

    uint32_t string_table_offset = readTableOffset(metadataFile, Offsets::STRING_TABLE_OFFSET);
    buildStringOffsetIndex(metadataFile, string_table_offset);
    
    // Load album and artist entries into RAM
    loadAlbumEntries(metadataFile);
    loadArtistEntries(metadataFile);
    
    metadataFile.close();
    
    Serial.printf("MetadataManager init complete: %u albums, %u artists\n", album_count, artist_count);
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

void MetadataManager::loadAlbumEntries(File& file) {
    // Read album count from header
    file.seek(0x10);
    album_count = file.read();
    album_count |= ((uint32_t)file.read() << 8);
    album_count |= ((uint32_t)file.read() << 16);
    album_count |= ((uint32_t)file.read() << 24);
    
    if (album_count > MAX_ALBUMS) {
        Serial.printf("WARNING: Album count %u exceeds MAX_ALBUMS %d, truncating\n", album_count, MAX_ALBUMS);
        album_count = MAX_ALBUMS;
    }
    
    uint32_t album_table_offset = readTableOffset(file, Offsets::ALBUM_TABLE_OFFSET);
    file.seek(album_table_offset);
    
    for (uint32_t i = 0; i < album_count; i++) {
        album_entries[i].name_string_id = file.read();
        album_entries[i].name_string_id |= ((uint32_t)file.read() << 8);
        album_entries[i].name_string_id |= ((uint32_t)file.read() << 16);
        album_entries[i].name_string_id |= ((uint32_t)file.read() << 24);
        
        album_entries[i].artist_id = file.read();
        album_entries[i].artist_id |= ((uint32_t)file.read() << 8);
        album_entries[i].artist_id |= ((uint32_t)file.read() << 16);
        album_entries[i].artist_id |= ((uint32_t)file.read() << 24);
        
        album_entries[i].year = file.read();
        album_entries[i].year |= ((uint16_t)file.read() << 8);
        
        // Skip 6 reserved bytes
        for (int j = 0; j < 6; j++) {
            file.read();
        }
    }
    
    Serial.printf("Loaded %u album entries into RAM\n", album_count);
}

void MetadataManager::loadArtistEntries(File& file) {
    // Read artist count from header
    file.seek(0x0C);
    artist_count = file.read();
    artist_count |= ((uint32_t)file.read() << 8);
    artist_count |= ((uint32_t)file.read() << 16);
    artist_count |= ((uint32_t)file.read() << 24);
    
    if (artist_count > MAX_ARTISTS) {
        Serial.printf("WARNING: Artist count %u exceeds MAX_ARTISTS %d, truncating\n", artist_count, MAX_ARTISTS);
        artist_count = MAX_ARTISTS;
    }
    
    uint32_t artist_table_offset = readTableOffset(file, Offsets::ARTIST_TABLE_OFFSET);
    file.seek(artist_table_offset);
    
    for (uint32_t i = 0; i < artist_count; i++) {
        artist_entries[i].name_string_id = file.read();
        artist_entries[i].name_string_id |= ((uint32_t)file.read() << 8);
        artist_entries[i].name_string_id |= ((uint32_t)file.read() << 16);
        artist_entries[i].name_string_id |= ((uint32_t)file.read() << 24);
        
        // Skip 4 reserved bytes
        for (int j = 0; j < 4; j++) {
            file.read();
        }
    }
    
    Serial.printf("Loaded %u artist entries into RAM\n", artist_count);
}

AlbumEntry* MetadataManager::getAlbumEntry(uint32_t index) {
    if (index >= album_count) {
        return nullptr;
    }
    return &album_entries[index];
}

ArtistEntry* MetadataManager::getArtistEntry(uint32_t index) {
    if (index >= artist_count) {
        return nullptr;
    }
    return &artist_entries[index];
}

CategoryInfo MetadataManager::getArtistDataByID(uint32_t artist_id) {
    CategoryInfo info = {};
    
    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("Failed to open metadata file in getArtistDataByID");
        return info;
    }

    uint32_t string_table_offset = readTableOffset(metadataFile, Offsets::STRING_TABLE_OFFSET);

    Serial.printf("Getting artist data for artist_id: %u\n", artist_id);

    int artist_string_id = readStringId(metadataFile, Offsets::ARTIST_TABLE_OFFSET, artist_id);
    if (artist_string_id < 0) {
        Serial.printf("Failed to get artist string ID for artist_id: %u\n", artist_id);
        metadataFile.close();
        return info;
    }

    char buffer[256];
    readString(metadataFile, artist_string_id, buffer, sizeof(buffer));
    info.categoryName = String(buffer);
    
    metadataFile.close();
    return info;
}

CategoryInfo MetadataManager::getAlbumDataByID(uint32_t album_id) {
    CategoryInfo info = {};

    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("Failed to open metadata file in getAlbumDataByID");
        return info;
    }

    // Could this be minimised?
    uint32_t string_table_offset = readTableOffset(metadataFile, Offsets::STRING_TABLE_OFFSET);
    uint32_t album_table_offset = readTableOffset(metadataFile, Offsets::ALBUM_TABLE_OFFSET);

    int album_string_id = readStringId(metadataFile, Offsets::ALBUM_TABLE_OFFSET, album_id);
    if (album_string_id < 0) {
        Serial.printf("Failed to get album string ID for album_id: %u\n", album_id);
        metadataFile.close();
        return info;
    }

    char buffer[256];
    readString(metadataFile, album_string_id, buffer, sizeof(buffer));
    info.categoryName = String(buffer);
    Serial.printf("Album name: %s\n", info.categoryName.c_str());

    int artist_id = getArtistIdFromAlbum(metadataFile, album_table_offset, album_id);
    if (artist_id >= 0) {
        int artist_string_id = readStringId(metadataFile, Offsets::ARTIST_TABLE_OFFSET, artist_id);
        if (artist_string_id >= 0) {
            readString(metadataFile, artist_string_id, buffer, sizeof(buffer));
            info.artistName = String(buffer);
            Serial.printf("Artist name from album: %s\n", info.artistName.c_str());
        }
    }

    metadataFile.close();
    return info;
}




void MetadataManager::readString(File& file, uint32_t string_id, char* buffer, size_t buffer_size) {
    if (string_id >= MAX_STRINGS) {
        Serial.printf("Error: string_id %u exceeds MAX_STRINGS %d\n", string_id, MAX_STRINGS);
        buffer[0] = '\0';
        return;
    }
    
    file.seek(string_offsets[string_id]);
    
    uint16_t string_length = file.read();
    string_length |= ((uint16_t)file.read() << 8);
    
    size_t read_len = (string_length < buffer_size - 1) ? string_length : buffer_size - 1;
    file.read((uint8_t*)buffer, read_len);
    buffer[read_len] = '\0';
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

int MetadataManager::readStringId(Offsets table_type, uint32_t entry_id) {
    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("Failed to open metadata file in readStringId");
        return -1;
    }
    
    int result = readStringId(metadataFile, table_type, entry_id);
    
    metadataFile.close();
    return result;
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
    File metadataFile = fileManager->openMetadataFile();
    
    if (!metadataFile) {
        Serial.println("Failed to open metadata file");
        return;
    }

    Serial.println("Reading songs from metadata file...");

    // Step 1: Read Header
    metadataFile.seek(0x08);
    uint32_t song_count = metadataFile.read();
    song_count |= ((uint32_t)metadataFile.read() << 8);
    song_count |= ((uint32_t)metadataFile.read() << 16);
    song_count |= ((uint32_t)metadataFile.read() << 24);

    metadataFile.seek(get_offset(Offsets::STRING_TABLE_OFFSET));
    uint32_t string_table_offset = metadataFile.read();
    string_table_offset |= ((uint32_t)metadataFile.read() << 8);
    string_table_offset |= ((uint32_t)metadataFile.read() << 16);
    string_table_offset |= ((uint32_t)metadataFile.read() << 24);
    
    metadataFile.seek(get_offset(Offsets::SONG_TABLE_OFFSET));
    uint32_t song_table_offset = metadataFile.read();
    song_table_offset |= ((uint32_t)metadataFile.read() << 8);
    song_table_offset |= ((uint32_t)metadataFile.read() << 16);
    song_table_offset |= ((uint32_t)metadataFile.read() << 24);

    Serial.printf("Total songs in library: %u\n", song_count);
    Serial.printf("String table offset: %u\n", string_table_offset);
    Serial.printf("Song table offset: %u\n", song_table_offset);
    Serial.println("----------------------------------------");

    // Step 2: Read N songs (skipping deleted)
    uint8_t songs_found = 0;
    uint32_t entry_index = 0;
    
    while (songs_found < n && entry_index < song_count) {
        uint32_t entry_offset = song_table_offset + (entry_index * 24);
        
        metadataFile.seek(entry_offset + 0x14);
        uint8_t flags = metadataFile.read();
        
        if (flags == 0x01) {
            Serial.printf("[%u] DELETED - skipping\n", entry_index);
            entry_index++;
            continue;
        }
        
        metadataFile.seek(entry_offset);
        
        uint32_t title_string_id = metadataFile.read();
        title_string_id |= ((uint32_t)metadataFile.read() << 8);
        title_string_id |= ((uint32_t)metadataFile.read() << 16);
        title_string_id |= ((uint32_t)metadataFile.read() << 24);
        
        uint32_t artist_id = metadataFile.read();
        artist_id |= ((uint32_t)metadataFile.read() << 8);
        artist_id |= ((uint32_t)metadataFile.read() << 16);
        artist_id |= ((uint32_t)metadataFile.read() << 24);

        CategoryInfo artist_info = getArtistDataByID(artist_id);
        Serial.printf("Artist name: %s\n", artist_info.categoryName.c_str());
        
        uint32_t album_id = metadataFile.read();
        album_id |= ((uint32_t)metadataFile.read() << 8);
        album_id |= ((uint32_t)metadataFile.read() << 16);
        album_id |= ((uint32_t)metadataFile.read() << 24);

        CategoryInfo album_info = getAlbumDataByID(album_id);

        uint32_t path_string_id = metadataFile.read();

        char buffer[256];
        readString(metadataFile, title_string_id, buffer, sizeof(buffer));
        String title = String(buffer);
        readString(metadataFile, path_string_id, buffer, sizeof(buffer));
        String path = String(buffer);

        Serial.printf("Song %u (entry %u):\n", songs_found + 1, entry_index);
        Serial.printf("  Title: %s\n", title.c_str());
        Serial.printf("  Path: %s\n", path.c_str());
        Serial.printf("  artist_id: %u, album_id: %u\n", artist_id, album_id);
        Serial.println("----------------------------------------");

        songs_found++;
        entry_index++;
    }

    Serial.printf("Displayed %u active songs\n", songs_found);
    metadataFile.close();
}