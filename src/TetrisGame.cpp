#include "TetrisGame.h"
#include <thread>
#include <algorithm>
#include <iostream>

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TetrisGame::TetrisGame(IInputHandler& inputHandler, IConfigManager& configManager)
    : input(inputHandler)
    , config(configManager)
    , highScore(configManager.loadHighScore())
    , state(GameState::Menu)
    , dropInterval(std::chrono::milliseconds(Constants::INITIAL_DROP_SPEED_MS))
    , lastDropTime(std::chrono::steady_clock::now())
    , lastRenderedState(GameState::Menu)
    , needsStaticRedraw(true)
{
}

// ---------------------------------------------------------------------------
// Main game loop
// ---------------------------------------------------------------------------

void TetrisGame::run() {
    // Force an initial render of the menu
    renderer.clear();

    while (true) {
        // ── Update phase ──────────────────────────────────────────────── //
        update();

        // ── Render phase ──────────────────────────────────────────────── //
        render();

        // ── Frame timing ─────────────────────────────────────────────── //
        std::this_thread::sleep_for(
            std::chrono::milliseconds(Constants::FRAME_TIME_MS));
    }
}

// ---------------------------------------------------------------------------
// State machine
// ---------------------------------------------------------------------------

void TetrisGame::transitionTo(GameState newState) {
    if (state == newState) return;

    switch (newState) {
        case GameState::Menu:
            // Reset all game data when returning to menu
            scoreSystem.reset();
            dropInterval = std::chrono::milliseconds(Constants::INITIAL_DROP_SPEED_MS);
            board.clear();
            break;

        case GameState::Playing:
            if (state == GameState::Menu) {
                // Fresh game start: reset everything and spawn first piece
                scoreSystem.reset();
                dropInterval = std::chrono::milliseconds(Constants::INITIAL_DROP_SPEED_MS);
                board.clear();
                spawnPiece();
            }
            // Resume from Paused: no reset needed
            lastDropTime = std::chrono::steady_clock::now();
            break;

        case GameState::Paused:
            // No special setup needed for pause
            break;

        case GameState::GameOver:
            // Check and persist high score before showing game over screen
            checkAndSaveHighScore();
            break;
    }

    state = newState;

    // Screen needs to be redrawn on every state change
    renderer.clear();
    lastRenderedState  = newState;
    needsStaticRedraw  = true;
}

// ---------------------------------------------------------------------------
// Update  (dispatches to per-state helpers)
// ---------------------------------------------------------------------------

void TetrisGame::update() {
    switch (state) {
        case GameState::Menu:     updateMenu();     break;
        case GameState::Playing:  updatePlaying();  break;
        case GameState::Paused:   updatePaused();   break;
        case GameState::GameOver: updateGameOver(); break;
    }
}

void TetrisGame::updateMenu() {
    input.update();

    if (input.wasJustConfirmPressed() || input.wasJustHardDropPressed()) {
        transitionTo(GameState::Playing);
    } else if (input.wasJustQuitPressed()) {
        ExitProcess(0);
    }
}

void TetrisGame::updatePlaying() {
    // ── Input ──
    input.update();
    handlePlayingInput();

    // We may have transitioned away during input (e.g., ESC -> pause)
    if (state != GameState::Playing) return;

    // ── Automatic drop ──
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDropTime);

    if (elapsed >= dropInterval) {
        dropPiece();
        lastDropTime = now;
    }
}

void TetrisGame::updatePaused() {
    input.update();

    if (input.wasJustPausePressed() || input.wasJustQuitPressed()) {
        transitionTo(GameState::Playing);
    }
}

void TetrisGame::updateGameOver() {
    input.update();

    if (input.wasJustConfirmPressed() || input.wasJustHardDropPressed()) {
        // Enter or Space — return to menu
        transitionTo(GameState::Menu);
    } else if (input.wasJustQuitPressed()) {
        ExitProcess(0);
    }
}

// ---------------------------------------------------------------------------
// Render  (dispatches to per-state helpers)
// ---------------------------------------------------------------------------

void TetrisGame::render() {
    switch (state) {
        case GameState::Menu:     renderMenu();     break;
        case GameState::Playing:  renderPlaying();  break;
        case GameState::Paused:   renderPaused();   break;
        case GameState::GameOver: renderGameOver(); break;
    }
}

void TetrisGame::renderMenu() {
    if (!needsStaticRedraw) return;
    needsStaticRedraw = false;

    std::cout << "\n\n";
    std::cout << "  \xe2\x95\x94\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x97\n";
    std::cout << "  \xe2\x95\x91      B L I X          \xe2\x95\x91\n";
    std::cout << "  \xe2\x95\x91   (Tetris Clone)      \xe2\x95\x91\n";
    std::cout << "  \xe2\x95\x91                       \xe2\x95\x91\n";

    char hsLine[48];
    snprintf(hsLine, sizeof(hsLine), "  \xe2\x95\x91  Best: %-14d\xe2\x95\x91", highScore);
    std::cout << hsLine << "\n";

    std::cout << "  \xe2\x95\x91                       \xe2\x95\x91\n";
    std::cout << "  \xe2\x95\x91  Enter / Space: Play  \xe2\x95\x91\n";
    std::cout << "  \xe2\x95\x91  Escape:        Quit  \xe2\x95\x91\n";
    std::cout << "  \xe2\x95\x9a\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x9d\n";
}

void TetrisGame::renderPlaying() {
    const Tetromino* piece = pieceManager.hasCurrentPiece()
                           ? &pieceManager.getCurrentPiece()
                           : nullptr;

    renderer.render(board, piece, scoreSystem.getScore(), scoreSystem.getLevel(),
                    scoreSystem.getLinesCleared(), pieceManager.getNextPieceType());
}

void TetrisGame::renderPaused() {
    renderer.renderPauseOverlay();
}

void TetrisGame::renderGameOver() {
    if (!needsStaticRedraw) return;
    needsStaticRedraw = false;

    renderer.renderGameOverOverlay(scoreSystem.getScore(), highScore);
}

// ---------------------------------------------------------------------------
// High score
// ---------------------------------------------------------------------------

void TetrisGame::checkAndSaveHighScore() {
    const int current = scoreSystem.getScore();
    if (current > highScore) {
        highScore = current;
        config.saveHighScore(highScore);
    }
}

// ---------------------------------------------------------------------------
// Piece lifecycle
// ---------------------------------------------------------------------------

void TetrisGame::spawnPiece() {
    pieceManager.spawnNewPiece();

    // If the spawn position is already occupied the game is over
    if (!board.isValidPosition(pieceManager.getCurrentPiece())) {
        transitionTo(GameState::GameOver);
    }
}

void TetrisGame::dropPiece() {
    if (!pieceManager.moveDown(board)) {
        lockPiece();
    }
}

void TetrisGame::lockPiece() {
    board.fixTetromino(pieceManager.getCurrentPiece());
    clearLines();
    spawnPiece();  // spawnPiece handles game-over detection
}

// ---------------------------------------------------------------------------
// Scoring / level helpers  (delegated to ScoreSystem)
// ---------------------------------------------------------------------------

void TetrisGame::clearLines() {
    auto filledRows = board.getFilledRows();
    if (filledRows.empty()) return;

    int numLines = board.clearRows(filledRows);

    // Delegate scoring and level progression to ScoreSystem
    scoreSystem.addLines(numLines);

    // Sync the drop interval with the (possibly updated) level
    updateDropSpeed();
}

void TetrisGame::updateDropSpeed() {
    dropInterval = std::chrono::milliseconds(scoreSystem.getDropSpeed());
}

int TetrisGame::getDropSpeed() const {
    return scoreSystem.getDropSpeed();
}

// ---------------------------------------------------------------------------
// Input helpers
// ---------------------------------------------------------------------------

void TetrisGame::handlePlayingInput() {
    // Pause (ESC or P via wasJustPausePressed)
    if (input.wasJustPausePressed()) {
        transitionTo(GameState::Paused);
        return;
    }

    // Left
    if (input.wasJustLeftPressed()) {
        pieceManager.moveLeft(board);
    }

    // Right
    if (input.wasJustRightPressed()) {
        pieceManager.moveRight(board);
    }

    // Down (soft drop – held key is fine)
    if (input.isDownPressed()) {
        if (pieceManager.moveDown(board)) {
            scoreSystem.addBonus(1);
        }
    }

    // Rotate
    if (input.wasJustRotatePressed()) {
        pieceManager.rotate(board);
    }

    // Hard drop
    if (input.wasJustHardDropPressed()) {
        int rows = pieceManager.hardDrop(board);
        scoreSystem.addBonus(rows * 2);
        lockPiece();
        lastDropTime = std::chrono::steady_clock::now();
    }
}
