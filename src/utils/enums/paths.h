#ifndef paths_h
#define paths_h

#include "Arduino.h"

enum class Paths {
    BASE_PATH,
    METADATA_PATH,
    MUSIC_PATH,
    PLAYLISTS_PATH
};

enum class Offsets {
    STRING_TABLE_OFFSET,
    ARTIST_TABLE_OFFSET,
    ALBUM_TABLE_OFFSET,
    SONG_TABLE_OFFSET 
};
/*
 0x00    | 4     | magic bytes              | "LIB1" (4 bytes)
0x04    | 4     | version                  | u32, little-endian
0x08    | 4     | song_count               | u32, little-endian
0x0C    | 4     | artist_count             | u32, little-endian
0x10    | 4     | album_count              | u32, little-endian
0x14    | 4     | string_table_offset      | u32, little-endian (typically 40)
0x18    | 4     | artist_table_offset      | u32, little-endian
0x1C    | 4     | album_table_offset       | u32, little-endian
0x20    | 4     | song_table_offset        | u32, little-endian
0x24    | 4     | reserved                 | 4 bytes

we will be using the 0x14 as string_table_offset alot, likewise for the others
*/

const char* get_path(Paths path);
const uint32_t get_offset(Offsets offset);



#endif