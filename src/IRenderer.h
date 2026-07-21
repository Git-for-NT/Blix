#pragma once
#include "GameBoard.h"
#include "Tetromino.h"
#include "ScoreSystem.h"

// Abstract interface for rendering.
// Implement this to support different rendering backends (console, GDI, test stubs).
class IRenderer {
public:
    virtual ~IRenderer() = default;

    // Render the fixed blocks on the board
    virtual void renderBoard(const GameBoard& board) = 0;

    // Render the currently active (falling) tetromino
    virtual void renderTetromino(const Tetromino& tetromino) = 0;

    // Render the score/level/lines-cleared HUD
    virtual void renderScore(const ScoreSystem& score) = 0;

    // Render the next-piece preview
    virtual void renderNextPiece(TetrominoType type) = 0;

    // Overlays
    virtual void renderPauseOverlay() = 0;
    virtual void renderGameOverOverlay(int finalScore) = 0;

    // Main menu screen
    virtual void renderMenu() = 0;

    // Clear the screen / back-buffer before drawing a new frame
    virtual void clear() = 0;

    // Flush / swap buffers — call once per frame after all render* calls
    virtual void present() = 0;
};
