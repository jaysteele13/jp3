#ifndef SCREEN_FACTORY_H
#define SCREEN_FACTORY_H

#include "Arduino.h"
#include "../../utils/gui/section/section.h"
#include "../../utils/gui/category/category.h"
#include "../../utils/gui/folder/folder.h"
#include "../../utils/gui/song/song.h"
#include "../../utils/enums/enums.h"

class ScreenFactory {
public:
    static Category* createCategory(FolderType folderType);
    static Folder* createFolder(FolderType folderType, const String& categoryName);
    static Song* createSong(const SongInfo* songInfo);
    static Section* createSection();
};

#endif