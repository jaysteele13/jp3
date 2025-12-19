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