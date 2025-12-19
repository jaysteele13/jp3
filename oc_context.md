# ESP32 MP3 Player - Project Context & Principles

## Project Overview
Lightweight MP3 player using ESP32 with OLED display (128x64 SSD1306). Features hierarchical navigation through music library with Factory Method pattern for screen creation and stack-based navigation system with state management.

## Architecture

use PATH to get to files to read them.

### Navigation System
PATH=src/utils/navigation
- **Factory Method Pattern**: `ScreenFactory` creates screen objects (Section, Category, Folder, Song)
- **Stack-based Navigation**: `NavigationController` manages screen stack with animations 
- **State System**: `NavigationState` handles hierarchical flow logic without RTTI
- **Screen Hierarchy**: Section → Category → Folder → Song

### Core Components
- **GUIManager**: Orchestrates display/input, handles button navigation (50ms update cycle) PATH=src/components/gui_manager
- **ScreenBase**: Abstract base for all screens with type identification via `ScreenType` enum PATH=src/utils/navigation
- **ButtonManager**: Handles input from physical buttons PATH=src/components/button_manager
- **Screen Types**: Section (albums/playlists/artists), Category (albums/artists/playlists), Folder (song lists), Song (individual song) PATH=src/utils/gui

### Data Organization
enums found in PATH=src/utils/enums
- **FolderType**: ALBUMS, PLAYLISTS, ARTISTS, ALL_SONGS
- **CategoryType**: ALBUMS, PLAYLISTS, ARTISTS  
- **SongInfo**: songName, artistName, albumName, playlistName, duration

## Hardware Configuration
- ESP32 with SSD1306 OLED (I2C: SDA=21, SCL=22, Address=0x3C)
- Input buttons managed by ButtonManager
- Serial output for debugging (115200 baud)

## Development Principles

### Code Standards
- **No RTTI**: Use `ScreenType` enum for type identification instead of `dynamic_cast`
- **Error Handling**: All navigation methods return `NavResult` enum for consistent error handling
- **Memory Management**: NavigationController does NOT own screen objects - caller manages lifecycle
- **Non-blocking**: Fixed 50ms update intervals, all operations must be non-blocking. No use of Arduino Delay!!

### File Structure
```
src/
├── components/
│   ├── gui_manager/     # Main GUI orchestration
│   └── button_manager/   # Input handling
├── utils/
│   ├── gui/             # Screen implementations
│   ├── navigation/       # Navigation system
│   └── enums/           # Shared data structures
```

### Build System
- **PlatformIO**: Primary build system (replaces Arduino CLI)
- **Scripts**: compile.sh, upload.sh for convenience
- **Target**: ESP32 (esp32:esp32:esp32)

### Navigation Flow
1. Root screen shows sections (Albums, Playlists, Artists, All Songs)
2. Select section → Category screen (shows album/artist/playlist names)
3. Select category → Folder screen (shows song list)
4. Select song → Song screen (individual song view)
5. Back button reverses navigation with stack pop

## Key Constraints
- Embedded environment: Minimal memory usage, no dynamic memory allocation in hot paths
- Real-time: All UI operations must complete within 50ms update window
- Resource constraints: Avoid expensive operations, use enums instead of strings where possible

## Development Guidelines
- Follow existing naming conventions (camelCase for methods, PascalCase for classes)
- Use Serial.println for debugging with appropriate log levels (INFO, ERROR)
- All screen creation must go through ScreenFactory
- Navigation errors must be handled gracefully with NavResult return codes