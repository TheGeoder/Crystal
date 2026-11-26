#include "Loader.hpp"
#include "Logger.hpp"
#include "ModManager.hpp"

Loader& Loader::get() {
    static Loader instance;
    return instance;
}

void Loader::init(const std::string& modsDirectory) {
    if (m_initialized) {
        LOGW("Loader::init() called more than once, skipping.");
        return;
    }

    LOGI("Starting GDModLoader v%s...", getVersion().c_str());

    // Load all .so mods from the target directory
    ModManager::get().loadAllMods(modsDirectory);

    m_initialized = true;
    LOGI("GDModLoader v%s initialization complete!", getVersion().c_str());
}
