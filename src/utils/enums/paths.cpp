#include "paths.h"

const char* get_path(Paths path) {
    switch (path) {
        case Paths::BASE_PATH: return "/jp3";
        case Paths::METADATA_PATH: return "/jp3/metadata/library.bin";
        case Paths::MUSIC_PATH: return "/jp3/music";
        case Paths::PLAYLISTS_PATH: return "/jp3/playlists";
        default: return nullptr;
    }
}

const uint32_t get_offset(Offsets offset) {
    switch (offset) {
        case Offsets::STRING_TABLE_OFFSET: return 0x14;
        case Offsets::ARTIST_TABLE_OFFSET: return 0x18;
        case Offsets::ALBUM_TABLE_OFFSET: return 0x1C;
        case Offsets::SONG_TABLE_OFFSET: return 0x20;
        default: return 0;
    }
}