#include "ConsoleRenderer.h"
#include "Constants.h"
#include <iostream>
#include <windows.h>

ConsoleRenderer::ConsoleRenderer() {
    // Set console to UTF-8 for better character support
    SetConsoleOutputCP(CP_UTF8);
    hConsole     = GetStdHandle(STD_OUTPUT_HANDLE);
    defaultAttrib = 7;  // default white-on-black; updated after first color read
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        defaultAttrib = csbi.wAttributes;
    }
}

ConsoleRenderer::~ConsoleRenderer() {
    // Restore original console text attributes on destruction
    if (hConsole != INVALID_HANDLE_VALUE) {
        SetConsoleTextAttribute(hConsole, defaultAttrib);
    }
}

// ---------------------------------------------------------------------------
// IRenderer interface implementation
// ---------------------------------------------------------------------------

void ConsoleRenderer::clear() {
    system("cls");
}

void ConsoleRenderer::present() {
    // Console output is immediate; nothing to flush/swap.
}

void ConsoleRenderer::renderBoard(const GameBoard& board) {
    cachedBoard = &board;
    // Full composite render is deferred to present() or triggered via render()
    // when all cached state is available.
}

void ConsoleRenderer::renderTetromino(const Tetromino& tetromino) {
    cachedPiece = &tetromino;
}

void ConsoleRenderer::renderScore(const ScoreSystem& score) {
    cachedScore = score.getScore();
    cachedLevel = score.getLevel();
    cachedLines = score.getLinesCleared();
}

void ConsoleRenderer::renderNextPiece(TetrominoType type) {
    cachedNext = type;

    // If we have all the data needed, draw the full frame now.
    if (cachedBoard) {
        render(*cachedBoard, cachedPiece, cachedScore, cachedLevel, cachedLines, cachedNext);
    }
}

void ConsoleRenderer::renderPauseOverlay() {
    // Centre the overlay on the board.
    // Board columns: 1 border + 10*2 chars = 21 chars wide (col 0..20).
    // Target row: roughly mid-board (row 10 of 20).
    // We overlay three lines; the middle one is centred within the 20-char board area.
    //   "┌─────────────┐"  (15 chars)
    //   "│   PAUSED    │"
    //   "└─────────────┘"
    // Left offset so it sits inside the board borders (board left border is col 0).
    // Board inner width = 10*2 = 20 chars.  Box width = 15.  Padding = (20-15)/2 = 2 => col 3.
    const int col = 3;
    const int row = 9;  // rows 9-11 are mid-board

    setCursorPosition(col, row);
    std::cout << "\xe2\x94\x8c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x90";  // ┌─────────────┐

    setCursorPosition(col, row + 1);
    std::cout << "\xe2\x94\x82  ** PAUSED **  \xe2\x94\x82";  // │  ** PAUSED **  │

    setCursorPosition(col, row + 2);
    std::cout << "\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x98";  // └─────────────┘
}

void ConsoleRenderer::renderGameOverOverlay(int finalScore) {
    // Position the overlay below the board (board ends at row ~22 counting header).
    // We use setCursorPosition so subsequent frames overwrite cleanly.
    const int col = 0;
    const int row = 22;  // just below the board bottom border

    // Box is 24 chars wide: "╔══════════════════════╗"
    setCursorPosition(col, row);
    std::cout << "  \xe2\x95\x94\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x97";
    setCursorPosition(col, row + 1);
    std::cout << "  \xe2\x95\x91    ** GAME  OVER **   \xe2\x95\x91";
    setCursorPosition(col, row + 2);
    std::cout << "  \xe2\x95\x91                     \xe2\x95\x91";
    setCursorPosition(col, row + 3);

    // Format the score line so it is always padded to fill the box width
    char scoreLine[32];
    snprintf(scoreLine, sizeof(scoreLine), "  \xe2\x95\x91   Final Score: %-6d\xe2\x95\x91", finalScore);
    std::cout << scoreLine;

    setCursorPosition(col, row + 4);
    std::cout << "  \xe2\x95\x91                     \xe2\x95\x91";
    setCursorPosition(col, row + 5);
    std::cout << "  \xe2\x95\x91  Enter: Restart      \xe2\x95\x91";
    setCursorPosition(col, row + 6);
    std::cout << "  \xe2\x95\x91  ESC:   Quit         \xe2\x95\x91";
    setCursorPosition(col, row + 7);
    std::cout << "  \xe2\x95\x9a\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x9d";
}

void ConsoleRenderer::renderMenu() {
    clear();
    std::cout << "\n\n";
    std::cout << "  +===================+\n";
    std::cout << "  |     B L I X       |\n";
    std::cout << "  |   (Tetris Clone)  |\n";
    std::cout << "  +===================+\n\n";
    std::cout << "  [P] Play\n";
    std::cout << "  [Q] Quit\n\n";
    std::cout << "  Enter choice: ";
}

// ---------------------------------------------------------------------------
// Internal composite renderer
// ---------------------------------------------------------------------------

void ConsoleRenderer::render(const GameBoard& board, const Tetromino* currentPiece,
                             int score, int level, int lines, TetrominoType nextPiece) {
    setCursorPosition(0, 0);

    std::cout << "\xe2\x95\x94\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x97  TETRIS\n";

    for (int y = 0; y < Constants::BOARD_HEIGHT; ++y) {
        std::cout << "\xe2\x95\x91";

        for (int x = 0; x < Constants::BOARD_WIDTH; ++x) {
            bool isCurrentPiece = false;

            if (currentPiece) {
                auto blocks = currentPiece->getBlockPositions();
                for (const auto& block : blocks) {
                    if (block.x == x && block.y == y) {
                        isCurrentPiece = true;
                        break;
                    }
                }
            }

            if (isCurrentPiece) {
                std::cout << getCharForType(currentPiece->type) << " ";
            } else if (board.isOccupied(x, y)) {
                auto cell = board.getCell(x, y);
                if (cell.has_value()) {
                    std::cout << getCharForType(cell.value()) << " ";
                } else {
                    std::cout << "  ";
                }
            } else {
                std::cout << "  ";
            }
        }

        std::cout << "\xe2\x95\x91";

        // Side panel HUD
        if (y == 2)  std::cout << "  Score: " << score;
        else if (y == 4) std::cout << "  Level: " << level;
        else if (y == 6) std::cout << "  Lines: " << lines;
        else if (y == 9) {
            std::cout << "  Next:";
        } else if (y >= 10 && y <= 13) {
            // Draw the 4-row preview grid for the next piece
            int previewRow = y - 10;  // 0..3
            auto shape = getPreviewShape(nextPiece);

            std::cout << "  ";
            for (int px = 0; px < 4; ++px) {
                bool filled = false;
                for (const auto& block : shape) {
                    if (block.x == px && block.y == previewRow) {
                        filled = true;
                        break;
                    }
                }
                std::cout << (filled ? (std::string(1, getCharForType(nextPiece)) + " ") : "  ");
            }
        }

        std::cout << "\n";
    }

    std::cout << "\xe2\x95\x9a\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x9d\n";
    std::cout << "\nControls: \xe2\x86\x90 \xe2\x86\x92 \xe2\x86\x93 (move)  \xe2\x86\x91 (rotate)  Space (drop)  ESC (quit)\n";
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

char ConsoleRenderer::getCharForType(TetrominoType type) const {
    switch (type) {
        case TetrominoType::I: return 'I';
        case TetrominoType::O: return 'O';
        case TetrominoType::T: return 'T';
        case TetrominoType::S: return 'S';
        case TetrominoType::Z: return 'Z';
        case TetrominoType::J: return 'J';
        case TetrominoType::L: return 'L';
        default:               return '?';
    }
}

// Returns the 4 relative block positions for a type at rotation 0.
// Mirrors the shape data in Tetromino.cpp so we don't need a full Tetromino object.
std::array<Position, 4> ConsoleRenderer::getPreviewShape(TetrominoType type) const {
    switch (type) {
        case TetrominoType::I:
            return {Position{0,1}, Position{1,1}, Position{2,1}, Position{3,1}};
        case TetrominoType::O:
            return {Position{1,1}, Position{2,1}, Position{1,2}, Position{2,2}};
        case TetrominoType::T:
            return {Position{1,0}, Position{0,1}, Position{1,1}, Position{2,1}};
        case TetrominoType::S:
            return {Position{1,0}, Position{2,0}, Position{0,1}, Position{1,1}};
        case TetrominoType::Z:
            return {Position{0,0}, Position{1,0}, Position{1,1}, Position{2,1}};
        case TetrominoType::J:
            return {Position{0,0}, Position{0,1}, Position{1,1}, Position{2,1}};
        case TetrominoType::L:
            return {Position{2,0}, Position{0,1}, Position{1,1}, Position{2,1}};
        default:
            return {Position{0,0}, Position{1,0}, Position{2,0}, Position{3,0}};
    }
}

void ConsoleRenderer::setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = static_cast<SHORT>(x);
    coord.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
