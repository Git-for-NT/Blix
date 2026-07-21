#pragma once
#include "Tetromino.h"
#include "GameBoard.h"
#include <random>
#include <vector>

class TetrominoManager {
public:
    TetrominoManager();
    
    // Piece generation
    void spawnNewPiece();
    TetrominoType getNextPieceType() const;
    
    // Current piece access
    Tetromino& getCurrentPiece();
    const Tetromino& getCurrentPiece() const;
    bool hasCurrentPiece() const;
    
    // Movement operations
    bool moveLeft(const GameBoard& board);
    bool moveRight(const GameBoard& board);
    bool moveDown(const GameBoard& board);
    bool rotate(const GameBoard& board);
    
    // Drop the piece all the way down
    int hardDrop(const GameBoard& board);
    
private:
    Tetromino currentPiece;
    TetrominoType nextPieceType;
    bool hasPiece;
    
    std::mt19937 rng;
    std::vector<TetrominoType> pieceBag;
    
    void refillBag();
    TetrominoType drawFromBag();
    
    // Wall kick system for rotation
    bool tryRotateWithKicks(const GameBoard& board);
    std::vector<Position> getWallKickOffsets(TetrominoType type, int fromRotation, int toRotation) const;
};
