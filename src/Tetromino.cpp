#include "Tetromino.h"

// Shape definitions for each tetromino type and rotation
// Each shape is defined as 4 relative positions from the tetromino's position
namespace {
    std::array<Position, 4> getShapeData(TetrominoType type, int rotation) {
        // I piece
        if (type == TetrominoType::I) {
            if (rotation == 0 || rotation == 2) return {Position{0, 0}, Position{1, 0}, Position{2, 0}, Position{3, 0}};
            else return {Position{0, 0}, Position{0, 1}, Position{0, 2}, Position{0, 3}};
        }
        // O piece
        else if (type == TetrominoType::O) {
            return {Position{0, 0}, Position{1, 0}, Position{0, 1}, Position{1, 1}};
        }
        // T piece
        else if (type == TetrominoType::T) {
            if (rotation == 0) return {Position{1, 0}, Position{0, 1}, Position{1, 1}, Position{2, 1}};
            else if (rotation == 1) return {Position{1, 0}, Position{1, 1}, Position{2, 1}, Position{1, 2}};
            else if (rotation == 2) return {Position{0, 1}, Position{1, 1}, Position{2, 1}, Position{1, 2}};
            else return {Position{1, 0}, Position{0, 1}, Position{1, 1}, Position{1, 2}};
        }
        // S piece
        else if (type == TetrominoType::S) {
            if (rotation == 0 || rotation == 2) return {Position{1, 0}, Position{2, 0}, Position{0, 1}, Position{1, 1}};
            else return {Position{1, 0}, Position{1, 1}, Position{2, 1}, Position{2, 2}};
        }
        // Z piece
        else if (type == TetrominoType::Z) {
            if (rotation == 0 || rotation == 2) return {Position{0, 0}, Position{1, 0}, Position{1, 1}, Position{2, 1}};
            else return {Position{2, 0}, Position{1, 1}, Position{2, 1}, Position{1, 2}};
        }
        // J piece
        else if (type == TetrominoType::J) {
            if (rotation == 0) return {Position{0, 0}, Position{0, 1}, Position{1, 1}, Position{2, 1}};
            else if (rotation == 1) return {Position{1, 0}, Position{2, 0}, Position{1, 1}, Position{1, 2}};
            else if (rotation == 2) return {Position{0, 1}, Position{1, 1}, Position{2, 1}, Position{2, 2}};
            else return {Position{1, 0}, Position{1, 1}, Position{0, 2}, Position{1, 2}};
        }
        // L piece
        else {
            if (rotation == 0) return {Position{2, 0}, Position{0, 1}, Position{1, 1}, Position{2, 1}};
            else if (rotation == 1) return {Position{1, 0}, Position{1, 1}, Position{1, 2}, Position{2, 2}};
            else if (rotation == 2) return {Position{0, 1}, Position{1, 1}, Position{2, 1}, Position{0, 2}};
            else return {Position{0, 0}, Position{1, 0}, Position{1, 1}, Position{1, 2}};
        }
    }
}

Tetromino::Tetromino(TetrominoType t, Position pos, int rot)
    : type(t), position(pos), rotation(rot % 4) {
}

std::array<Position, 4> Tetromino::getBlockPositions() const {
    const auto shape = getShapeData(type, rotation);
    std::array<Position, 4> positions;
    
    for (int i = 0; i < 4; ++i) {
        positions[i] = {
            position.x + shape[i].x,
            position.y + shape[i].y
        };
    }
    
    return positions;
}

void Tetromino::rotate() {
    rotation = (rotation + 1) % 4;
}
