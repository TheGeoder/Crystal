#pragma once
#include <string>
#include <cstdint>
#include <vector>

struct ThemeColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct LoaderTheme {
    std::string id;
    std::string displayName;
    ThemeColor primary;
    ThemeColor background;
    ThemeColor accent;
};

class StyleManager {
public:
    static StyleManager& get();

    // Sets the active theme (defaults to "Amethyst")
    void setTheme(const std::string& themeName);

    const LoaderTheme& getCurrentTheme() const { return m_currentTheme; }
    std::vector<std::string> getAvailableThemes() const;

private:
    StyleManager();
    LoaderTheme m_currentTheme;
};
