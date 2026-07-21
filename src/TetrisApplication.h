#pragma once
#include <windows.h>
#include <string>

// Forward declaration – TetrisApplication owns a TetrisGame instance
class TetrisGame;

// TetrisApplication wraps all Windows console setup and teardown,
// then delegates to TetrisGame for the game loop.
//
// Responsibilities:
//   - Set the console window title
//   - Resize the console screen buffer and window to a suitable size
//   - Hide the blinking cursor for cleaner rendering
//   - Register a Ctrl+C / Ctrl+Break / window-close handler so the
//     process saves state (high score) before exiting unexpectedly
//   - Automatically pause gameplay when the console window loses focus
//   - Instantiate TetrisGame and call run()
//
// Requirements addressed: 9.1, 9.2, 9.3, 9.4
class TetrisApplication {
public:
    TetrisApplication();
    ~TetrisApplication();

    // Perform all console setup then start the game loop.
    void run();

private:
    // Console configuration helpers
    void setupConsole();
    void setWindowTitle(const std::wstring& title);
    void setConsoleBufferAndWindowSize(SHORT bufferWidth, SHORT bufferHeight);
    void hideCursor();

    // Ctrl+C / Ctrl+Break / CLOSE handler (Req 9.3)
    // Registered with SetConsoleCtrlHandler; called on the signal thread.
    static BOOL WINAPI consoleCtrlHandler(DWORD ctrlType);

    // Tracks whether the game has been initialised so the ctrl handler can
    // safely invoke a save even if TetrisApplication::run() is mid-flight.
    static TetrisApplication* s_instance;

    // Owned game instance – created inside run() after console setup.
    TetrisGame* game;
};
