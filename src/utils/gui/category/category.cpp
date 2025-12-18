#include "category.h"

Category::Category(CategoryType categoryType) : categoryType(categoryType), totalCategories(0), currentPage(0), selectedFolderIndex(0), screenActive(false) {
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
    screenActive = false;
}

CategoryInfo* Category::loadCategoryData(int amount) {
    // For demonstration, we will create dummy category data
    // based on category type produce dummy data
    totalCategories = amount;
    categories = new CategoryInfo[totalCategories];
    switch(categoryType) {
        case CategoryType::ALBUMS:
            for (int i = 0; i < totalCategories; ++i) {
                categories[i].categoryName = "Album" + String(i + 1);
                categories[i].artistName = "Artist " + String(i + 1);
            }
            break;
        case CategoryType::PLAYLISTS:
            for (int i = 0; i < totalCategories; ++i) {
                categories[i].categoryName = "Playlist" + String(i + 1);
                categories[i].artistName = "";
            }
            break;
        case CategoryType::ARTISTS:
            for (int i = 0; i < totalCategories; ++i) {
                categories[i].categoryName = "Artist" + String(i + 1);
                categories[i].artistName = ""; // Artists don't need a secondary field
            }
            break;
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
    int spacing = isAlbum ? CategoryConfig::ALBUM_SPACING : CategoryConfig::FOLDER_SPACING;
    
    if (isSelected) {
        // Draw small bitmap icon beside the selected folder
        drawSmallBitmap(display, pointerX, currentY);
        textX = pointerX + 14; // Leave space for the small bitmap
    } else {
        currentY += spacing;
    }
    drawDivider(display, currentY);

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

    int currentY = 0;

    if (categories == nullptr) {
        categories = loadCategoryData(5);
    }

    // Display folders for current page
    int foldersPerPage = getFoldersPerPage();
    int startFolderIndex = currentPage * foldersPerPage;
    for (int i = 0; i < foldersPerPage; ++i) {
        int folderIndex = startFolderIndex + i;
        if (folderIndex >= totalCategories) break;

        bool isSelected = (folderIndex == selectedFolderIndex);
        drawFolder(display, folderIndex, currentY, isSelected);
    }

    display.display();
}



// void Category::display(Adafruit_SSD1306 &display) {
//     display.clearDisplay();

//     int currentY = 0;

//     if (categories == nullptr) {
//         categories = loadCategoryData(5);
//     }

//     // Show header and divider only on first page
//     if (shouldShowHeader()) {
//         drawHeader(display, currentY);
//         drawDivider(display, currentY);
//     }

//     // Get current page configuration
//     int foldersPerPage = getFoldersPerPage();
//     int startFolderIndex = getStartFolderIndex();

//     // Display folders for current page
//     for (int i = 0; i < foldersPerPage; ++i) {
//         int folderIndex = startFolderIndex + i;
//         if (folderIndex >= totalCategories) break;

//         bool isSelected = (folderIndex == selectedFolderIndex);
//         drawFolder(display, folderIndex, currentY, isSelected);
//     }

//     display.display();
// }

