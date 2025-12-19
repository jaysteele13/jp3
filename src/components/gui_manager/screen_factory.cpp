#include "screen_factory.h"
#include "../../utils/utils.h"

Category* ScreenFactory::createCategory(FolderType folderType) {
    CategoryType categoryType = Utils::folderTypeToCategoryType(folderType);
    return new Category(categoryType);
}

Folder* ScreenFactory::createFolder(FolderType folderType, const String& categoryName) {
    return new Folder(folderType, categoryName);
}

Song* ScreenFactory::createSong(const SongInfo* songInfo) {
    return new Song(songInfo);
}

Section* ScreenFactory::createSection() {
    return new Section();
}