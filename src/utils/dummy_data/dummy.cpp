#include "dummy.h"

// ==================== DUMMY DATA ARRAYS ====================

// Macro to calculate array size at compile time
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))


static const SongInfo SONG_NAMES_DARK_SIDE[] = {
    {"Speak to Me", "Pink Floyd", "The Dark Side of the Moon", "", 90},
    {"Breathe", "Pink Floyd", "The Dark Side of the Moon", "", 163},
    {"On the Run", "Pink Floyd", "The Dark Side of the Moon", "", 234},
    {"Time", "Pink Floyd", "The Dark Side of the Moon", "", 421},
    {"The Great Gig in the Sky", "Pink Floyd", "The Dark Side of the Moon", "", 276},
    {"Money", "Pink Floyd", "The Dark Side of the Moon", "", 382},
    {"Us and Them", "Pink Floyd", "The Dark Side of the Moon", "", 462},
    {"Any Colour You Like", "Pink Floyd", "The Dark Side of the Moon", "", 205},
    {"Brain Damage", "Pink Floyd", "The Dark Side of the Moon", "", 228},
    {"Eclipse", "Pink Floyd", "The Dark Side of the Moon", "", 123}
};

// Song names for Mathrock Playlist
static const SongInfo SONG_NAMES_MATHROCK_PLAYLIST[] = {
    {"Ants of the Sky", "Between the Buried and Me", "Mathrock Playlist", "Mathrock Playlist", 383},
    {"Toccata", "Battles", "Mathrock Playlist", "Mathrock Playlist", 245},
    {"Jane II", "Jane Doe", "Mathrock Playlist", "Mathrock Playlist", 412},
    {"Here Come the Tubular Bells", "Mike Oldfield", "Mathrock Playlist", "Mathrock Playlist", 318},
    {"Dyed in the Wool", "Don Caballero", "Mathrock Playlist", "Mathrock Playlist", 356},
    {"Goodbye Butterfly", "Minus the Bear", "Mathrock Playlist", "Mathrock Playlist", 289},
    {"Transmit Receive", "Lynches", "Mathrock Playlist", "Mathrock Playlist", 274},
    {"The Patient", "Tool", "Mathrock Playlist", "Mathrock Playlist", 205},
    {"Complexity of Suspense", "Yvynyl", "Mathrock Playlist", "Mathrock Playlist", 421},
    {"Trigonometry", "Polygonometry", "Mathrock Playlist", "Mathrock Playlist", 367}
};

// Song names for Abbey Road
static const SongInfo SONG_NAMES_ABBEY_ROAD[] = {
    {"Come Together", "The Beatles", "Abbey Road", "", 259},
    {"Something", "The Beatles", "Abbey Road", "", 182},
    {"Maxwell's Silver Hammer", "The Beatles", "Abbey Road", "", 207},
    {"Oh! Darling", "The Beatles", "Abbey Road", "", 214},
    {"Octopus's Garden", "The Beatles", "Abbey Road", "", 170},
    {"I Want You (She's So Heavy)", "The Beatles", "Abbey Road", "", 467},
    {"Here Comes the Sun", "The Beatles", "Abbey Road", "", 185},
    {"Because", "The Beatles", "Abbey Road", "", 165},
    {"You Never Give Me Your Money", "The Beatles", "Abbey Road", "", 242},
    {"Sun King", "The Beatles", "Abbey Road", "", 145},
    {"Mean Mr. Mustard", "The Beatles", "Abbey Road", "", 66},
    {"Polythene Pam", "The Beatles", "Abbey Road", "", 67},
    {"She Came In Through the Bathroom Window", "The Beatles", "Abbey Road", "", 118},
    {"Golden Slumbers", "The Beatles", "Abbey Road", "", 91},
    {"Carry That Weight", "The Beatles", "Abbey Road", "", 97},
    {"The End", "The Beatles", "Abbey Road", "", 141},
    {"Her Majesty", "The Beatles", "Abbey Road", "", 23}
};

// Album structure for storing album metadata
struct AlbumData {
    String albumName;
    const SongInfo* songs;
    int songCount;
    String artistName;
};

// Album data
static const AlbumData ALBUMS[] = {
    {"Abbey Road", SONG_NAMES_ABBEY_ROAD, ARRAY_SIZE(SONG_NAMES_ABBEY_ROAD), "The Beatles"},
    {"The Dark Side of the Moon", SONG_NAMES_DARK_SIDE, ARRAY_SIZE(SONG_NAMES_DARK_SIDE), "Pink Floyd"},
};
static const int ALBUM_COUNT = sizeof(ALBUMS) / sizeof(ALBUMS[0]);


// Playlists
struct PlaylistData {
    String playlistName;
    const SongInfo* songs;
    int songCount;
};

static const PlaylistData PLAYLISTS[] = {
    {"Mathrock Playlist", SONG_NAMES_MATHROCK_PLAYLIST, ARRAY_SIZE(SONG_NAMES_MATHROCK_PLAYLIST)},
};

// ==================== UTILITY FUNCTIONS ====================

// Get songs for a specific album
SongInfo* dummyData::getSongsForAlbum(String albumName, int& count) {
    // Check in ALBUMS array if album exists by given name
    for (int i = 0; i < ALBUM_COUNT; ++i) {
        if (ALBUMS[i].albumName == albumName) {
            count = ALBUMS[i].songCount;
            SongInfo* songs = new SongInfo[count];
            for (int j = 0; j < count; ++j) {
                songs[j].songName = ALBUMS[i].songs[j].songName;
                songs[j].artistName = ALBUMS[i].artistName;
                songs[j].albumName = ALBUMS[i].albumName;
                songs[j].playlistName = ALBUMS[i].songs[j].playlistName;
                songs[j].duration = ALBUMS[i].songs[j].duration;
            }
            return songs;
        }
    }
    count = 0;
    return nullptr;
}

// Get all albums as CategoryInfo (for category display)
CategoryInfo* dummyData::getAllAlbums(int& count) {
    count = ALBUM_COUNT;
    CategoryInfo* albums = new CategoryInfo[count];
    for (int i = 0; i < count; ++i) {
        albums[i].categoryName = ALBUMS[i].albumName;
        albums[i].artistName = ALBUMS[i].artistName;
    }
    return albums;
}

// Get all unique artists from all albums
CategoryInfo* dummyData::getAllArtists(int& count) {
    // First pass: count unique artists (max possible is ALBUM_COUNT)
    String uniqueArtists[ALBUM_COUNT]; // Dynamically sized based on album count
    int uniqueCount = 0;
    
    for (int i = 0; i < ALBUM_COUNT; ++i) {
        bool isNew = true;
        for (int j = 0; j < uniqueCount; ++j) {
            if (uniqueArtists[j] == ALBUMS[i].artistName) {
                isNew = false;
                break;
            }
        }
        if (isNew) {
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
SongInfo* dummyData::getSongsForArtist(String artistName, int& count) {
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
    
    // Second pass: collect all songs for this artist with album info
    SongInfo* songs = new SongInfo[totalSongs];
    int songIndex = 0;
    for (int i = 0; i < ALBUM_COUNT; ++i) {
        if (ALBUMS[i].artistName == artistName) {
            for (int j = 0; j < ALBUMS[i].songCount; ++j) {
                songs[songIndex].songName = ALBUMS[i].songs[j].songName;
                songs[songIndex].artistName = ALBUMS[i].songs[j].artistName;
                songs[songIndex].albumName = ALBUMS[i].albumName;  // Include album name
                songs[songIndex].playlistName = ALBUMS[i].songs[j].playlistName;
                songs[songIndex].duration = ALBUMS[i].songs[j].duration;
                songIndex++;
            }
        }
    }
    
    count = totalSongs;
    return songs;
}

// Get all songs for a specific playlist
SongInfo* dummyData::getSongsForPlaylist(String playlistName, int& count) {
    // First pass: count total songs in this playlist
    int totalSongs = 0;
    for (int i = 0; i < sizeof(PLAYLISTS) / sizeof(PLAYLISTS[0]); ++i) {
        if (PLAYLISTS[i].playlistName == playlistName) {
            totalSongs = PLAYLISTS[i].songCount;
            break;
        }
    }
    
    if (totalSongs == 0) {
        count = 0;
        return nullptr;
    }
    
    // Second pass: collect all songs for this playlist
    SongInfo* songs = new SongInfo[totalSongs];
    for (int i = 0; i < sizeof(PLAYLISTS) / sizeof(PLAYLISTS[0]); ++i) {
        if (PLAYLISTS[i].playlistName == playlistName) {
            for (int j = 0; j < PLAYLISTS[i].songCount; ++j) {
                songs[j] = PLAYLISTS[i].songs[j];
            }
            break;
        }
    }
    
    count = totalSongs;
    return songs;
}

// Get all playlists (stub - add actual playlists as needed)
CategoryInfo* dummyData::getAllPlaylists(int& count) {
    // For now, return empty - implement playlists in the future
    count = 0;
    return nullptr;
}