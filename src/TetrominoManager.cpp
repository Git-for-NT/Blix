#include "TetrominoManager.h"
#include <algorithm>
#include <chrono>

TetrominoManager::TetrominoManager() 
    : currentPiece(TetrominoType::I, {0, 0}, 0)
    , nextPieceType(TetrominoType::I)
    , hasPiece(false) {
    
    // Seed random number generator
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed));
    
    refillBag();
    nextPieceType = drawFromBag();
}

void TetrominoManager::spawnNewPiece() {
    // Spawn at top center of board
    Position spawnPos = {Constants::BOARD_WIDTH / 2 - 1, 0};
    currentPiece = Tetromino(nextPieceType, spawnPos, 0);
    hasPiece = true;
    
    // Draw next piece
    nextPieceType = drawFromBag();
}

TetrominoType TetrominoManager::getNextPieceType() const {
    return nextPieceType;
}

Tetromino& TetrominoManager::getCurrentPiece() {
    return currentPiece;
}

const Tetromino& TetrominoManager::getCurrentPiece() const {
    return currentPiece;
}

bool TetrominoManager::hasCurrentPiece() const {
    return hasPiece;
}

bool TetrominoManager::moveLeft(const GameBoard& board) {
    if (!hasPiece) return false;
    
    Tetromino testPiece = currentPiece;
    testPiece.position.x--;
    
    if (board.isValidPosition(testPiece)) {
        currentPiece = testPiece;
        return true;
    }
    return false;
}

bool TetrominoManager::moveRight(const GameBoard& board) {
    if (!hasPiece) return false;
    
    Tetromino testPiece = currentPiece;
    testPiece.position.x++;
    
    if (board.isValidPosition(testPiece)) {
        currentPiece = testPiece;
        return true;
    }
    return false;
}

bool TetrominoManager::moveDown(const GameBoard& board) {
    if (!hasPiece) return false;
    
    Tetromino testPiece = currentPiece;
    testPiece.position.y++;
    
    if (board.isValidPosition(testPiece)) {
        currentPiece = testPiece;
        return true;
    }
    return false;
}

bool TetrominoManager::rotate(const GameBoard& board) {
    if (!hasPiece) return false;
    return tryRotateWithKicks(board);
}

int TetrominoManager::hardDrop(const GameBoard& board) {
    if (!hasPiece) return 0;
    
    int rowsDropped = 0;
    while (moveDown(board)) {
        rowsDropped++;
    }
    return rowsDropped;
}

void TetrominoManager::refillBag() {
    pieceBag.clear();
    pieceBag = {
        TetrominoType::I,
        TetrominoType::O,
        TetrominoType::T,
        TetrominoType::S,
        TetrominoType::Z,
        TetrominoType::J,
        TetrominoType::L
    };
    
    std::shuffle(pieceBag.begin(), pieceBag.end(), rng);
}

TetrominoType TetrominoManager::drawFromBag() {
    if (pieceBag.empty()) {
        refillBag();
    }
    
    TetrominoType piece = pieceBag.back();
    pieceBag.pop_back();
    return piece;
}

bool TetrominoManager::tryRotateWithKicks(const GameBoard& board) {
    int oldRotation = currentPiece.rotation;
    int newRotation = (oldRotation + 1) % 4;
    
    // Try rotation without kick first
    Tetromino testPiece = currentPiece;
    testPiece.rotation = newRotation;
    
    if (board.isValidPosition(testPiece)) {
        currentPiece = testPiece;
        return true;
    }
    
    // Try wall kicks
    auto kicks = getWallKickOffsets(currentPiece.type, oldRotation, newRotation);
    for (const auto& kick : kicks) {
        testPiece = currentPiece;
        testPiece.rotation = newRotation;
        testPiece.position.x += kick.x;
        testPiece.position.y += kick.y;
        
        if (board.isValidPosition(testPiece)) {
            currentPiece = testPiece;
            return true;
        }
    }
    
    return false;
}

std::vector<Position> TetrominoManager::getWallKickOffsets(TetrominoType type, int fromRotation, int toRotation) const {
    // Simplified SRS wall kick system
    // I-piece has special kicks, others use standard kicks
    
    if (type == TetrominoType::I) {
        // I-piece wall kicks
        return {
            {-1, 0}, {1, 0}, {0, -1}, {-1, -1}, {1, -1}
        };
    } else if (type == TetrominoType::O) {
        // O-piece doesn't need wall kicks
        return {};
    } else {
        // Standard wall kicks for other pieces
        return {
            {-1, 0}, {1, 0}, {0, -1}, {-1, -1}, {1, -1}
        };
    }
}
