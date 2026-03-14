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




#endif