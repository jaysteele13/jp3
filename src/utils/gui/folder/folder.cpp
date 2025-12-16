// Make Dummy Call to Receive Album Name Folder, with file names

// Parse these so we display song name (use scrollable text) and artist in a box.

// Paginate so we can fit 2 songs on screen at once. When button is clicked we scroll down one song.

#include "folder.h"
#include "../../validation/text_validator.h"

Folder::Folder(FolderType folderType, String folderName) : 
    folderType(folderType), 
    folderName(folderName), 
    totalSongs(0), 
    currentPage(0) {
    songs = nullptr;
}

SongInfo* Folder::loadSongData(int amount) {
    // For demonstration, we will create dummy song data
    totalSongs = amount;
    songs = new SongInfo[totalSongs];
    for (int i = 0; i < totalSongs; ++i) {
        songs[i].songName = "Song " + String(i + 1);
        songs[i].artistName = "Artist " + String(i + 1);
    }
    return songs;
}

void Folder::display(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    const int lineHeight = 12;
    const int startX = 0;
    int currentY = 0;

    // Get Dummy Data in Screen (Probably not best Practice but for demo purposes)
    if (songs == nullptr) {
        songs = loadSongData(5); // Load 5 dummy songs
    }

    int startSongIndex = currentPage * SONGS_PER_PAGE;
    for (int i = 0; i < SONGS_PER_PAGE; ++i) {
        int songIndex = startSongIndex + i;
        if (songIndex >= totalSongs) break;

        // Display song name with scrolling
        TextValidator::displayScrollingText(display, songs[songIndex].songName, startX, currentY, 1, 128, i);
        currentY += lineHeight;

        // Display artist name with scrolling
        TextValidator::displayScrollingText(display, songs[songIndex].artistName, startX, currentY, 1, 128, i);
        currentY += lineHeight;
    }

    display.display();
}