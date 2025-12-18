#include "category.h"
#include "../../../components/button_manager/button_manager.h"

void Category::handleInput(ButtonManager& buttons) {
    if (buttons.checkDownPressed()) {
        Serial.println("Category: Down button pressed - navigating to next category");
        selectNextFolder();
    }
    
    if (buttons.checkUpPressed()) {
        Serial.println("Category: Up button pressed - navigating to previous category");
        selectPreviousFolder();
    }
}

Category::Category(CategoryType categoryType) : categoryType(categoryType), totalCategories(0), currentPage(0), selectedFolderIndex(0) {
    categories = nullptr;
    switch (categoryType) {
        case CategoryType::ALBUMS:
            categoryName = "Albums";
            break;
        case CategoryType::PLAYLISTS:
            categoryName = "Playlists";
            break;
        case CategoryType::ARTISTS:
            categoryName = "Artists";
            break;
    }
    
}

CategoryInfo* Category::loadCategoryData() {
    // Try to load real data first based on category type
    int dataCount = 0;
    CategoryInfo* data = nullptr;
    
    switch(categoryType) {
        case CategoryType::ALBUMS:
            data = dummyData::getAllAlbums(dataCount);
            if (data != nullptr && dataCount > 0) {
                totalCategories = dataCount;
                categories = data;
                return categories;
            }
            break;
        case CategoryType::ARTISTS:
            data = dummyData::getAllArtists(dataCount);
            if (data != nullptr && dataCount > 0) {
                totalCategories = dataCount;
                categories = data;
                return categories;
            }
            break;
        case CategoryType::PLAYLISTS:
            data = dummyData::getAllPlaylists(dataCount);
            if (data != nullptr && dataCount > 0) {
                totalCategories = dataCount;
                categories = data;
                return categories;
            }
            break;
    }
    
    // Fallback to dummy data if real data not available
    totalCategories = 1;
    categories = new CategoryInfo[totalCategories];
    for (int i = 0; i < totalCategories; ++i) {
        categories[i].categoryName = "No Data";
        categories[i].artistName = "";
    }

    return categories;
}

int Category::getFoldersPerPage() {
    return (categoryType == CategoryType::ALBUMS) ? CategoryConfig::ALBUMS_PER_PAGE : CategoryConfig::FOLDERS_PER_PAGE;
}

void Category::selectNextFolder() {
    if (selectedFolderIndex < totalCategories - 1) {
        selectedFolderIndex++;
        // Update current page if needed using dynamic folder count
        int foldersPerPage = getFoldersPerPage();
        if (selectedFolderIndex >= (currentPage + 1) * foldersPerPage) {
            currentPage++;
        }
    }
}

void Category::selectPreviousFolder() {
    if (selectedFolderIndex > 0) {
        selectedFolderIndex--;
        // Update current page if needed using dynamic folder count
        int foldersPerPage = getFoldersPerPage();
        if (selectedFolderIndex < currentPage * foldersPerPage) {
            currentPage--;
        }
    }
}

void Category::drawSmallBitmap(Adafruit_SSD1306 &display, int x, int y) {
    switch (categoryType) {
        case CategoryType::ALBUMS:
            display.drawBitmap(x, y, small_album_bitmap, 12, 12, SSD1306_WHITE);
            break;
        case CategoryType::PLAYLISTS:
            display.drawBitmap(x, y, small_playlist_bitmap, 12, 12, SSD1306_WHITE);
            break;
        case CategoryType::ARTISTS:
            display.drawBitmap(x, y, small_artist_bitmap, 12, 12, SSD1306_WHITE);
            break;
    }
}

void Category::drawDivider(Adafruit_SSD1306 &display, int &currentY) {
    display.drawLine(0, currentY, CategoryConfig::SCREEN_WIDTH, currentY, SSD1306_WHITE);
    currentY += CategoryConfig::DIVIDER_MARGIN;
}

void Category::drawFolder(Adafruit_SSD1306 &display, int folderIndex, int &currentY, bool isSelected) {
    int textX = CategoryConfig::MARGIN_X;
    int pointerX = CategoryConfig::INDICATOR_OFFSET;
    bool isAlbum = (categoryType == CategoryType::ALBUMS);
    bool isArtist = (categoryType == CategoryType::ARTISTS);
    bool isPlaylist = (categoryType == CategoryType::PLAYLISTS);
    
    if (isSelected) {
        // Draw small bitmap icon beside the selected folder
        if (isAlbum) {
            drawSmallBitmap(display, pointerX, (currentY+ (CategoryConfig::CENTRE_ALBUM_BITMAP_SPACING))); // to centre bitmap with Albums
        }
        else {
            drawSmallBitmap(display, pointerX, (currentY-2)); // to centre bitmap
        }
        textX = pointerX + 16; // Leave space for the small bitmap
    } else if (!isAlbum) {
        // Only add static spacing for non-album categories
        currentY += CategoryConfig::FOLDER_SPACING;
    }

    // Display folder name (category name)
    TextValidator::displayScrollingText(display, categories[folderIndex].categoryName, textX, currentY, 1, CategoryConfig::SCREEN_WIDTH - textX, folderIndex);
    currentY += CategoryConfig::LINE_HEIGHT;

    // Display artist name if available (for albums)
    if (categories[folderIndex].artistName.length() > 0) {
        TextValidator::displayScrollingText(display, categories[folderIndex].artistName, textX, currentY, 1, CategoryConfig::SCREEN_WIDTH - textX, folderIndex + 100);
        currentY += CategoryConfig::LINE_HEIGHT;
    }
}

void Category::display(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    int currentY = 2; // offset for bitmaps

    if (categories == nullptr) {
        categories = loadCategoryData();
    }

    // Display folders for current page
    int foldersPerPage = getFoldersPerPage();
    int startFolderIndex = currentPage * foldersPerPage;
    bool isAlbum = (categoryType == CategoryType::ALBUMS);
    
    // Calculate dynamic spacing for albums to fill screen evenly
    int albumSpacing = 0;
    if (isAlbum) {
        int totalTextHeight = foldersPerPage * 2 * CategoryConfig::LINE_HEIGHT; // 2 lines per album
        int totalDividers = (foldersPerPage - 1) * (1 + CategoryConfig::DIVIDER_MARGIN); // dividers between albums
        int usedSpace = totalTextHeight + totalDividers;
        int remainingSpace = CategoryConfig::SCREEN_HEIGHT - usedSpace;
        albumSpacing = remainingSpace / (foldersPerPage + 1); // distribute spacing top, between, and bottom
        currentY = albumSpacing; // Start with top spacing
    }
    
    for (int i = 0; i < foldersPerPage; ++i) {
        int folderIndex = startFolderIndex + i;
        if (folderIndex >= totalCategories) break;

        bool isSelected = (folderIndex == selectedFolderIndex);
        drawFolder(display, folderIndex, currentY, isSelected);
        
        // For albums: draw divider between first and last records only
        if (isAlbum && i < foldersPerPage - 1 && folderIndex < totalCategories - 1) {
            
            drawDivider(display, currentY);
            currentY += albumSpacing; // Add spacing after divider for albums
        }
    }

    display.display();
}


void Category::setCategoryType(CategoryType type) {
    categoryType = type;
    switch (categoryType) {
        case CategoryType::ALBUMS:
            categoryName = "Albums";
            break;
        case CategoryType::PLAYLISTS:
            categoryName = "Playlists";
            break;
        case CategoryType::ARTISTS:
            categoryName = "Artists";
            break;
    }
    // Reset selection when category changes
    selectedFolderIndex = 0;
    currentPage = 0;
    
    // Reload data for the new category type
    loadCategoryData();
}

CategoryInfo* Category::getSelectedCategory() const {
    if (categories == nullptr || selectedFolderIndex >= totalCategories) {
        return nullptr;
    }
    return &categories[selectedFolderIndex];
}

void Category::resetSelection() {
    selectedFolderIndex = 0;
    currentPage = 0;
}