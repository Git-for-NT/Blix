# Tetris Game

A classic Tetris implementation for Windows using C++.

## Building the Project

### Prerequisites
- Visual Studio 2022 (or compatible C++ compiler with C++17 support)
- Windows 10 or higher

### Build Instructions

#### Option 1: Using Visual Studio (Recommended)
1. Open `TetrisGame.sln` in Visual Studio
2. Select Release configuration (or Debug for debugging)
3. Build the solution (Ctrl+Shift+B or Build > Build Solution)
4. The executable will be in `bin/Release/TetrisGame.exe`

#### Option 2: Using MSBuild from Command Line
```cmd
msbuild TetrisGame.sln /p:Configuration=Release /p:Platform=x64
```

#### Option 3: Using CMake (if installed)
```cmd
build.bat
```

### Running the Game

```cmd
bin\Release\TetrisGame.exe
```

## Project Structure

```
tetris-game/
├── src/
│   ├── main.cpp           # Entry point
│   ├── Tetromino.h/cpp    # Tetromino piece definitions
│   └── Constants.h        # Game constants
├── CMakeLists.txt         # Build configuration
├── build.bat              # Windows build script
└── README.md              # This file
```

## Development Status

This is the initial minimal setup. The project currently includes:
- Basic tetromino data structures
- Shape definitions for all 7 piece types
- Rotation logic
- Build system configuration

Next steps will add game board, input handling, rendering, and game logic incrementally.
