#include "ModManager.hpp"
#include "Logger.hpp"
#include <filesystem>
#include <dlfcn.h>

namespace fs = std::filesystem;

ModManager& ModManager::get() {
    static ModManager instance;
    return instance;
}

void ModManager::loadAllMods(const std::string& modsDir) {
    std::error_code ec;
    if (!fs::exists(modsDir, ec)) {
        LOGW("Mods directory does not exist: %s", modsDir.c_str());
        return;
    }

    for (const auto& entry : fs::directory_iterator(modsDir, ec)) {
        if (!entry.is_regular_file()) continue;

        auto path = entry.path();
        if (path.extension() == ".so") {
            std::string pathStr = path.string();
            std::string fileName = path.filename().string();

            LOGI("Loading mod: %s", fileName.c_str());

            // Load the mod's .so binary into Geometry Dash's process
            void* handle = dlopen(pathStr.c_str(), RTLD_NOW | RTLD_GLOBAL);
            if (!handle) {
                LOGE("Failed to load %s: %s", fileName.c_str(), dlerror());
                continue;
            }

            // Look for an optional entry function named "mod_main" inside the mod
            using ModMainFn = void(*)();
            auto modMain = reinterpret_cast<ModMainFn>(dlsym(handle, "mod_main"));
            if (modMain) {
                modMain();
            }

            m_mods.push_back({ fileName, pathStr, handle });
            LOGI("Successfully loaded mod: %s", fileName.c_str());
        }
    }

    LOGI("Finished loading %zu mod(s).", m_mods.size());
}
