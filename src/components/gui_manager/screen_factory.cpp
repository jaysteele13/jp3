#include "screen_factory.h"
#include "../../utils/utils.h"
#include "../data_manager/data_manager.h"

Category* ScreenFactory::createCategory(FolderType folderType, DataManager* dm) {
    CategoryType categoryType = Utils::folderTypeToCategoryType(folderType);
    Category* category = new Category(categoryType);
    category->setDataManager(dm);
    return category;
}

Folder* ScreenFactory::createFolder(FolderType folderType, const String& categoryName, DataManager* dm) {
    Folder* folder = new Folder(folderType, categoryName);
    folder->setDataManager(dm);
    return folder;
}



Song* ScreenFactory::createSong(const SongInfo* songInfo) {
    return new Song(songInfo);
}

Section* ScreenFactory::createSection() {
    return new Section();
}
