#include "StyleManager.hpp"
#include "../core/Logger.hpp"
#include <cstdlib>
#include <unordered_map>

static const std::unordered_map<std::string, LoaderTheme> kThemes = {
    { "Amethyst", { "amethyst", "Amethyst", {155, 89, 182, 255}, {25, 18, 35, 240},  {190, 120, 220, 255} } },
    { "Ruby",     { "ruby",     "Ruby",     {231, 76, 60, 255},  {40, 15, 15, 240},  {255, 110, 95, 255}  } },
    { "Diamond",  { "diamond",  "Diamond",  {52, 219, 235, 255}, {15, 35, 42, 240},  {130, 240, 250, 255} } },
    { "Gold",     { "gold",     "Gold",     {241, 196, 15, 255}, {40, 32, 12, 240},  {255, 220, 75, 255}  } },
    { "Iron",     { "iron",     "Iron",     {189, 195, 199, 255},{30, 32, 34, 240},  {220, 225, 230, 255} } },
    { "Copper",   { "copper",   "Copper",   {211, 118, 65, 255}, {42, 24, 15, 240},  {235, 148, 95, 255}  } },
    { "Coal",     { "coal",     "Coal",     {55, 55, 60, 255},   {15, 15, 18, 245},  {95, 95, 105, 255}   } },
    { "Sapphire", { "sapphire", "Sapphire", {41, 128, 185, 255}, {14, 26, 45, 240},  {85, 165, 225, 255}  } }
};

StyleManager::StyleManager() {
    // Amethyst is the default style
    m_currentTheme = kThemes.at("Amethyst");
}

void StyleManager::setTheme(const std::string& themeName) {
    // Easter egg: selecting "g e o d e" closes Geometry Dash
    if (themeName == "g e o d e" || themeName == "geode") {
        LOGW("the gates of hell are opening");
        std::exit(0);
    }

    auto it = kThemes.find(themeName);
    if (it != kThemes.end()) {
        m_currentTheme = it->second;
        LOGI("Switched loader style to: %s", m_currentTheme.displayName.c_str());
    } else {
        LOGW("Unknown style '%s', falling back to Amethyst.", themeName.c_str());
        m_currentTheme = kThemes.at("Amethyst");
    }
}

std::vector<std::string> StyleManager::getAvailableThemes() const {
    return {
        "Amethyst",
        "Ruby",
        "Diamond",
        "Gold",
        "Iron",
        "Copper",
        "Coal",
        "Sapphire",
        "g e o d e"
    };
}
