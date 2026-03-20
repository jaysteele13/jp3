#include "metadata_manager.h"
#include "../file_manager/file_manager.h"

uint32_t MetadataManager::string_offsets[MAX_STRINGS];
bool MetadataManager::offset_index_valid = false;

AlbumEntry MetadataManager::album_entries[MAX_ALBUMS];
ArtistEntry MetadataManager::artist_entries[MAX_ARTISTS];
uint32_t MetadataManager::album_count = 0;
uint32_t MetadataManager::artist_count = 0;

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

bool MetadataManager::readStringById(uint32_t string_id, char* buffer, size_t buffer_size) {
    if (!offset_index_valid) {
        Serial.println("ERROR: String offset index not ready");
        buffer[0] = '\0';
        return false;
    }
    
    if (string_id >= MAX_STRINGS) {
        Serial.printf("ERROR: string_id %u exceeds MAX_STRINGS %d\n", string_id, MAX_STRINGS);
        buffer[0] = '\0';
        return false;
    }
    
    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("ERROR: Failed to open metadata file in readStringById");
        buffer[0] = '\0';
        return false;
    }
    
    metadataFile.seek(string_offsets[string_id]);
    
    uint16_t string_length = metadataFile.read();
    string_length |= ((uint16_t)metadataFile.read() << 8);
    
    size_t read_len = (string_length < buffer_size - 1) ? string_length : buffer_size - 1;
    metadataFile.read((uint8_t*)buffer, read_len);
    buffer[read_len] = '\0';
    
    metadataFile.close();
    return true;
}

int MetadataManager::getSongCount() {
    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("ERROR: Failed to open metadata file in getSongCount");
        return 0;
    }
    
    metadataFile.seek(0x08);
    uint32_t song_count = metadataFile.read();
    song_count |= ((uint32_t)metadataFile.read() << 8);
    song_count |= ((uint32_t)metadataFile.read() << 16);
    song_count |= ((uint32_t)metadataFile.read() << 24);
    
    metadataFile.close();
    return song_count;
}

static uint32_t readUint32(File& file) {
    uint32_t val = file.read();
    val |= ((uint32_t)file.read() << 8);
    val |= ((uint32_t)file.read() << 16);
    val |= ((uint32_t)file.read() << 24);
    return val;
}

static void skipSongReservedBytes(File& file) {
    for (int i = 0; i < 7; i++) {
        file.read();
    }
}

int MetadataManager::loadSongEntriesByAlbum(uint32_t albumId, uint16_t startIndex, uint8_t count, SongEntry* out) {
    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("ERROR: Failed to open metadata file in loadSongEntriesByAlbum");
        return 0;
    }
    
    uint32_t song_table_offset = readTableOffset(metadataFile, Offsets::SONG_TABLE_OFFSET);
    uint32_t total_song_count = getSongCount();
    
    uint32_t matched_count = 0;
    uint32_t returned_count = 0;
    uint32_t current_pos = song_table_offset;
    
    for (uint32_t i = 0; i < total_song_count; i++) {
        metadataFile.seek(current_pos);
        
        SongEntry entry;
        entry.title_string_id = readUint32(metadataFile);
        entry.artist_id = readUint32(metadataFile);
        entry.album_id = readUint32(metadataFile);
        entry.path_string_id = readUint32(metadataFile);
        skipSongReservedBytes(metadataFile);
        
        if ((entry.flags & 0x01) == 0x00) {
            if (entry.album_id == albumId) {
                matched_count++;
                if (matched_count > startIndex && returned_count < count) {
                    out[returned_count] = entry;
                    returned_count++;
                }
            }
        }
        
        current_pos += 24;
    }
    
    metadataFile.close();
    Serial.printf("loadSongEntriesByAlbum: albumId=%u, found=%u, returned=%u\n", albumId, matched_count, returned_count);
    return returned_count;
}

int MetadataManager::loadSongEntriesByArtist(uint32_t artistId, uint16_t startIndex, uint8_t count, SongEntry* out) {
    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("ERROR: Failed to open metadata file in loadSongEntriesByArtist");
        return 0;
    }
    
    uint32_t song_table_offset = readTableOffset(metadataFile, Offsets::SONG_TABLE_OFFSET);
    uint32_t total_song_count = getSongCount();
    
    uint32_t matched_count = 0;
    uint32_t returned_count = 0;
    uint32_t current_pos = song_table_offset;
    
    for (uint32_t i = 0; i < total_song_count; i++) {
        metadataFile.seek(current_pos);
        
        SongEntry entry;
        entry.title_string_id = readUint32(metadataFile);
        entry.artist_id = readUint32(metadataFile);
        entry.album_id = readUint32(metadataFile);
        entry.path_string_id = readUint32(metadataFile);
        skipSongReservedBytes(metadataFile);
        
        if ((entry.flags & 0x01) == 0x00) {
            if (entry.artist_id == artistId) {
                matched_count++;
                if (matched_count > startIndex && returned_count < count) {
                    out[returned_count] = entry;
                    returned_count++;
                }
            }
        }
        
        current_pos += 24;
    }
    
    metadataFile.close();
    Serial.printf("loadSongEntriesByArtist: artistId=%u, found=%u, returned=%u\n", artistId, matched_count, returned_count);
    return returned_count;
}

int MetadataManager::loadAllSongEntries(uint16_t startIndex, uint8_t count, SongEntry* out) {
    File metadataFile = fileManager->openMetadataFile();
    if (!metadataFile) {
        Serial.println("ERROR: Failed to open metadata file in loadAllSongEntries");
        return 0;
    }
    
    uint32_t song_table_offset = readTableOffset(metadataFile, Offsets::SONG_TABLE_OFFSET);
    uint32_t total_song_count = getSongCount();
    
    uint32_t matched_count = 0;
    uint32_t returned_count = 0;
    uint32_t current_pos = song_table_offset;
    
    for (uint32_t i = 0; i < total_song_count; i++) {
        metadataFile.seek(current_pos);
        
        SongEntry entry;
        entry.title_string_id = readUint32(metadataFile);
        entry.artist_id = readUint32(metadataFile);
        entry.album_id = readUint32(metadataFile);
        entry.path_string_id = readUint32(metadataFile);
        skipSongReservedBytes(metadataFile);
        
        if ((entry.flags & 0x01) == 0x00) {
            matched_count++;
            if (matched_count > startIndex && returned_count < count) {
                out[returned_count] = entry;
                returned_count++;
            }
        }
        
        current_pos += 24;
    }
    
    metadataFile.close();
    Serial.printf("loadAllSongEntries: total=%u, found=%u, returned=%u\n", total_song_count, matched_count, returned_count);
    return returned_count;
}
