#pragma once
#include "IConfigManager.h"
#include <string>
#include <unordered_map>

// ---------------------------------------------------------------------------
// FileConfigManager
//
// Concrete IConfigManager that persists settings in a simple key=value file
// (one entry per line, blank lines and lines starting with '#' are ignored).
//
// Default file location: "tetris_config.ini" in the current working directory.
//
// High score is stored as the special key "high_score".
//
// Satisfies Requirements 10.1, 10.2, 10.5:
//   10.1 – High scores saved to a local configuration file
//   10.2 – Saved settings loaded on startup
//   10.5 – Default settings provided when no configuration file exists
// ---------------------------------------------------------------------------
class FileConfigManager : public IConfigManager {
public:
    // Constructs the manager and immediately loads from filePath.
    // If the file does not exist the in-memory store is empty (defaults apply).
    explicit FileConfigManager(const std::string& filePath = "tetris_config.ini");

    // -------------------------------------------------------------------------
    // IConfigManager — high score
    // -------------------------------------------------------------------------
    int  loadHighScore()          override;
    void saveHighScore(int score) override;

    // -------------------------------------------------------------------------
    // IConfigManager — general settings
    // -------------------------------------------------------------------------
    std::string getSetting(const std::string& key,
                           const std::string& defaultValue) override;

    void setSetting(const std::string& key,
                    const std::string& value) override;

    // Flush all in-memory settings to disk.
    void save() override;

private:
    std::string m_filePath;

    // In-memory key→value store (all values stored as strings).
    std::unordered_map<std::string, std::string> m_settings;

    // Parse the config file into m_settings.  Called once in the constructor.
    void load();

    // Trim leading/trailing whitespace from a string (helper).
    static std::string trim(const std::string& s);

    static constexpr const char* HIGH_SCORE_KEY = "high_score";
};
