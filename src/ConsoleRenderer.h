#pragma once
#include "IRenderer.h"
#include "GameBoard.h"
#include "Tetromino.h"
#include "ScoreSystem.h"
#include <string>
#include <array>
#include <windows.h>

// Concrete IRenderer implementation using Windows Console API.
// Uses SetConsoleTextAttribute for per-tetromino-type colors and cursor
// repositioning (rather than system("cls")) to eliminate flicker.
class ConsoleRenderer : public IRenderer {
public:
    ConsoleRenderer();
    ~ConsoleRenderer() override;

    // IRenderer implementation
    void renderBoard(const GameBoard& board) override;
    void renderTetromino(const Tetromino& tetromino) override;
    void renderScore(const ScoreSystem& score) override;
    void renderNextPiece(TetrominoType type) override;
    void renderPauseOverlay() override;
    void renderGameOverOverlay(int finalScore) override;
    void renderMenu() override;
    void clear() override;
    void present() override;

    // Full-frame composite render — convenience method combining board + piece + HUD.
    // Used directly by TetrisGame (which holds ConsoleRenderer by value, not via interface).
    void render(const GameBoard& board, const Tetromino* currentPiece,
                int score, int level, int lines, TetrominoType nextPiece);

private:
    HANDLE  hConsole;           // STD_OUTPUT_HANDLE
    WORD    defaultAttrib;      // Original console color attributes (restored on exit)

    // Cached state so individual render* calls can compose the full frame
    const GameBoard*  cachedBoard    = nullptr;
    const Tetromino*  cachedPiece    = nullptr;
    int               cachedScore    = 0;
    int               cachedLevel    = 1;
    int               cachedLines    = 0;
    TetrominoType     cachedNext     = TetrominoType::I;

    // Cursor helpers
    void setCursorPosition(int x, int y);
    void setCursorVisible(bool visible);

    // Color helpers
    WORD  colorForType(TetrominoType type) const;
    void  setColor(WORD attrib);
    void  resetColor();

    // Character helpers
    char getCharForType(TetrominoType type) const;
    std::array<Position, 4> getPreviewShape(TetrominoType type) const;

    // Character pair rendered for one board cell (2 chars wide)
    const char* blockStr() const { return "[]"; }
    const char* emptyStr() const { return "  "; }

    // Right-panel content for a given board row (0-based).
    // Returns a fixed-width string (padded with spaces) so stale text is erased.
    std::string sidePanelLine(int row, int score, int level, int lines,
                              TetrominoType nextPiece) const;
};
