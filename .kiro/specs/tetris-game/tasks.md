# Implementation Plan: Tetris Game

## Overview

This implementation plan breaks down the Tetris game development into incremental, testable steps following the modular C++ architecture. Each task builds upon previous work, starting with core data structures and progressing through game logic, input handling, and rendering. The approach prioritizes getting a minimal working game early, then adding features incrementally.

## Tasks

- [x] 1. Set up project structure and core data types
  - Create Visual Studio project with appropriate C++ settings
  - Define TetrominoType enum and basic constants
  - Implement Tetromino class with shape data and rotation logic
  - Set up basic project structure with header/source separation
  - _Requirements: 2.4, 3.4_

- [x] 2. Implement core game board functionality
  - [x] 2.1 Create GameBoard class with grid management
    - Implement 10x20 grid with cell state tracking
    - Add methods for cell access and modification
    - _Requirements: 1.1, 1.2_
  
  - [ ]* 2.2 Write property test for game board dimensions
    - **Property 1: Game board maintains correct dimensions**
    - **Validates: Requirements 1.1**
  
  - [x] 2.3 Implement line clearing logic
    - Add methods to detect full lines and clear them
    - Implement row shifting after line clears
    - _Requirements: 1.3, 1.4_
  
  - [ ]* 2.4 Write property test for line clearing
    - **Property 3: Line clearing and shifting**
    - **Validates: Requirements 1.3, 1.4**
  
  - [x] 2.5 Add game over detection
    - Implement logic to detect when top row has blocks
    - _Requirements: 1.5_

- [x] 3. Create tetromino management system
  - [x] 3.1 Implement TetrominoManager class
    - Add piece generation with random selection
    - Implement current and next piece tracking
    - Add basic movement and rotation methods
    - _Requirements: 2.1, 2.4, 3.4_
  
  - [ ]* 3.2 Write property test for piece generation
    - **Property 8: Standard piece generation**
    - **Validates: Requirements 2.4**
  
  - [x] 3.3 Add collision detection system
    - Implement boundary checking for movements
    - Add collision detection with existing blocks
    - Implement wall kick logic for rotations
    - _Requirements: 4.1, 4.2, 4.3, 4.4, 4.5_
  
  - [ ]* 3.4 Write property test for movement validation
    - **Property 9: Movement validation**
    - **Validates: Requirements 3.1, 3.2, 3.5, 4.1, 4.2, 4.5**
  
  - [ ]* 3.5 Write property test for rotation behavior
    - **Property 11: Piece rotation behavior**
    - **Validates: Requirements 3.4, 4.3, 4.4**

- [x] 4. Implement core game logic and state management
  - [x] 4.1 Create TetrisGame class with state machine
    - Implement GameState enum and state transitions
    - Add game loop structure with update/render separation
    - Implement piece spawning and automatic dropping
    - _Requirements: 2.1, 2.2, 2.3, 6.1, 6.2, 6.3, 6.4_
  
  - [ ]* 4.2 Write property test for piece spawning
    - **Property 5: Piece spawning location**
    - **Validates: Requirements 2.1**
  
  - [ ]* 4.3 Write property test for automatic dropping
    - **Property 6: Automatic piece dropping**
    - **Validates: Requirements 2.2**
  
  - [x] 4.4 Add piece locking and game over logic
    - Implement logic for when pieces can't move down
    - Add game over detection for spawn collisions
    - _Requirements: 2.3, 2.5_
  
  - [ ]* 4.5 Write property test for game over detection
    - **Property 4: Game over detection**
    - **Validates: Requirements 1.5, 2.5**

- [x] 5. Checkpoint - Core game logic validation
  - Ensure all tests pass, ask the user if questions arise.

- [x] 6. Implement scoring and level system
  - [x] 6.1 Create ScoreSystem class
    - Implement score calculation based on lines cleared
    - Add level progression logic (10 lines per level)
    - Implement drop speed scaling with level
    - Track total lines cleared across levels
    - _Requirements: 5.1, 5.2, 5.3, 5.4_
  
  - [ ]* 6.2 Write property test for scoring calculation
    - **Property 12: Scoring calculation**
    - **Validates: Requirements 5.1**
  
  - [ ]* 6.3 Write property test for level progression
    - **Property 13: Level progression**
    - **Validates: Requirements 5.2, 5.3**
  
  - [ ]* 6.4 Write property test for line counting
    - **Property 14: Line counting persistence**
    - **Validates: Requirements 5.4**

- [x] 7. Create interface definitions and basic implementations
  - [x] 7.1 Define IInputHandler interface
    - Create abstract interface for input handling
    - Define KeyCode and ControllerButton enums
    - Added wasJustConfirmPressed() for menu navigation
    - _Requirements: 3.1, 3.2, 3.3, 3.4, 3.5_
  
  - [x] 7.2 Define IRenderer interface
    - Create abstract interface for rendering
    - Define methods for drawing game elements
    - _Requirements: 7.1, 7.2, 7.3, 7.4, 7.5_
  
  - [x] 7.3 Define IConfigManager interface
    - Create abstract interface for configuration management
    - Define methods for settings persistence
    - _Requirements: 10.1, 10.2, 10.3, 10.5_

- [x] 8. Implement Windows input handling
  - [x] 8.1 Create WindowsInputHandler class
    - Implement keyboard state tracking
    - Add support for arrow keys and basic controls
    - Handle key press detection (current and just-pressed)
    - Added Enter key (VK_RETURN) as confirm/menu-select action
    - _Requirements: 3.1, 3.2, 3.3, 3.4, 3.5_
  
  - [x] 8.2 Add USB controller support
    - Implement controller detection and input reading via XInput
    - Map controller buttons to game actions (D-Pad, A, B, Start, Select)
    - CombinedInputHandler aggregates keyboard + controller (OR logic)
    - Added to TetrisGame.vcxproj with xinput.lib linker dependency
    - _Requirements: 3.6, 3.7_
  
  - [ ]* 8.3 Write property test for soft drop acceleration
    - **Property 10: Soft drop acceleration**
    - **Validates: Requirements 3.3**

- [x] 9. Implement console rendering system
  - [x] 9.1 Create ConsoleRenderer class
    - Implement basic console output for game board
    - Add piece rendering with distinct characters/colors
    - Display game statistics (score, level, lines)
    - Implemented setCursorVisible() for clean cursor management
    - _Requirements: 7.1, 7.2, 7.3_
  
  - [x] 9.2 Add next piece preview and UI elements
    - Implement next piece display (4-row side-panel preview)
    - Add pause overlay and game over overlay with high score
    - renderGameOverOverlay shows final score, best score (* for new record)
    - _Requirements: 7.4, 7.5_
  
  - [ ]* 9.3 Write property test for rendering completeness
    - **Property 16: Rendering completeness**
    - **Validates: Requirements 7.1, 7.2, 7.3, 7.4, 7.5**

- [x] 10. Create Windows application framework
  - [x] 10.1 Implement TetrisApplication class
    - Console title, buffer size, cursor hide, Ctrl+C handler
    - Builds CombinedInputHandler (keyboard + controller) and FileConfigManager
    - Passes both into TetrisGame via constructor injection
    - _Requirements: 9.1, 9.2, 9.3, 9.4_
  
  - [ ]* 10.2 Write property test for window focus handling
    - **Property 17: Window focus handling**
    - **Validates: Requirements 9.3, 9.4**

- [x] 11. Implement configuration management
  - [x] 11.1 Create FileConfigManager class
    - Implements IConfigManager with key=value file (tetris_config.ini)
    - High score persisted immediately on game over
    - Graceful defaults when config file is absent
    - _Requirements: 10.1, 10.2, 10.5_
  
  - [ ] 11.2 Add key binding customization
    - Implement customizable key mappings
    - Add persistence for key binding changes
    - _Requirements: 10.3_

- [x] 12. Integration and game loop implementation
  - [x] 12.1 Wire all components together in main game loop
    - IInputHandler injected into TetrisGame (replaced raw GetAsyncKeyState)
    - IConfigManager injected into TetrisGame (high score load/save)
    - Timing: 60 FPS frame loop with drop-speed scaling per level
    - _Requirements: 2.2, 6.2, 6.3, 6.4, 6.5_
  
  - [ ]* 12.2 Write property test for state transitions
    - **Property 15: State transition correctness**
    - **Validates: Requirements 6.2, 6.3, 6.4, 6.5**
  
  - [x] 12.3 Add menu system and game flow
    - Main menu with B L I X title, best score display, Enter/Space to play
    - Pause overlay (ESC or P to toggle)
    - Game over screen with final score, best score, new-record marker
    - needsStaticRedraw flag prevents redundant redraws each frame
    - _Requirements: 6.1, 6.3, 6.4_

- [x] 13. Final integration and testing
  - [x] 13.1 Implement complete game flow
    - Full cycle: Menu → Playing → Paused → GameOver → Menu verified by build
    - All state transitions call renderer.clear() and set needsStaticRedraw
    - High score saved to tetris_config.ini on game over
    - Clean exit via ESC (ExitProcess) or Ctrl+C handler
    - _Requirements: 6.1, 6.2, 6.3, 6.4, 6.5_
  
  - [ ]* 13.2 Write property test for tetromino placement
    - **Property 2: Tetromino placement persistence**
    - **Validates: Requirements 1.2**
  
  - [ ]* 13.3 Write property test for piece locking cycle
    - **Property 7: Piece locking and spawning cycle**
    - **Validates: Requirements 2.3**

- [x] 14. Final checkpoint - Complete system validation
  - All non-optional tasks complete. Build verified clean (exit code 0).
  - Executable: bin\Release\TetrisGame.exe

## Notes

- Tasks marked with `*` are optional and can be skipped for faster MVP
- Each task references specific requirements for traceability
- The implementation follows incremental development - each phase produces working code
- Property tests validate universal correctness properties across all inputs
- Unit tests can be added for specific examples and edge cases as needed
- Focus on getting a minimal playable game first, then enhance with additional features
