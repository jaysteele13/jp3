#include "folder.h"


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
    selectedSongIndex(0),
    screenActive(false) {
    songs = nullptr;
}

SongInfo* Folder::loadSongData(int amount) {
    // Try to load real album data first
    int albumCount = 0;
    SongInfo* albumSongs = getSongsForAlbum(folderName, albumCount);
    
    if (albumSongs != nullptr && albumCount > 0) {
        // Use real album data
        totalSongs = albumCount;
        songs = albumSongs;
        return songs;
    }
    
    // Fallback to dummy data if album not found
    totalSongs = amount;
    songs = new SongInfo[totalSongs];
    for (int i = 0; i < totalSongs; ++i) {
        songs[i].songName = "Song" + String(i + 1);
        songs[i].artistName = "Artist " + String(i + 1);
        songs[i].duration = 180 + i * 10; // dummy duration in seconds
    }
    return songs;
}

void Folder::drawHeader(Adafruit_SSD1306 &display, int &currentY) {
    TextValidator::displayScrollingText(display, folderName, DisplayConfig::MARGIN_X, currentY, 1, DisplayConfig::SCREEN_WIDTH, 0);
    currentY += DisplayConfig::LINE_HEIGHT;
}

void Folder::drawDivider(Adafruit_SSD1306 &display, int &currentY) {
    display.drawLine(0, currentY, DisplayConfig::SCREEN_WIDTH, currentY, SSD1306_WHITE);
    currentY += DisplayConfig::DIVIDER_MARGIN;
}

void Folder::drawSong(Adafruit_SSD1306 &display, int songIndex, int &currentY, bool isSelected) {
    int textX = DisplayConfig::MARGIN_X;
    
    if (isSelected) {
        drawSelectionBox(display, DisplayConfig::MARGIN_X - DisplayConfig::MARGIN_X, 
                        currentY - DisplayConfig::SELECTION_MARGIN, 
                        DisplayConfig::SCREEN_WIDTH, DisplayConfig::LINE_HEIGHT * 2 + DisplayConfig::DIVIDER_MARGIN);
        display.setCursor(textX, currentY);
    } else {
        currentY += DisplayConfig::SONG_SPACING;
    }

    TextValidator::displayScrollingText(display, songs[songIndex].songName, textX, currentY, 1, DisplayConfig::SCREEN_WIDTH - textX, songIndex);
    currentY += DisplayConfig::LINE_HEIGHT;

    TextValidator::displayScrollingText(display, songs[songIndex].artistName, textX, currentY, 1, DisplayConfig::SCREEN_WIDTH - textX, songIndex);
    currentY += DisplayConfig::LINE_HEIGHT;
}

void Folder::display(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    int currentY = 0;

    if (songs == nullptr) {
        songs = loadSongData(5);
    }

    drawHeader(display, currentY);
    drawDivider(display, currentY);

    int startSongIndex = currentPage * SONGS_PER_PAGE;
    for (int i = 0; i < SONGS_PER_PAGE; ++i) {
        int songIndex = startSongIndex + i;
        if (songIndex >= totalSongs) break;

        bool isSelected = (songIndex == selectedSongIndex);
        drawSong(display, songIndex, currentY, isSelected);
    }

    display.display();
}