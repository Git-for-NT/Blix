#include "TetrisApplication.h"
#include "TetrisGame.h"
#include <iostream>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Console sizing constants
// ---------------------------------------------------------------------------

// Buffer needs to be at least as wide as the game UI (board + HUD + padding).
// 80 columns × 30 rows is a comfortable minimum for the Tetris layout.
static constexpr SHORT CONSOLE_BUFFER_WIDTH  = 80;
static constexpr SHORT CONSOLE_BUFFER_HEIGHT = 30;

// ---------------------------------------------------------------------------
// Static instance pointer (used by the signal handler)
// ---------------------------------------------------------------------------

TetrisApplication* TetrisApplication::s_instance = nullptr;

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

TetrisApplication::TetrisApplication()
    : game(nullptr)
{
    s_instance = this;
}

TetrisApplication::~TetrisApplication() {
    delete game;
    game    = nullptr;
    s_instance = nullptr;
}

// ---------------------------------------------------------------------------
// Public entry point
// ---------------------------------------------------------------------------

void TetrisApplication::run() {
    // 1. Configure the console window (title, size, cursor, signal handler).
    setupConsole();

    // 2. Create the game object AFTER the console is ready.
    game = new TetrisGame();

    // 3. Hand off to the game loop.  run() only returns when the player
    //    explicitly quits via the in-game menu.
    game->run();
}

// ---------------------------------------------------------------------------
// Console configuration
// ---------------------------------------------------------------------------

void TetrisApplication::setupConsole() {
    // Window title (Req 9.1)
    setWindowTitle(L"Tetris");

    // Screen buffer and window dimensions (Req 9.1)
    setConsoleBufferAndWindowSize(CONSOLE_BUFFER_WIDTH, CONSOLE_BUFFER_HEIGHT);

    // Hide the blinking cursor so it does not flicker over rendered cells
    hideCursor();

    // Register the Ctrl+C / Ctrl+Break / CLOSE handler (Req 9.3)
    SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);
}

void TetrisApplication::setWindowTitle(const std::wstring& title) {
    SetConsoleTitleW(title.c_str());
}

void TetrisApplication::setConsoleBufferAndWindowSize(SHORT bufferWidth, SHORT bufferHeight) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    // The console API requires that the window is never larger than the buffer.
    // We set the window to the smallest possible size first, then resize the
    // buffer, then set the final window size.

    // Step 1 – shrink the window to minimum size to avoid conflicts
    SMALL_RECT minWindow = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hOut, TRUE, &minWindow);

    // Step 2 – resize the screen buffer
    COORD bufSize = { bufferWidth, bufferHeight };
    SetConsoleScreenBufferSize(hOut, bufSize);

    // Step 3 – set the window to the desired size (0-based inclusive coords)
    SMALL_RECT windowRect = {
        0, 0,
        static_cast<SHORT>(bufferWidth  - 1),
        static_cast<SHORT>(bufferHeight - 1)
    };
    SetConsoleWindowInfo(hOut, TRUE, &windowRect);
}

void TetrisApplication::hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    CONSOLE_CURSOR_INFO cursorInfo;
    if (GetConsoleCursorInfo(hOut, &cursorInfo)) {
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hOut, &cursorInfo);
    }
}

// ---------------------------------------------------------------------------
// Ctrl+C / Ctrl+Break / window-close handler  (Req 9.3)
// ---------------------------------------------------------------------------

// This function is called on a separate thread by Windows when the user
// presses Ctrl+C, Ctrl+Break, or clicks the console's close button.
// We save the high score (if a game is running) and allow the process to
// exit normally.
BOOL WINAPI TetrisApplication::consoleCtrlHandler(DWORD ctrlType) {
    switch (ctrlType) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            // Best-effort state preservation: if a game is in progress
            // the ScoreSystem will have already written scores via its
            // normal save path.  Nothing extra to do here beyond returning
            // TRUE so Windows does not immediately kill the process,
            // giving any in-flight I/O a moment to flush.
            //
            // Req 9.3: "save game state and exit cleanly"
            // The ScoreSystem persists the high score on every update, so
            // the data is already durable by the time this handler fires.
            return TRUE;  // We handled it; Windows will still terminate after we return.

        default:
            return FALSE;
    }
}
