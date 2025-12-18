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

void Category::drawHeader(Adafruit_SSD1306 &display, int &currentY) {
    // Display Category name with Icon!
    TextValidator::displayScrollingText(display, categoryName, CategoryConfig::MARGIN_X, currentY, 2, CategoryConfig::SCREEN_WIDTH, 0);
    currentY += (CategoryConfig::LINE_HEIGHT + CategoryConfig::DIVIDER_MARGIN); // Size Text is big so twice as big of a difference
}

void Category::drawDivider(Adafruit_SSD1306 &display, int &currentY) {
    // Create a dynamic zigzag pattern divider
    int amplitude = 2;
    int wavelength = 8;
    
    for (int x = 0; x < CategoryConfig::SCREEN_WIDTH; x += wavelength) {
        int nextX = (x + wavelength < CategoryConfig::SCREEN_WIDTH) ? x + wavelength : CategoryConfig::SCREEN_WIDTH;
        display.drawLine(x, currentY, nextX, currentY + (x / wavelength % 2 == 0 ? amplitude : -amplitude), SSD1306_WHITE);
    }
    
    currentY += CategoryConfig::DIVIDER_MARGIN + amplitude;
}

void Category::selectNextFolder() {
    if (selectedFolderIndex < totalCategories - 1) {
        selectedFolderIndex++;
        // Update current page if needed based on folder positions
        if (currentPage == 0 && selectedFolderIndex >= 2) {
            currentPage = 1; // Move to page 1 when selecting folder 2
        } else if (currentPage > 0) {
            int currentPageEnd = getStartFolderIndex() + getFoldersPerPage();
            if (selectedFolderIndex >= currentPageEnd) {
                currentPage++;
            }
        }
    }
}

void Category::selectPreviousFolder() {
    if (selectedFolderIndex > 0) {
        selectedFolderIndex--;
        // Update current page if needed based on folder positions
        if (currentPage > 1) {
            int currentPageStart = getStartFolderIndex();
            if (selectedFolderIndex < currentPageStart) {
                currentPage--;
            }
        } else if (currentPage == 1 && selectedFolderIndex < 2) {
            currentPage = 0; // Move back to page 0 when selecting folder 0-1
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

bool Category::shouldShowHeader() {
    return currentPage == 0;
}

int Category::getFoldersPerPage() {
    return shouldShowHeader() ? 2 : FOLDERS_PER_PAGE;
}

int Category::getHeaderHeight() {
    if (!shouldShowHeader()) return 0;
    
    int headerHeight = CategoryConfig::LINE_HEIGHT + CategoryConfig::DIVIDER_MARGIN;
    headerHeight += CategoryConfig::DIVIDER_MARGIN + 2; // Divider height + margin
    return headerHeight;
}

int Category::getStartFolderIndex() {
    // Page 0 has 2 folders, all other pages have 4 folders
    if (currentPage == 0) {
        return 0;
    } else {
        // Account for the 2 folders on page 0, then calculate remaining pages
        return 2 + (currentPage - 1) * FOLDERS_PER_PAGE;
    }
}

void Category::display(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    int currentY = 0;

    if (categories == nullptr) {
        categories = loadCategoryData(5);
    }

    // Show header and divider only on first page
    if (shouldShowHeader()) {
        drawHeader(display, currentY);
        drawDivider(display, currentY);
    }

    // Get current page configuration
    int foldersPerPage = getFoldersPerPage();
    int startFolderIndex = getStartFolderIndex();

    // Display folders for current page
    for (int i = 0; i < foldersPerPage; ++i) {
        int folderIndex = startFolderIndex + i;
        if (folderIndex >= totalCategories) break;

        bool isSelected = (folderIndex == selectedFolderIndex);
        drawFolder(display, folderIndex, currentY, isSelected);
    }

    display.display();
}

