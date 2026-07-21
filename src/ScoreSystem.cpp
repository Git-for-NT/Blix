#include "ScoreSystem.h"
#include <algorithm>

// ---------------------------------------------------------------------------
// Construction / reset
// ---------------------------------------------------------------------------

ScoreSystem::ScoreSystem()
    : score(0)
    , level(1)
    , linesCleared(0)
{
}

void ScoreSystem::reset() {
    score        = 0;
    level        = 1;
    linesCleared = 0;
}

// ---------------------------------------------------------------------------
// Scoring
// ---------------------------------------------------------------------------

// Returns the base point value for clearing `lines` lines simultaneously
// (before the level multiplier is applied).
// Requirement 5.1: 1=100, 2=300, 3=500, 4=800
int ScoreSystem::basePoints(int lines) {
    switch (lines) {
        case 1:  return 100;
        case 2:  return 300;
        case 3:  return 500;
        case 4:  return 800;
        default: return 0;      // 0 or >4 lines → no award
    }
}

void ScoreSystem::addLines(int lines) {
    if (lines <= 0) return;

    // Award points multiplied by the current level (Req 5.1)
    score += basePoints(lines) * level;

    // Track cumulative lines cleared (Req 5.4)
    linesCleared += lines;

    // Level progression: one level per LINES_PER_LEVEL lines (Req 5.2)
    level = 1 + (linesCleared / Constants::LINES_PER_LEVEL);
}

void ScoreSystem::addBonus(int points) {
    if (points > 0) {
        score += points;
    }
}

// ---------------------------------------------------------------------------
// Drop speed
// ---------------------------------------------------------------------------

// Returns the drop interval in milliseconds for the current level (Req 5.3).
// Formula: max(MIN_DROP_SPEED_MS, INITIAL_DROP_SPEED_MS - (level - 1) * 50)
//   Level 1  → 1000 ms
//   Level 10 →  550 ms
//   Level 19 →   50 ms (minimum, clamped)
int ScoreSystem::getDropSpeed() const {
    const int speed = Constants::INITIAL_DROP_SPEED_MS - (level - 1) * 50;
    return std::max(Constants::MIN_DROP_SPEED_MS, speed);
}
