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
    }

    return categories;
}

void Category::selectNextFolder() {
    if (selectedFolderIndex < totalCategories - 1) {
        selectedFolderIndex++;
        // Update current page if needed
        if (selectedFolderIndex >= (currentPage + 1) * CategoryConfig::FOLDERS_PER_PAGE) {
            currentPage++;
        }
    }
}

void Category::selectPreviousFolder() {
    if (selectedFolderIndex > 0) {
        selectedFolderIndex--;
        // Update current page if needed
        if (selectedFolderIndex < currentPage * CategoryConfig::FOLDERS_PER_PAGE) {
            currentPage--;
        }
    }
}

void Category::drawPointer(Adafruit_SSD1306 &display, int x, int y) {
    // Draw a simple triangle pointer that points right
    display.fillTriangle(x, y + 3, x + 6, y, x + 6, y + 6, SSD1306_WHITE);
}

void Category::drawFolder(Adafruit_SSD1306 &display, int folderIndex, int &currentY, bool isSelected) {
    int textX = CategoryConfig::MARGIN_X;
    int pointerX = CategoryConfig::INDICATOR_OFFSET;
    
    if (isSelected) {
        // Draw pointer icon beside the selected folder
        drawPointer(display, pointerX, currentY);
        textX = pointerX + 10; // Leave space for the pointer
    } else {
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

    int currentY = 0;

    if (categories == nullptr) {
        categories = loadCategoryData(5);
    }

    // Display folders for current page
    int startFolderIndex = currentPage * CategoryConfig::FOLDERS_PER_PAGE;
    for (int i = 0; i < CategoryConfig::FOLDERS_PER_PAGE; ++i) {
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

