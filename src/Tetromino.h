#pragma once
#include <array>

enum class TetrominoType {
    I, O, T, S, Z, J, L
};

struct Position {
    int x;
    int y;
};

class Tetromino {
public:
    TetrominoType type;
    Position position;
    int rotation;  // 0-3 for four orientations

    Tetromino(TetrominoType t, Position pos = {0, 0}, int rot = 0);
    
    // Returns the 4 block positions for current rotation
    std::array<Position, 4> getBlockPositions() const;
    
    // Rotate clockwise
    void rotate();
};
