#pragma once
#include "Tetromino.h"
#include "Constants.h"
#include <array>
#include <optional>
#include <vector>

class GameBoard {
public:
    GameBoard();
    
    // Cell access and modification
    bool isOccupied(int x, int y) const;
    std::optional<TetrominoType> getCell(int x, int y) const;
    void setCell(int x, int y, std::optional<TetrominoType> type);
    
    // Tetromino operations
    bool isValidPosition(const Tetromino& tetromino) const;
    void fixTetromino(const Tetromino& tetromino);
    
    // Line clearing
    std::vector<int> getFilledRows() const;
    int clearRows(const std::vector<int>& rows);
    
    // Game over detection
    bool isGameOver() const;
    
    // Clear the entire board
    void clear();
    
private:
    std::array<std::array<std::optional<TetrominoType>, Constants::BOARD_WIDTH>, Constants::BOARD_HEIGHT> cells;
};
