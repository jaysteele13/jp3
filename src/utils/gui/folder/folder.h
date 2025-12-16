#ifndef FOLDER_H
#define FOLDER_H

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

enum class FolderType {
    ALBUMS,
    PLAYLISTS,
    ALL_SONGS
};

struct SongInfo {
            String songName;
            String artistName;
};

class Folder {
    public:
        Folder(FolderType folderType, String folderName);
        SongInfo* loadSongData(int amount);
        void display(Adafruit_SSD1306 &display);
        void nextPage(int currentPage);

    private:
        FolderType folderType;
        String folderName;
        SongInfo* songs;
        int totalSongs;
        int currentPage;
        static const int SONGS_PER_PAGE = 2;
};


#endif