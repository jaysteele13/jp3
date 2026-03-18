# JP3 MP3 Player - Project Context

## Project Overview
ESP32 MP3 player with SSD1306 OLED (128x64), hierarchical navigation through music library (SD card), factory method pattern for screen creation, stack-based navigation.

## Binary Format for SD Card Metadata

**File**: `/jp3/metadata/library.bin`

### Write Order (sequential):
1. Header (40 bytes)
2. String Table (variable)
3. Artist Table (8 bytes × n)
4. Album Table (16 bytes × n)
5. Song Table (24 bytes × n)

### Header (40 bytes at offset 0)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0x00 | 4 | magic bytes | "LIB1" (4 bytes) |
| 0x04 | 4 | version | u32, little-endian |
| 0x08 | 4 | song_count | u32, little-endian |
| 0x0C | 4 | artist_count | u32, little-endian |
| 0x10 | 4 | album_count | u32, little-endian |
| 0x14 | 4 | string_table_offset | u32 (typically 40) |
| 0x18 | 4 | artist_table_offset | u32 |
| 0x1C | 4 | album_table_offset | u32 |
| 0x20 | 4 | song_table_offset | u32 |
| 0x24 | 4 | reserved | 4 bytes |

### Artist Entry (8 bytes)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0x00 | 4 | name_string_id | u32 - lookup in string table |
| 0x04 | 4 | reserved | 4 bytes |

### Album Entry (16 bytes)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0x00 | 4 | name_string_id | u32 |
| 0x04 | 4 | artist_id | u32 |
| 0x08 | 2 | year | u16, little-endian |
| 0x0A | 6 | reserved | 6 bytes |

### Song Entry (24 bytes)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0x00 | 4 | title_string_id | u32 |
| 0x04 | 4 | artist_id | u32 |
| 0x08 | 4 | album_id | u32 |
| 0x0C | 4 | path_string_id | u32 (e.g., "00/001.mp3") |
| 0x10 | 2 | track_number | u16 |
| 0x12 | 2 | duration_sec | u16 |
| 0x14 | 1 | flags | u8 - 0x00=active, 0x01=deleted |
| 0x15 | 3 | reserved | 3 bytes |

### String Table
Format: `[2 bytes: length (u16 LE)] + [N bytes: UTF-8 string]`
String ID = index (0 = first string, etc.)

## Architecture

### Navigation System PATH=src/utils/navigation
- **Factory Method Pattern**: `ScreenFactory` creates screen objects
- **Stack-based Navigation**: `NavigationController` manages screen stack
- **State System**: `NavigationState` handles hierarchical flow logic
- **Screen Hierarchy**: Section → Category → Folder → Song

### Core Components PATH=src/components
- **GUIManager**: Orchestrates display/input (50ms update cycle)
- **ButtonManager**: Handles 4 buttons (Up=26, Down=25, Select=27, Back=32)
- **FileManager**: SD card initialization (VSPI, CS=5)
- **MetadataManager**: Binary metadata parsing with string offset index
- **DataManager**: Data access with sorting (albums/artists)

### Data Organization PATH=src/utils/enums
- **FolderType**: ALBUMS, PLAYLISTS, ARTISTS, ALL_SONGS
- **CategoryType**: ALBUMS, PLAYLISTS, ARTISTS
- **SortMode**: FILE_ORDER, ALPHABETICAL, NEWEST, OLDEST

### GUI Screens PATH=src/utils/gui
- **Section**: Root menu with animated icons (Playlists/Albums/Artists/All Songs)
- **Category**: Lists albums/artists/playlists with scrolling text
- **Folder**: Song lists with selection highlighting
- **Song**: Individual song view with progress bar

## Hardware Configuration
- ESP32 with SSD1306 OLED (I2C: SDA=21, SCL=22, Address=0x3C)
- SD Card: VSPI (MOSI=23, MISO=19, CLK=18, CS=5)
- Buttons: Up=26, Down=25, Select=27, Back=32 (INPUT_PULLUP)
- Serial: 115200 baud

## File Structure
```
src/
├── main.cpp
├── components/
│   ├── button_manager/    # Input handling
│   ├── data_manager/      # Data access & sorting
│   ├── file_manager/      # SD card operations
│   ├── gui_manager/       # Display & navigation orchestration
│   └── metadata_manager/  # Binary metadata parsing
└── utils/
    ├── enums/              # Types, paths, offsets
    ├── gui/                # Screen implementations
    ├── navigation/        # Navigation controller & states
    ├── validation/        # Text scrolling
    └── dummy_data/         # Fallback test data
```

## Build System
- **PlatformIO**: `pio run`, `pio upload`
- **Framework**: Arduino for ESP32
- **Libraries**: Adafruit SSD1306, Adafruit GFX

## Key Constraints
- Non-blocking: 50ms update intervals, no Arduino Delay
- Embedded: Minimal heap usage, static allocation preferred
- No RTTI: Use ScreenType enum for type identification

## MetadataManager Implementation
- Builds string offset index on init() for O(1) lookup
- Loads album/artist entries into RAM on init()
- MAX_STRINGS=6000, MAX_ALBUMS=2000, MAX_ARTISTS=2000
- readStringById() uses pre-built offset index
