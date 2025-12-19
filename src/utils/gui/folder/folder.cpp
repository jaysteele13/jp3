#include "folder.h"
#include "../../../components/button_manager/button_manager.h"

void Folder::handleInput(ButtonManager& buttons) {
    if (buttons.checkDownPressed()) {
        Serial.println("Folder: Down button pressed - selecting next song");
        selectNextSong();
    }
    
    if (buttons.checkUpPressed()) {
        Serial.println("Folder: Up button pressed - selecting previous song");
        selectPreviousSong();
    }
}

void Folder::drawSelectionBox(Adafruit_SSD1306 &display, int x, int y, int width, int height) {
    display.drawRect(x, y, width, height, SSD1306_WHITE);
}

SongInfo* Folder::getSelectedSong() {
    if (songs != nullptr && selectedSongIndex >= 0 && selectedSongIndex < totalSongs) {
        return &songs[selectedSongIndex];
    }
    return nullptr;
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

// Getters and Setters

// Set folder data when navigating to this screen
void Folder::setFolderData(FolderType type, String name) {
    folderType = type;
    folderName = name;
}

// Reset selection and page
void Folder::resetSelection() {
    selectedSongIndex = 0;
    currentPage = 0;
}

SongInfo* Folder::loadSongData(int amount) {
    // Try to load real data based on folder type
    int dataCount = 0;
    SongInfo* songData = nullptr;
    
    if (folderType == FolderType::ALBUMS) {
        songData = dummyData::getSongsForAlbum(folderName, dataCount);
    } else if (folderType == FolderType::ARTISTS) {
        songData = dummyData::getSongsForArtist(folderName, dataCount);
    } else if (folderType == FolderType::PLAYLISTS) {
        songData = dummyData::getSongsForPlaylist(folderName, dataCount);
    } 
    else if (folderType == FolderType::ALL_SONGS) {
        songData = dummyData::getAllSongs(dataCount);
    }


    // If song data isn't null return
    if (songData != nullptr && dataCount > 0) {
        // Use real data
        totalSongs = dataCount;
        songs = songData;
        return songs;
    }
    
    SongInfo* noSong = new SongInfo[1];
    noSong[0].songName = "No Songs Found";
    noSong[0].artistName = "";
    totalSongs = 1;
    songs = noSong;
    return songs;
}


void Folder::drawHeader(Adafruit_SSD1306 &display, int &currentY) {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false); 

    // Build song counter text (e.g. "1/10")
    String songCounter = String(selectedSongIndex + 1) + "/" + String(totalSongs);
    int counterWidth = TextValidator::getTextWidth(songCounter, 1);

    // Visual gap between scrolling text and counter
    constexpr int HEADER_GAP = 6; // pixels

    // Calculate available width for scrolling text ONLY
    int availableHeaderTextWidth =
        DisplayConfig::SCREEN_WIDTH
        - DisplayConfig::MARGIN_X * 2
        - counterWidth
        - HEADER_GAP;

    TextValidator::displayScrollingText(
        display,
        folderName,
        DisplayConfig::MARGIN_X,
        currentY,
        1,
        availableHeaderTextWidth,
        0
    );

    int counterX =
        DisplayConfig::SCREEN_WIDTH
        - DisplayConfig::MARGIN_X
        - counterWidth;

    display.setCursor(counterX, currentY);
    display.print(songCounter);  

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

    // Display different secondary info based on folder type
    String secondaryInfo;
    if (folderType == FolderType::ARTISTS) {
        secondaryInfo = songs[songIndex].albumName;
    } else {
        secondaryInfo = songs[songIndex].artistName;
    }
    
    TextValidator::displayScrollingText(display, secondaryInfo, textX, currentY, 1, DisplayConfig::SCREEN_WIDTH - textX, songIndex);
    currentY += DisplayConfig::LINE_HEIGHT;
}

// Display
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