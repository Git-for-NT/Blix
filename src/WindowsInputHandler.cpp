#include "WindowsInputHandler.h"

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

WindowsInputHandler::WindowsInputHandler()
    : left_{}
    , right_{}
    , down_{}
    , rotate_{}
    , hardDrop_{}
    , pause_{}
    , quit_{}
{
}

// ---------------------------------------------------------------------------
// Frame update — snapshot hardware state and compute edges
// ---------------------------------------------------------------------------

void WindowsInputHandler::update() {
    auto snap = [](KeyState& ks, bool newState) {
        ks.prev    = ks.current;
        ks.current = newState;
    };

    snap(left_,     isVKDown(VK_LEFT));
    snap(right_,    isVKDown(VK_RIGHT));
    snap(down_,     isVKDown(VK_DOWN));
    snap(rotate_,   isVKDown(VK_UP));
    snap(hardDrop_, isVKDown(VK_SPACE));
    // Pause fires on ESC or P
    snap(pause_,    isVKDown(VK_ESCAPE) || isVKDown('P'));
    // Quit also maps to ESC (game logic decides context)
    snap(quit_,     isVKDown(VK_ESCAPE));
}

// ---------------------------------------------------------------------------
// Private helper
// ---------------------------------------------------------------------------

bool WindowsInputHandler::isVKDown(int vkCode) {
    return (GetAsyncKeyState(vkCode) & 0x8000) != 0;
}

// ---------------------------------------------------------------------------
// Held-state queries
// ---------------------------------------------------------------------------

bool WindowsInputHandler::isLeftPressed()     const { return left_.current;     }
bool WindowsInputHandler::isRightPressed()    const { return right_.current;    }
bool WindowsInputHandler::isDownPressed()     const { return down_.current;     }
bool WindowsInputHandler::isRotatePressed()   const { return rotate_.current;   }
bool WindowsInputHandler::isHardDropPressed() const { return hardDrop_.current; }
bool WindowsInputHandler::isPausePressed()    const { return pause_.current;    }
bool WindowsInputHandler::isQuitPressed()     const { return quit_.current;     }

// ---------------------------------------------------------------------------
// Edge-triggered (just-pressed) queries
// true only on the first frame the key transitions from up -> down
// ---------------------------------------------------------------------------

bool WindowsInputHandler::wasJustLeftPressed()     const { return left_.current     && !left_.prev;     }
bool WindowsInputHandler::wasJustRightPressed()    const { return right_.current    && !right_.prev;    }
bool WindowsInputHandler::wasJustDownPressed()     const { return down_.current     && !down_.prev;     }
bool WindowsInputHandler::wasJustRotatePressed()   const { return rotate_.current   && !rotate_.prev;   }
bool WindowsInputHandler::wasJustHardDropPressed() const { return hardDrop_.current && !hardDrop_.prev; }
bool WindowsInputHandler::wasJustPausePressed()    const { return pause_.current    && !pause_.prev;    }
bool WindowsInputHandler::wasJustQuitPressed()     const { return quit_.current     && !quit_.prev;     }
