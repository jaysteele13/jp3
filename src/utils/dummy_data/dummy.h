#ifndef DUMMY_H
#define DUMMY_H

#include "../enums/enums.h"

class dummyData {
public:
    // Get songs for a specific album
    static SongInfo* getSongsForAlbum(String albumName, int& count);

    // Get all songs for a specific artist (aggregates across albums)
    static SongInfo* getSongsForArtist(String artistName, int& count);

    // Get all songs for a specific playlist
    static SongInfo* getSongsForPlaylist(String playlistName, int& count);

    // Get all albums as CategoryInfo
    static CategoryInfo* getAllAlbums(int& count);

    // Get all unique artists as CategoryInfo
    static CategoryInfo* getAllArtists(int& count);

    // Get all playlists as CategoryInfo
    static CategoryInfo* getAllPlaylists(int& count);

    // Get All Songs
    static SongInfo* getAllSongs(int& count);
};

#endif