#pragma once
#include <string>

// Abstract interface for configuration and persistence management.
// Implementations can support different storage backends (file system,
// registry, in-memory for testing, etc.) by deriving from this class.
//
// Satisfies Requirements 10.1, 10.2, 10.3, 10.5:
//   10.1 - High scores saved to a local configuration file
//   10.2 - Saved settings loaded on startup
//   10.3 - Key binding changes persisted
//   10.5 - Default settings provided when no configuration file exists
class IConfigManager {
public:
    virtual ~IConfigManager() = default;

    // -------------------------------------------------------------------------
    // High score persistence (Requirement 10.1, 10.4)
    // -------------------------------------------------------------------------

    // Returns the stored high score, or 0 if none has been saved.
    virtual int loadHighScore() = 0;

    // Persists the given score as the new high score.
    virtual void saveHighScore(int score) = 0;

    // -------------------------------------------------------------------------
    // General settings – key/value pairs (Requirements 10.2, 10.3, 10.5)
    // -------------------------------------------------------------------------

    // Returns the value stored under 'key'.
    // If the key is absent, returns 'defaultValue' instead of throwing.
    virtual std::string getSetting(const std::string& key,
                                   const std::string& defaultValue) = 0;

    // Stores a value under 'key'.  Changes are held in memory until save() is
    // called.
    virtual void setSetting(const std::string& key,
                            const std::string& value) = 0;

    // Flushes all in-memory settings (and the current high score) to durable
    // storage.  Must be called explicitly; individual setSetting/saveHighScore
    // calls are not guaranteed to be durable until save() returns successfully.
    virtual void save() = 0;
};
