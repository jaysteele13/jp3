#include "dummy.h"

// ==================== DUMMY DATA ARRAYS ====================



// Song names for Abbey Road
static const SongInfo SONG_NAMES_ABBEY_ROAD[] = {
    {"Come Together", "The Beatles", 259},
    {"Something", "The Beatles", 182},
    {"Maxwell's Silver Hammer", "The Beatles", 207},
    {"Oh! Darling", "The Beatles", 214},
    {"Octopus's Garden", "The Beatles", 170},
    {"I Want You (She's So Heavy)", "The Beatles", 467},
    {"Here Comes the Sun", "The Beatles", 185},
    {"Because", "The Beatles", 165},
    {"You Never Give Me Your Money", "The Beatles", 242},
    {"Sun King", "The Beatles", 145},
    {"Mean Mr. Mustard", "The Beatles", 66},
    {"Polythene Pam", "The Beatles", 67},
    {"She Came In Through the Bathroom Window", "The Beatles", 118},
    {"Golden Slumbers", "The Beatles", 91},
    {"Carry That Weight", "The Beatles", 97},
    {"The End", "The Beatles", 141},
    {"Her Majesty", "The Beatles", 23}
};

static const int ABBEY_ROAD_SONG_COUNT = sizeof(SONG_NAMES_ABBEY_ROAD) / sizeof(SONG_NAMES_ABBEY_ROAD[0]);

// Album structure for storing album metadata
struct AlbumData {
    String albumName;
    const SongInfo* songs;
    int songCount;
    String artistName;
};

// Album data
static const AlbumData ALBUMS[] = {
    {"Abbey Road", SONG_NAMES_ABBEY_ROAD, ABBEY_ROAD_SONG_COUNT, "The Beatles"}
};

static const int ALBUM_COUNT = sizeof(ALBUMS) / sizeof(ALBUMS[0]);


// ==================== UTILITY FUNCTIONS ====================

// Get songs for a specific album
SongInfo* getSongsForAlbum(String albumName, int& count) {
    // Check in ALBUMS array if album exists by given name
    for (int i = 0; i < ALBUM_COUNT; ++i) {
        if (ALBUMS[i].albumName == albumName) {
            count = ALBUMS[i].songCount;
            SongInfo* songs = new SongInfo[count];
            for (int j = 0; j < count; ++j) {
                songs[j].songName = ALBUMS[i].songs[j].songName;
                songs[j].artistName = ALBUMS[i].artistName;
                songs[j].duration = ALBUMS[i].songs[j].duration;
            }
            return songs;
        }
    }
    count = 0;
    return nullptr;
}

// Get all albums as CategoryInfo (for category display)
CategoryInfo* getAllAlbums(int& count) {
    count = ALBUM_COUNT;
    CategoryInfo* albums = new CategoryInfo[count];
    for (int i = 0; i < count; ++i) {
        albums[i].categoryName = ALBUMS[i].albumName;
        albums[i].artistName = ALBUMS[i].artistName;
    }
    return albums;
}

// Get all unique artists from all albums
CategoryInfo* getAllArtists(int& count) {
    // Count unique artists
    int uniqueCount = 0;
    String uniqueArtists[10]; // Assuming max 10 unique artists
    
    for (int i = 0; i < ALBUM_COUNT; ++i) {
        bool isNew = true;
        for (int j = 0; j < uniqueCount; ++j) {
            if (uniqueArtists[j] == ALBUMS[i].artistName) {
                isNew = false;
                break;
            }
        }
        if (isNew && uniqueCount < 10) {
            uniqueArtists[uniqueCount++] = ALBUMS[i].artistName;
        }
    }
    
    count = uniqueCount;
    CategoryInfo* artists = new CategoryInfo[count];
    for (int i = 0; i < count; ++i) {
        artists[i].categoryName = uniqueArtists[i];
        artists[i].artistName = "";
    }
    return artists;
}

// Get all songs for a specific artist (aggregates from all albums)
SongInfo* getSongsForArtist(String artistName, int& count) {
    // First pass: count total songs for this artist
    int totalSongs = 0;
    for (int i = 0; i < ALBUM_COUNT; ++i) {
        if (ALBUMS[i].artistName == artistName) {
            totalSongs += ALBUMS[i].songCount;
        }
    }
    
    if (totalSongs == 0) {
        count = 0;
        return nullptr;
    }
    
    // Second pass: collect all songs for this artist
    SongInfo* songs = new SongInfo[totalSongs];
    int songIndex = 0;
    for (int i = 0; i < ALBUM_COUNT; ++i) {
        if (ALBUMS[i].artistName == artistName) {
            for (int j = 0; j < ALBUMS[i].songCount; ++j) {
                songs[songIndex++] = ALBUMS[i].songs[j];
            }
        }
    }
    
    count = totalSongs;
    return songs;
}

// Get all playlists (stub - add actual playlists as needed)
CategoryInfo* getAllPlaylists(int& count) {
    // For now, return empty - implement playlists in the future
    count = 0;
    return nullptr;
}