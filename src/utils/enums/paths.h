#ifndef paths_h
#define paths_h

enum class Paths {
    BASE_PATH,
    METADATA_PATH,
    MUSIC_PATH,
    PLAYLISTS_PATH
};

const char* get_path(Paths path);



#endif