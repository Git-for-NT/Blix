#include "FileConfigManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

FileConfigManager::FileConfigManager(const std::string& filePath)
    : m_filePath(filePath)
{
    load();
}

// ---------------------------------------------------------------------------
// Private: parse file into m_settings
// ---------------------------------------------------------------------------

void FileConfigManager::load() {
    m_settings.clear();

    std::ifstream file(m_filePath);
    if (!file.is_open()) {
        // File doesn't exist yet — start with empty store; defaults apply
        // via getSetting's defaultValue parameter (Req 10.5).
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Strip comments and blank lines
        const std::string stripped = trim(line);
        if (stripped.empty() || stripped[0] == '#') continue;

        const auto sep = stripped.find('=');
        if (sep == std::string::npos) continue;  // malformed line, skip

        std::string key   = trim(stripped.substr(0, sep));
        std::string value = trim(stripped.substr(sep + 1));

        if (!key.empty()) {
            m_settings[key] = value;
        }
    }
}

// ---------------------------------------------------------------------------
// High score  (Req 10.1, 10.4)
// ---------------------------------------------------------------------------

int FileConfigManager::loadHighScore() {
    auto it = m_settings.find(HIGH_SCORE_KEY);
    if (it == m_settings.end()) return 0;

    try {
        return std::stoi(it->second);
    } catch (...) {
        return 0;  // corrupt value — treat as no high score
    }
}

void FileConfigManager::saveHighScore(int score) {
    m_settings[HIGH_SCORE_KEY] = std::to_string(score);
    save();  // persist immediately so a crash doesn't lose the score
}

// ---------------------------------------------------------------------------
// General settings  (Req 10.2, 10.3, 10.5)
// ---------------------------------------------------------------------------

std::string FileConfigManager::getSetting(const std::string& key,
                                          const std::string& defaultValue) {
    auto it = m_settings.find(key);
    return (it != m_settings.end()) ? it->second : defaultValue;
}

void FileConfigManager::setSetting(const std::string& key,
                                   const std::string& value) {
    m_settings[key] = value;
    // Changes are held in memory; caller must invoke save() to persist.
}

// ---------------------------------------------------------------------------
// Flush to disk  (Req 10.1, 10.2)
// ---------------------------------------------------------------------------

void FileConfigManager::save() {
    std::ofstream file(m_filePath, std::ios::trunc);
    if (!file.is_open()) return;  // silently skip if path is not writable

    file << "# Tetris / Blix configuration file\n";
    file << "# Edited automatically by the game — do not remove keys.\n\n";

    for (const auto& [key, value] : m_settings) {
        file << key << "=" << value << "\n";
    }
}

// ---------------------------------------------------------------------------
// Helper
// ---------------------------------------------------------------------------

std::string FileConfigManager::trim(const std::string& s) {
    const auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return {};
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}
