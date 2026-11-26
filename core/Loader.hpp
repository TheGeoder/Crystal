#pragma once
#include <string>

class Loader {
public:
    static Loader& get();

    // Starts the loader engine (hooks + mod loading)
    void init(const std::string& modsDirectory);

    // Returns the current loader version
    std::string getVersion() const { return "0.1.0"; }

private:
    Loader() = default;
    bool m_initialized = false;
};
