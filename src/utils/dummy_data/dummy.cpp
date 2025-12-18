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