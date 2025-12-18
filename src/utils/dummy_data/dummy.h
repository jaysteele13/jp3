#ifndef DUMMY_H
#define DUMMY_H

#include "../enums/enums.h"


// Get songs for a specific album
SongInfo* getSongsForAlbum(String albumName, int& count);

// Get all songs for a specific artist (aggregates across albums)
SongInfo* getSongsForArtist(String artistName, int& count);

// Get all albums as CategoryInfo
CategoryInfo* getAllAlbums(int& count);

// Get all unique artists as CategoryInfo
CategoryInfo* getAllArtists(int& count);

// Get all playlists as CategoryInfo
CategoryInfo* getAllPlaylists(int& count);


#endif