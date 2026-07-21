#include "ControllerInputHandler.h"
#include <cstring>

// ---------------------------------------------------------------------------
// XInput button masks used for the Tetris mapping
// ---------------------------------------------------------------------------
static constexpr WORD BTN_DPAD_UP    = XINPUT_GAMEPAD_DPAD_UP;
static constexpr WORD BTN_DPAD_DOWN  = XINPUT_GAMEPAD_DPAD_DOWN;
static constexpr WORD BTN_DPAD_LEFT  = XINPUT_GAMEPAD_DPAD_LEFT;
static constexpr WORD BTN_DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT;
static constexpr WORD BTN_A          = XINPUT_GAMEPAD_A;
static constexpr WORD BTN_B          = XINPUT_GAMEPAD_B;
static constexpr WORD BTN_START      = XINPUT_GAMEPAD_START;
static constexpr WORD BTN_SELECT     = XINPUT_GAMEPAD_BACK;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

ControllerInputHandler::ControllerInputHandler()
    : m_userIndex(0)
    , m_connected(false)
{
    ZeroMemory(&m_currentState,  sizeof(m_currentState));
    ZeroMemory(&m_previousState, sizeof(m_previousState));

    detectController();
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void ControllerInputHandler::detectController() {
    XINPUT_STATE state{};
    for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i) {
        ZeroMemory(&state, sizeof(state));
        if (XInputGetState(i, &state) == ERROR_SUCCESS) {
            m_userIndex      = i;
            m_connected      = true;
            m_currentState   = state;
            m_previousState  = state;
            return;
        }
    }
    m_connected = false;
    m_userIndex = 0;
}

bool ControllerInputHandler::isButtonDown(WORD mask) const {
    return (m_currentState.Gamepad.wButtons & mask) != 0;
}

bool ControllerInputHandler::wasButtonDown(WORD mask) const {
    return (m_previousState.Gamepad.wButtons & mask) != 0;
}

// ---------------------------------------------------------------------------
// Frame update
// ---------------------------------------------------------------------------

void ControllerInputHandler::update() {
    m_previousState = m_currentState;

    if (m_connected) {
        XINPUT_STATE newState{};
        DWORD result = XInputGetState(m_userIndex, &newState);
        if (result == ERROR_SUCCESS) {
            m_currentState = newState;
        } else {
            // Controller disconnected; try to find another one
            m_connected = false;
            ZeroMemory(&m_currentState, sizeof(m_currentState));
            detectController();
        }
    } else {
        // Periodically probe for a newly connected controller
        detectController();
    }
}

// ---------------------------------------------------------------------------
// Connection query
// ---------------------------------------------------------------------------

bool ControllerInputHandler::isConnected() const {
    return m_connected;
}

// ---------------------------------------------------------------------------
// Held-state queries
// ---------------------------------------------------------------------------

bool ControllerInputHandler::isLeftPressed() const {
    return isButtonDown(BTN_DPAD_LEFT);
}

bool ControllerInputHandler::isRightPressed() const {
    return isButtonDown(BTN_DPAD_RIGHT);
}

bool ControllerInputHandler::isDownPressed() const {
    return isButtonDown(BTN_DPAD_DOWN);
}

bool ControllerInputHandler::isRotatePressed() const {
    // D-Pad Up or A button both trigger rotate
    return isButtonDown(BTN_DPAD_UP) || isButtonDown(BTN_A);
}

bool ControllerInputHandler::isHardDropPressed() const {
    return isButtonDown(BTN_B);
}

bool ControllerInputHandler::isPausePressed() const {
    return isButtonDown(BTN_START);
}

bool ControllerInputHandler::isQuitPressed() const {
    // Select / Back button maps to quit
    return isButtonDown(BTN_SELECT);
}

// ---------------------------------------------------------------------------
// Just-pressed queries  (rising-edge: not pressed last frame, pressed now)
// ---------------------------------------------------------------------------

bool ControllerInputHandler::wasJustLeftPressed() const {
    return isButtonDown(BTN_DPAD_LEFT) && !wasButtonDown(BTN_DPAD_LEFT);
}

bool ControllerInputHandler::wasJustRightPressed() const {
    return isButtonDown(BTN_DPAD_RIGHT) && !wasButtonDown(BTN_DPAD_RIGHT);
}

bool ControllerInputHandler::wasJustDownPressed() const {
    return isButtonDown(BTN_DPAD_DOWN) && !wasButtonDown(BTN_DPAD_DOWN);
}

bool ControllerInputHandler::wasJustRotatePressed() const {
    bool upNow   = isButtonDown(BTN_DPAD_UP);
    bool aNow    = isButtonDown(BTN_A);
    bool upPrev  = wasButtonDown(BTN_DPAD_UP);
    bool aPrev   = wasButtonDown(BTN_A);

    // Rising edge on either D-Up or A (handles the two-button alias)
    return (upNow && !upPrev) || (aNow && !aPrev);
}

bool ControllerInputHandler::wasJustHardDropPressed() const {
    return isButtonDown(BTN_B) && !wasButtonDown(BTN_B);
}

bool ControllerInputHandler::wasJustPausePressed() const {
    return isButtonDown(BTN_START) && !wasButtonDown(BTN_START);
}

bool ControllerInputHandler::wasJustQuitPressed() const {
    return isButtonDown(BTN_SELECT) && !wasButtonDown(BTN_SELECT);
}
