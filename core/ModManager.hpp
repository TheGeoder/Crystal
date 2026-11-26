#pragma once
#include <string>
#include <vector>

struct LoadedMod {
    std::string name;
    std::string path;
    void* handle;
};

class ModManager {
public:
    static ModManager& get();

    // Scans the mods folder and loads all .so files
    void loadAllMods(const std::string& modsDir);

    const std::vector<LoadedMod>& getLoadedMods() const { return m_mods; }

private:
    ModManager() = default;
    std::vector<LoadedMod> m_mods;
};
