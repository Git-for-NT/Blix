#pragma once
#include "GameBoard.h"
#include "TetrominoManager.h"
#include "ConsoleRenderer.h"
#include "ScoreSystem.h"
#include "IInputHandler.h"
#include "IConfigManager.h"
#include <chrono>

enum class GameState {
    Menu,
    Playing,
    Paused,
    GameOver
};

class TetrisGame {
public:
    // inputHandler and configManager must outlive TetrisGame (owned by TetrisApplication).
    explicit TetrisGame(IInputHandler& inputHandler, IConfigManager& configManager);

    void run();

    // Accessors for testing / external inspection
    GameState getState()        const { return state; }
    int getScore()              const { return scoreSystem.getScore(); }
    int getLevel()              const { return scoreSystem.getLevel(); }
    int getLinesCleared()       const { return scoreSystem.getLinesCleared(); }
    int getHighScore()          const { return highScore; }

private:
    // ------------------------------------------------------------------ //
    //  Components
    // ------------------------------------------------------------------ //
    GameBoard board;
    TetrominoManager pieceManager;
    ConsoleRenderer renderer;
    ScoreSystem scoreSystem;

    // ------------------------------------------------------------------ //
    //  State machine
    // ------------------------------------------------------------------ //
    GameState state;

    // Transition to a new state, resetting any state-specific variables
    void transitionTo(GameState newState);

    // ------------------------------------------------------------------ //
    //  Scoring / progression  (now delegated to ScoreSystem)
    // ------------------------------------------------------------------ //
    // (score, level, linesCleared are accessed via scoreSystem)

    // ------------------------------------------------------------------ //
    //  Drop timing
    // ------------------------------------------------------------------ //
    std::chrono::milliseconds dropInterval;
    std::chrono::steady_clock::time_point lastDropTime;

    // ------------------------------------------------------------------ //
    //  Game-loop phases  (update / render separation)
    // ------------------------------------------------------------------ //
    void update();
    void render();

    // State-specific update helpers
    void updateMenu();
    void updatePlaying();
    void updatePaused();
    void updateGameOver();

    // State-specific render helpers
    void renderMenu();
    void renderPlaying();
    void renderPaused();
    void renderGameOver();

    // ------------------------------------------------------------------ //
    //  Piece lifecycle
    // ------------------------------------------------------------------ //
    void spawnPiece();   // Spawn next piece; triggers GameOver on collision
    void dropPiece();    // Attempt automatic drop; lock on failure
    void lockPiece();    // Fix piece, clear lines, spawn next

    // ------------------------------------------------------------------ //
    //  Scoring helpers
    // ------------------------------------------------------------------ //
    void clearLines();
    void updateDropSpeed();
    int  getDropSpeed() const;

    // ------------------------------------------------------------------ //
    //  Input
    // ------------------------------------------------------------------ //
    IInputHandler&  input;    // injected; owned by TetrisApplication
    IConfigManager& config;   // injected; owned by TetrisApplication

    void handlePlayingInput();

    // ------------------------------------------------------------------ //
    //  High score
    // ------------------------------------------------------------------ //
    int highScore;   // loaded from config on construction, updated on game over

    void checkAndSaveHighScore();

    // Track whether the last render cleared the screen (avoid redundant cls).
    // Set to true by transitionTo(); cleared after the one-time static draw.
    GameState lastRenderedState;
    bool      needsStaticRedraw;  // true whenever a state transition just occurred
};
