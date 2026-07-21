#include "GameBoard.h"

GameBoard::GameBoard() {
    clear();
}

bool GameBoard::isOccupied(int x, int y) const {
    if (x < 0 || x >= Constants::BOARD_WIDTH || y < 0 || y >= Constants::BOARD_HEIGHT) {
        return true; // Out of bounds counts as occupied
    }
    return cells[y][x].has_value();
}

std::optional<TetrominoType> GameBoard::getCell(int x, int y) const {
    if (x < 0 || x >= Constants::BOARD_WIDTH || y < 0 || y >= Constants::BOARD_HEIGHT) {
        return std::nullopt;
    }
    return cells[y][x];
}

void GameBoard::setCell(int x, int y, std::optional<TetrominoType> type) {
    if (x >= 0 && x < Constants::BOARD_WIDTH && y >= 0 && y < Constants::BOARD_HEIGHT) {
        cells[y][x] = type;
    }
}

bool GameBoard::isValidPosition(const Tetromino& tetromino) const {
    auto blocks = tetromino.getBlockPositions();
    
    for (const auto& block : blocks) {
        // Check boundaries
        if (block.x < 0 || block.x >= Constants::BOARD_WIDTH || 
            block.y < 0 || block.y >= Constants::BOARD_HEIGHT) {
            return false;
        }
        
        // Check collision with existing blocks
        if (isOccupied(block.x, block.y)) {
            return false;
        }
    }
    
    return true;
}

void GameBoard::fixTetromino(const Tetromino& tetromino) {
    auto blocks = tetromino.getBlockPositions();
    
    for (const auto& block : blocks) {
        setCell(block.x, block.y, tetromino.type);
    }
}

std::vector<int> GameBoard::getFilledRows() const {
    std::vector<int> filledRows;
    
    for (int y = 0; y < Constants::BOARD_HEIGHT; ++y) {
        bool isFilled = true;
        
        for (int x = 0; x < Constants::BOARD_WIDTH; ++x) {
            if (!isOccupied(x, y)) {
                isFilled = false;
                break;
            }
        }
        
        if (isFilled) {
            filledRows.push_back(y);
        }
    }
    
    return filledRows;
}

int GameBoard::clearRows(const std::vector<int>& rows) {
    if (rows.empty()) {
        return 0;
    }
    
    // Build a set of rows to remove for O(1) lookup
    // rows is expected to be sorted ascending (as returned by getFilledRows)
    std::array<bool, Constants::BOARD_HEIGHT> isCleared = {};
    for (int row : rows) {
        if (row >= 0 && row < Constants::BOARD_HEIGHT) {
            isCleared[row] = true;
        }
    }
    
    // Compact: collect surviving rows from bottom to top, then rebuild
    // We write from the bottom of the board upward, skipping cleared rows
    int writeRow = Constants::BOARD_HEIGHT - 1;
    for (int readRow = Constants::BOARD_HEIGHT - 1; readRow >= 0; --readRow) {
        if (!isCleared[readRow]) {
            if (writeRow != readRow) {
                for (int x = 0; x < Constants::BOARD_WIDTH; ++x) {
                    cells[writeRow][x] = cells[readRow][x];
                }
            }
            --writeRow;
        }
    }
    
    // Fill remaining top rows with empty cells
    for (int y = writeRow; y >= 0; --y) {
        for (int x = 0; x < Constants::BOARD_WIDTH; ++x) {
            cells[y][x] = std::nullopt;
        }
    }
    
    return static_cast<int>(rows.size());
}

bool GameBoard::isGameOver() const {
    // Game over if any cell in the top row is occupied
    for (int x = 0; x < Constants::BOARD_WIDTH; ++x) {
        if (isOccupied(x, 0)) {
            return true;
        }
    }
    return false;
}

void GameBoard::clear() {
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell = std::nullopt;
        }
    }
}
