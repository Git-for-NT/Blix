#pragma once

// Raw key codes for keyboard input mapping.
// Values correspond to Windows Virtual-Key codes where applicable.
enum class KeyCode {
    LeftArrow,
    RightArrow,
    DownArrow,
    UpArrow,
    Escape,
    Enter,
    Space,
    W, A, S, D,  // WASD alternatives
    P,           // Pause
    Q,           // Quit
    Unknown
};

// USB controller button identifiers.
// Intended to cover common gamepad layouts (e.g., 2A8K controllers).
enum class ControllerButton {
    DPadLeft,
    DPadRight,
    DPadDown,
    DPadUp,
    A,        // Typically confirm / rotate
    B,        // Typically back / hard drop
    Start,    // Pause / resume
    Select,   // Quit / menu
    Unknown
};

// Logical game actions that input sources can trigger.
// Raw device input is mapped to these actions by IInputHandler implementations.
enum class GameAction {
    MoveLeft,
    MoveRight,
    SoftDrop,
    Rotate,
    HardDrop,
    Pause,
    Quit
};

// Abstract interface for input handling.
// Implement this to support different input sources (keyboard, controller, test stubs).
//
// Two query modes are provided for every action:
//   is*Pressed()     — true as long as the input is held (continuous / repeating)
//   wasJust*Pressed() — true only on the first frame the input became active
//
// Use wasJust*Pressed() for one-shot actions (rotate, hard drop, pause, quit) and
// is*Pressed() for actions that benefit from auto-repeat (move, soft drop).
class IInputHandler {
public:
    virtual ~IInputHandler() = default;

    // ------------------------------------------------------------------
    // Held-state queries (true while the input is continuously pressed)
    // ------------------------------------------------------------------

    // Movement
    virtual bool isLeftPressed()     const = 0;
    virtual bool isRightPressed()    const = 0;
    virtual bool isDownPressed()     const = 0;   // Soft drop

    // Actions
    virtual bool isRotatePressed()   const = 0;
    virtual bool isHardDropPressed() const = 0;

    // Game control
    virtual bool isPausePressed()    const = 0;
    virtual bool isQuitPressed()     const = 0;

    // ------------------------------------------------------------------
    // Just-pressed queries (true only on the first frame of activation)
    // Use these for one-shot, non-repeating actions.
    // ------------------------------------------------------------------

    // Movement (useful for single-step moves without auto-repeat)
    virtual bool wasJustLeftPressed()     const = 0;
    virtual bool wasJustRightPressed()    const = 0;
    virtual bool wasJustDownPressed()     const = 0;

    // Actions
    virtual bool wasJustRotatePressed()   const = 0;
    virtual bool wasJustHardDropPressed() const = 0;

    // Game control
    virtual bool wasJustPausePressed()    const = 0;
    virtual bool wasJustQuitPressed()     const = 0;

    // ------------------------------------------------------------------
    // Frame update
    // ------------------------------------------------------------------

    // Called once per frame to capture a fresh snapshot of device state.
    // Implementations should update internal "previous state" tracking here
    // so that wasJust*Pressed() can compute edge-transitions correctly.
    virtual void update() = 0;
};
