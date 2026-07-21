#pragma once

namespace Constants {
    // Game board dimensions
    constexpr int BOARD_WIDTH = 10;
    constexpr int BOARD_HEIGHT = 20;
    
    // Timing
    constexpr int TARGET_FPS = 60;
    constexpr int FRAME_TIME_MS = 1000 / TARGET_FPS;
    
    // Initial game settings
    constexpr int INITIAL_DROP_SPEED_MS = 1000;
    constexpr int MIN_DROP_SPEED_MS = 50;
    constexpr int LINES_PER_LEVEL = 10;
}
