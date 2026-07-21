#pragma once

#include "IInputHandler.h"
#include <windows.h>

// Windows keyboard implementation of IInputHandler.
// Uses GetAsyncKeyState() to poll key states each frame.
//
// Call update() once per frame before querying any is*/wasJust* methods.
// update() snapshots the current hardware state and computes edge transitions
// so that wasJust*Pressed() correctly fires only on the first frame of a press.
//
// Key mappings:
//   Move Left    — VK_LEFT
//   Move Right   — VK_RIGHT
//   Soft Drop    — VK_DOWN
//   Rotate       — VK_UP
//   Hard Drop    — VK_SPACE
//   Pause        — VK_ESCAPE or 'P'
//   Quit         — VK_ESCAPE (menu context; game logic decides)
class WindowsInputHandler : public IInputHandler {
public:
    WindowsInputHandler();
    ~WindowsInputHandler() override = default;

    // ------------------------------------------------------------------
    // IInputHandler — held-state queries
    // ------------------------------------------------------------------
    bool isLeftPressed()     const override;
    bool isRightPressed()    const override;
    bool isDownPressed()     const override;
    bool isRotatePressed()   const override;
    bool isHardDropPressed() const override;
    bool isPausePressed()    const override;
    bool isQuitPressed()     const override;

    // ------------------------------------------------------------------
    // IInputHandler — just-pressed (edge-triggered) queries
    // ------------------------------------------------------------------
    bool wasJustLeftPressed()     const override;
    bool wasJustRightPressed()    const override;
    bool wasJustDownPressed()     const override;
    bool wasJustRotatePressed()   const override;
    bool wasJustHardDropPressed() const override;
    bool wasJustPausePressed()    const override;
    bool wasJustQuitPressed()     const override;

    // ------------------------------------------------------------------
    // IInputHandler — frame update
    // ------------------------------------------------------------------
    // Must be called once per frame before any queries.
    void update() override;

private:
    // Helper: returns true if the given virtual-key is currently held.
    static bool isVKDown(int vkCode);

    // Per-action current and previous frame state.
    // "current" reflects this frame; "prev" reflects last frame (set by update()).
    struct KeyState {
        bool current = false;
        bool prev    = false;
    };

    KeyState left_;
    KeyState right_;
    KeyState down_;
    KeyState rotate_;
    KeyState hardDrop_;
    KeyState pause_;   // ESC or P
    KeyState quit_;    // ESC
};
