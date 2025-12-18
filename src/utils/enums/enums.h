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
    int duration; // in seconds
};


// CATEGORY
enum class CategoryType {
    ALBUMS,
    PLAYLISTS,
    ARTISTS
};

struct CategoryInfo {
    String categoryName;
    String artistName;
};




#endif