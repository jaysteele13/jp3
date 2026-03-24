#ifndef ENUMS_H
#define ENUMS_H

#include "Arduino.h"

// FOLDER
enum class FolderType {
    ALBUMS,
    PLAYLISTS,
    ARTISTS,
    ALL_SONGS
};

struct SongInfo {
    String songName;
    String artistName;
    String albumName;     // Album name (used for artist view display)
    String playlistName;  // Playlist name (empty if not in a playlist)
    int duration;         // in seconds
    String path;          // File path for playback
};

// Cache configuration to hold song data for pagination (Used within the folder section)
struct CacheConfig {
    static constexpr int MAX_CACHED_BATCHES = 3;
    static constexpr int PREFETCH_THRESHOLD = 3;
    static constexpr int SONGS_PER_BATCH = 10;
};


// SORTING
enum class SortMode {
    FILE_ORDER,      // File order
    ALPHABETICAL, // Alphabetical by name
    NEWEST,       // By year (descending) - albums only
    OLDEST        // By year (ascending) - albums only
};

// CATEGORY
enum class CategoryType {
    ALBUMS,
    PLAYLISTS,
    ARTISTS
};

struct AlbumInfo {
    String albumName;
};

struct CategoryInfo {
    String categoryName;
    String artistName;
};

struct SelectionContext {
    FolderType folderType;
    uint32_t id;  // album_id or artist_id (0 for ALL_SONGS)
};




#endif