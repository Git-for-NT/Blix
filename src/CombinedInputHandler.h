#pragma once
#include "IInputHandler.h"
#include <vector>
#include <memory>

// ---------------------------------------------------------------------------
// CombinedInputHandler
//
// Aggregates one or more IInputHandler implementations and returns the
// logical OR of their states.  This satisfies Requirement 3.7:
//   "WHEN multiple input devices are connected, THE Input_Handler SHALL accept
//    input from any connected device simultaneously."
//
// Usage example:
//
//   auto kb  = std::make_unique<WindowsInputHandler>();
//   auto pad = std::make_unique<ControllerInputHandler>();
//
//   CombinedInputHandler combined;
//   combined.addHandler(std::move(kb));
//   combined.addHandler(std::move(pad));
//
// Call combined.update() once per frame; it delegates to every child handler.
// ---------------------------------------------------------------------------
class CombinedInputHandler : public IInputHandler {
public:
    CombinedInputHandler() = default;

    // Add an owned handler to the aggregator.
    void addHandler(std::unique_ptr<IInputHandler> handler) {
        m_handlers.push_back(std::move(handler));
    }

    // ------------------------------------------------------------------
    // Frame update — propagated to every child handler
    // ------------------------------------------------------------------
    void update() override {
        for (auto& h : m_handlers) h->update();
    }

    // ------------------------------------------------------------------
    // Held-state queries  (any child returning true wins)
    // ------------------------------------------------------------------
    bool isLeftPressed()     const override { return any(&IInputHandler::isLeftPressed);     }
    bool isRightPressed()    const override { return any(&IInputHandler::isRightPressed);    }
    bool isDownPressed()     const override { return any(&IInputHandler::isDownPressed);     }
    bool isRotatePressed()   const override { return any(&IInputHandler::isRotatePressed);   }
    bool isHardDropPressed() const override { return any(&IInputHandler::isHardDropPressed); }
    bool isPausePressed()    const override { return any(&IInputHandler::isPausePressed);    }
    bool isQuitPressed()     const override { return any(&IInputHandler::isQuitPressed);     }

    // ------------------------------------------------------------------
    // Just-pressed queries  (any child returning true wins)
    // ------------------------------------------------------------------
    bool wasJustLeftPressed()     const override { return any(&IInputHandler::wasJustLeftPressed);     }
    bool wasJustRightPressed()    const override { return any(&IInputHandler::wasJustRightPressed);    }
    bool wasJustDownPressed()     const override { return any(&IInputHandler::wasJustDownPressed);     }
    bool wasJustRotatePressed()   const override { return any(&IInputHandler::wasJustRotatePressed);   }
    bool wasJustHardDropPressed() const override { return any(&IInputHandler::wasJustHardDropPressed); }
    bool wasJustPausePressed()    const override { return any(&IInputHandler::wasJustPausePressed);    }
    bool wasJustQuitPressed()     const override { return any(&IInputHandler::wasJustQuitPressed);     }

private:
    std::vector<std::unique_ptr<IInputHandler>> m_handlers;

    // Helper: returns true if at least one child handler returns true
    // for the given member-function pointer.
    using BoolQuery = bool (IInputHandler::*)() const;
    bool any(BoolQuery fn) const {
        for (const auto& h : m_handlers) {
            if ((h.get()->*fn)()) return true;
        }
        return false;
    }
};
