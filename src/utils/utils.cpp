#include "utils.h"

FolderType Utils::categoryTypeToFolderType(CategoryType categoryType) {
        switch (categoryType) {
            case CategoryType::ALBUMS:
                return FolderType::ALBUMS;
            case CategoryType::ARTISTS:
                return FolderType::ARTISTS;
            case CategoryType::PLAYLISTS:
                return FolderType::PLAYLISTS;
            default:
                return FolderType::ALL_SONGS; // Fallback
        }
    }

CategoryType Utils::folderTypeToCategoryType(FolderType folderType) {
        switch (folderType) {
            case FolderType::ALBUMS:
                return CategoryType::ALBUMS;
            case FolderType::ARTISTS:
                return CategoryType::ARTISTS;
            case FolderType::PLAYLISTS:
                return CategoryType::PLAYLISTS;
            default:
                return CategoryType::ALBUMS; // Fallback
        }
    }