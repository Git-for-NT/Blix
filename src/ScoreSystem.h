#pragma once
#include "Constants.h"

// ScoreSystem encapsulates all scoring, level progression, and drop-speed
// logic as required by Requirements 5.1–5.4.
//
// Score table (× level):
//   1 line  → 100
//   2 lines → 300
//   3 lines → 500
//   4 lines → 800
//
// Level: starts at 1, increments every LINES_PER_LEVEL (10) lines cleared.
//
// Drop speed: max(MIN_DROP_SPEED_MS, INITIAL_DROP_SPEED_MS - (level-1) * 50)
//   Level 1  → 1000 ms
//   Level 10 → 550 ms
//   Level 19 →  50 ms  (minimum)

class ScoreSystem {
public:
    ScoreSystem();

    // --------------------------------------------------------------------- //
    //  Mutators
    // --------------------------------------------------------------------- //

    /// Award points for clearing `lines` rows at once.
    /// Also advances the level counter when the cumulative line threshold is hit.
    void addLines(int lines);

    /// Add a flat bonus (e.g. soft-drop or hard-drop bonus points).
    void addBonus(int points);

    /// Reset all state to the initial values (call on new game / return to menu).
    void reset();

    // --------------------------------------------------------------------- //
    //  Accessors
    // --------------------------------------------------------------------- //

    int getScore()        const { return score; }
    int getLevel()        const { return level; }
    int getLinesCleared() const { return linesCleared; }

    /// Returns the current drop interval in milliseconds.
    int getDropSpeed() const;

private:
    int score;
    int level;
    int linesCleared;

    /// Points awarded for clearing n lines simultaneously (before level mult.).
    static int basePoints(int lines);
};
