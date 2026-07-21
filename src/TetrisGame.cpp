#include "TetrisGame.h"
#include <windows.h>
#include <thread>
#include <algorithm>
#include <iostream>

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TetrisGame::TetrisGame()
    : state(GameState::Menu)
    , dropInterval(std::chrono::milliseconds(Constants::INITIAL_DROP_SPEED_MS))
    , lastDropTime(std::chrono::steady_clock::now())
    , lastRenderedState(GameState::Menu)
{
    for (int i = 0; i < 256; ++i) {
        prevKeyStates[i] = false;
    }
    // The first piece is spawned when transitioning to Playing, not here.
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
            // No special setup; final state is displayed until user acts
            break;
    }

    state = newState;

    // Screen needs to be redrawn on every state change
    renderer.clear();
    lastRenderedState = newState;
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
    // Refresh key-state snapshot so wasKeyJustPressed works on the menu
    for (int i = 0; i < 256; ++i) {
        prevKeyStates[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    if (wasKeyJustPressed(VK_RETURN) || wasKeyJustPressed(' ')) {
        transitionTo(GameState::Playing);
    } else if (wasKeyJustPressed(VK_ESCAPE)) {
        ExitProcess(0);
    }
}

void TetrisGame::updatePlaying() {
    // ── Input ──
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
    if (wasKeyJustPressed('P') || wasKeyJustPressed(VK_ESCAPE)) {
        transitionTo(GameState::Playing);
    }
}

void TetrisGame::updateGameOver() {
    if (wasKeyJustPressed(VK_RETURN)) {
        transitionTo(GameState::Menu);
    } else if (wasKeyJustPressed(VK_ESCAPE)) {
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
    // Only repaint if we just entered this state (transitionTo already called
    // clear(); subsequent frames re-draw in-place via setCursorPosition).
    static bool menuPrinted = false;
    if (!menuPrinted || lastRenderedState != GameState::Menu) {
        menuPrinted = true;
        std::cout << "\n\n";
        std::cout << "  ╔══════════════════════╗\n";
        std::cout << "  ║      T E T R I S     ║\n";
        std::cout << "  ║                      ║\n";
        std::cout << "  ║  Enter / Space: Play ║\n";
        std::cout << "  ║  Escape:        Quit ║\n";
        std::cout << "  ╚══════════════════════╝\n";
    }
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
    renderer.renderGameOverOverlay(scoreSystem.getScore());
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
    // ESC -> Pause
    if (wasKeyJustPressed(VK_ESCAPE)) {
        transitionTo(GameState::Paused);
        return;
    }

    // P -> Pause
    if (wasKeyJustPressed('P')) {
        transitionTo(GameState::Paused);
        return;
    }

    // Left
    if (wasKeyJustPressed(VK_LEFT)) {
        pieceManager.moveLeft(board);
    }

    // Right
    if (wasKeyJustPressed(VK_RIGHT)) {
        pieceManager.moveRight(board);
    }

    // Down (soft drop – held key is fine)
    if (isKeyPressed(VK_DOWN)) {
        if (pieceManager.moveDown(board)) {
            scoreSystem.addBonus(1); // Soft-drop bonus
        }
    }

    // Up -> rotate
    if (wasKeyJustPressed(VK_UP)) {
        pieceManager.rotate(board);
    }

    // Space -> hard drop
    if (wasKeyJustPressed(VK_SPACE)) {
        int rows = pieceManager.hardDrop(board);
        scoreSystem.addBonus(rows * 2); // Hard-drop bonus
        lockPiece();
        // Reset the drop timer so the newly spawned piece gets a full interval
        lastDropTime = std::chrono::steady_clock::now();
    }
}

bool TetrisGame::isKeyPressed(int vkCode) {
    return (GetAsyncKeyState(vkCode) & 0x8000) != 0;
}

bool TetrisGame::wasKeyJustPressed(int vkCode) {
    bool current   = isKeyPressed(vkCode);
    bool justPressed = current && !prevKeyStates[vkCode];
    prevKeyStates[vkCode] = current;
    return justPressed;
}
