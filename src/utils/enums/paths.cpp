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