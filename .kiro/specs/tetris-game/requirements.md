# Requirements Document

## Introduction

A classic Tetris game implementation for Windows using C++ with an incremental development approach. The system shall provide the traditional Tetris gameplay experience with falling tetrominoes, line clearing mechanics, and progressive difficulty scaling. The architecture shall support iterative development, starting with a minimal foundation and allowing incremental addition of UI components and game logic.

## Glossary

- **Tetris_Game**: The main game system managing all game states and components
- **Tetromino**: A geometric shape composed of four connected squares (the falling pieces)
- **Game_Board**: The rectangular playing field where tetrominoes fall and accumulate
- **Line_Clear**: The removal of completely filled horizontal rows from the game board
- **Drop_Speed**: The rate at which tetrominoes fall, measured in cells per time unit
- **Game_State**: The current operational mode (Menu, Playing, Paused, Game_Over)
- **Input_Handler**: The component responsible for processing keyboard and USB controller input
- **Renderer**: The component responsible for drawing the game to the screen
- **Score_System**: The component that calculates and tracks player score

## Requirements

### Requirement 1: Core Game Board Management

**User Story:** As a player, I want a game board where tetrominoes can fall and accumulate, so that I can play the classic Tetris experience.

#### Acceptance Criteria

1. THE Game_Board SHALL maintain a 10-column by 20-row grid of cells
2. WHEN a tetromino lands, THE Game_Board SHALL fix the tetromino's blocks in their final positions
3. WHEN a row is completely filled, THE Game_Board SHALL remove that row and shift all rows above downward
4. WHEN multiple rows are filled simultaneously, THE Game_Board SHALL clear all filled rows in a single operation
5. WHEN the top row contains fixed blocks, THE Game_Board SHALL trigger a game over condition

### Requirement 2: Tetromino Generation and Movement

**User Story:** As a player, I want tetrominoes to spawn and fall automatically, so that the game progresses continuously.

#### Acceptance Criteria

1. THE Tetris_Game SHALL spawn a new tetromino at the top center of the game board
2. WHEN the drop timer expires, THE Tetris_Game SHALL move the active tetromino down one row
3. WHEN a tetromino cannot move down further, THE Tetris_Game SHALL lock it in place and spawn a new tetromino
4. THE Tetris_Game SHALL generate tetrominoes using the standard seven piece types (I, O, T, S, Z, J, L)
5. WHEN a new tetromino spawns in an occupied space, THE Tetris_Game SHALL trigger game over

### Requirement 3: Player Input Controls

**User Story:** As a player, I want to control falling tetrominoes with keyboard input and USB controller, so that I can position them strategically using my preferred input method.

#### Acceptance Criteria

1. WHEN the left arrow key or controller left is pressed, THE Input_Handler SHALL move the active tetromino one column left
2. WHEN the right arrow key or controller right is pressed, THE Input_Handler SHALL move the active tetromino one column right
3. WHEN the down arrow key or controller down is pressed, THE Input_Handler SHALL accelerate the tetromino's downward movement
4. WHEN the up arrow key or controller A button is pressed, THE Input_Handler SHALL rotate the active tetromino clockwise
5. WHEN a movement would cause collision, THE Input_Handler SHALL prevent the movement and maintain current position
6. THE Input_Handler SHALL detect and support 2A8K USB controllers when connected
7. WHEN multiple input devices are connected, THE Input_Handler SHALL accept input from any connected device simultaneously

### Requirement 4: Collision Detection System

**User Story:** As a player, I want tetrominoes to interact realistically with the game board and other pieces, so that the game follows proper physics.

#### Acceptance Criteria

1. WHEN a tetromino moves, THE Tetris_Game SHALL validate the new position against board boundaries
2. WHEN a tetromino moves, THE Tetris_Game SHALL validate the new position against existing fixed blocks
3. WHEN a rotation would cause collision, THE Tetris_Game SHALL attempt wall kicks before rejecting the rotation
4. WHEN no valid wall kick exists, THE Tetris_Game SHALL maintain the tetromino's current orientation
5. THE Tetris_Game SHALL prevent any movement that would place tetromino blocks outside the game board

### Requirement 5: Scoring and Level Progression

**User Story:** As a player, I want to earn points and advance through levels, so that I have goals and increasing challenge.

#### Acceptance Criteria

1. WHEN lines are cleared, THE Score_System SHALL award points based on the number of lines cleared simultaneously
2. WHEN the player clears 10 lines, THE Score_System SHALL increase the level by one
3. WHEN the level increases, THE Tetris_Game SHALL increase the drop speed proportionally
4. THE Score_System SHALL track total lines cleared across all levels
5. THE Score_System SHALL maintain the current high score between game sessions

### Requirement 6: Game State Management

**User Story:** As a player, I want clear game states and transitions, so that I can navigate the game interface intuitively.

#### Acceptance Criteria

1. WHEN the game starts, THE Tetris_Game SHALL display a main menu with play and quit options
2. WHEN play is selected, THE Tetris_Game SHALL transition to the playing state and begin gameplay
3. WHEN the escape key is pressed during gameplay, THE Tetris_Game SHALL pause the game and display pause menu
4. WHEN game over conditions are met, THE Tetris_Game SHALL transition to game over state and display final score
5. WHEN returning to menu from any state, THE Tetris_Game SHALL reset all game variables to initial values

### Requirement 7: Visual Rendering System

**User Story:** As a player, I want clear visual representation of the game state, so that I can make informed decisions.

#### Acceptance Criteria

1. THE Renderer SHALL display the game board with distinct colors for different tetromino types
2. THE Renderer SHALL show the currently falling tetromino with real-time position updates
3. THE Renderer SHALL display the current score, level, and lines cleared
4. THE Renderer SHALL show the next tetromino that will spawn
5. WHEN the game is paused, THE Renderer SHALL display a pause indicator overlay

### Requirement 8: Incremental Development Architecture

**User Story:** As a developer, I want a modular architecture that supports incremental development, so that I can add features iteratively without major refactoring.

#### Acceptance Criteria

1. THE Tetris_Game SHALL separate core game logic from rendering concerns
2. THE Tetris_Game SHALL use interfaces to decouple major components (input, rendering, audio)
3. WHEN new UI components are added, THE existing game logic SHALL remain unchanged
4. WHEN rendering systems are modified, THE core game mechanics SHALL continue functioning
5. THE Tetris_Game SHALL support plugin-style addition of new features without modifying existing code

### Requirement 9: Windows Platform Integration

**User Story:** As a Windows user, I want the game to integrate properly with the Windows environment, so that it behaves like a native application.

#### Acceptance Criteria

1. THE Tetris_Game SHALL create a standard Windows application window
2. THE Tetris_Game SHALL handle Windows message processing for input and window events
3. WHEN the window is closed, THE Tetris_Game SHALL save game state and exit cleanly
4. WHEN the window loses focus, THE Tetris_Game SHALL automatically pause gameplay
5. THE Tetris_Game SHALL support standard Windows keyboard shortcuts where applicable

### Requirement 10: Configuration and Persistence

**User Story:** As a player, I want my settings and high scores to be remembered, so that I can customize my experience and track progress.

#### Acceptance Criteria

1. THE Tetris_Game SHALL save high scores to a local configuration file
2. THE Tetris_Game SHALL load saved settings on startup
3. WHEN key bindings are modified, THE Tetris_Game SHALL persist the changes
4. WHEN the application exits unexpectedly, THE Tetris_Game SHALL preserve high score data
5. THE Tetris_Game SHALL provide default settings when no configuration file exists