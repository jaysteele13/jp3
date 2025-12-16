// Make Dummy Call to Receive Album Name Folder, with file names

// Parse these so we display song name (use scrollable text) and artist in a box.

// Paginate so we can fit 2 songs on screen at once. When button is clicked we scroll down one song.

#include "folder.h"
#include "../../validation/text_validator.h"

void Folder::drawSelectionBox(Adafruit_SSD1306 &display, int x, int y, int width, int height) {
    display.drawRect(x, y, width, height, SSD1306_WHITE);
}

void Folder::selectNextSong() {
    if (selectedSongIndex < totalSongs - 1) {
        selectedSongIndex++;
        // Update current page if needed
        if (selectedSongIndex >= (currentPage + 1) * SONGS_PER_PAGE) {
            currentPage++;
        }
    }
}

void Folder::selectPreviousSong() {
    if (selectedSongIndex > 0) {
        selectedSongIndex--;
        // Update current page if needed
        if (selectedSongIndex < currentPage * SONGS_PER_PAGE) {
            currentPage--;
        }
    }
}

Folder::Folder(FolderType folderType, String folderName) : 
    folderType(folderType), 
    folderName(folderName), 
    totalSongs(0), 
    currentPage(0),
    selectedSongIndex(0) {
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
    int startX = 2; // Avoid Overlap with Selection Box
    int currentY = 0;

    // Get Dummy Data in Screen (Probably not best Practice but for demo purposes)
    if (songs == nullptr) {
        songs = loadSongData(5); // Load 5 dummy songs
    }

    // Display folder Name
    TextValidator::displayScrollingText(display, folderName, startX, currentY, 1, 128, 0);
    currentY += lineHeight;

    // Divider
    display.drawLine(0, currentY, 128, currentY, SSD1306_WHITE);
    currentY += 2;

    int startSongIndex = currentPage * SONGS_PER_PAGE;
    for (int i = 0; i < SONGS_PER_PAGE; ++i) {
        int songIndex = startSongIndex + i;
        if (songIndex >= totalSongs) break;

        int songStartY = currentY;
        bool isSelected = (songIndex == selectedSongIndex);

        // Draw selection box if selected
        if (isSelected) {
            drawSelectionBox(display, startX-2, currentY - 4, 128, lineHeight * 2 + 2);
            display.setCursor(startX, currentY);
            // startX += 8; // Offset text after > symbol
        }
        else {
            currentY += 4;
        }

        // Display song name with scrolling
        TextValidator::displayScrollingText(display, songs[songIndex].songName, startX, currentY, 1, 128 - startX, i);
        currentY += lineHeight;

        // Display artist name with scrolling
        TextValidator::displayScrollingText(display, songs[songIndex].artistName, startX, currentY, 1, 128 - startX, i);
        currentY += lineHeight;

        // if (isSelected) {
        //     startX -= 8; // Reset offset for next iteration
        // }
    }

    display.display();
}