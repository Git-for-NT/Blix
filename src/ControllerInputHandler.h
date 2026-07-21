#pragma once
#include "IInputHandler.h"
#include <windows.h>
#include <xinput.h>

// ---------------------------------------------------------------------------
// ControllerInputHandler
//
// Implements IInputHandler using the Windows XInput API.
// Supports standard XInput-compatible USB controllers including the 2A8K.
//
// Button mapping (Req 3.6, 3.7):
//   D-Pad Left           -> MoveLeft
//   D-Pad Right          -> MoveRight
//   D-Pad Down           -> SoftDrop
//   D-Pad Up  OR  A btn  -> Rotate
//   B button             -> HardDrop
//   Start button         -> Pause
//
// The handler polls the first connected controller (user index 0–3).
// Call update() once per frame to refresh the internal state snapshot.
// ---------------------------------------------------------------------------
class ControllerInputHandler : public IInputHandler {
public:
    ControllerInputHandler();

    // IInputHandler — held-state queries
    bool isLeftPressed()     const override;
    bool isRightPressed()    const override;
    bool isDownPressed()     const override;
    bool isRotatePressed()   const override;
    bool isHardDropPressed() const override;
    bool isPausePressed()    const override;
    bool isQuitPressed()     const override;

    // IInputHandler — just-pressed queries
    bool wasJustLeftPressed()     const override;
    bool wasJustRightPressed()    const override;
    bool wasJustDownPressed()     const override;
    bool wasJustRotatePressed()   const override;
    bool wasJustHardDropPressed() const override;
    bool wasJustPausePressed()    const override;
    bool wasJustQuitPressed()     const override;

    // Frame update — captures a fresh XInput state snapshot
    void update() override;

    // Returns true if at least one XInput controller is connected
    bool isConnected() const;

private:
    // Index of the controller being polled (first connected wins)
    DWORD m_userIndex;

    // Raw XInput state for the current and previous frame
    XINPUT_STATE m_currentState;
    XINPUT_STATE m_previousState;

    // Whether a controller was found during the last update()
    bool m_connected;

    // Helper: test a button bitmask against the current gamepad buttons
    bool isButtonDown(WORD mask) const;

    // Helper: test a button bitmask against the previous frame's buttons
    bool wasButtonDown(WORD mask) const;

    // Probe user indices 0–3 and return the first one that responds.
    // Sets m_userIndex and m_connected.
    void detectController();
};
