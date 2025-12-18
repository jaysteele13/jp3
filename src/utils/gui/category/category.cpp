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

void Category::display(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    int currentY = 0;

    if (categories == nullptr) {
        categories = loadCategoryData(5);
    }

    drawHeader(display, currentY);
    drawDivider(display, currentY);

    // Additional display logic for categories can be added here

    display.display();
}

