#include "WindowsPaths.hpp"
#include "../../core/Logger.hpp"
#include <windows.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace WindowsPaths {
    std::string getModsDirectory() {
        char exePath[MAX_PATH];
        GetModuleFileNameA(nullptr, exePath, MAX_PATH);

        fs::path gameDir = fs::path(exePath).parent_path();
        fs::path modsPath = gameDir / "loader" / "mods";

        std::error_code ec;
        if (!fs::exists(modsPath, ec)) {
            if (fs::create_directories(modsPath, ec)) {
                LOGI("Created Windows mods directory: %s", modsPath.string().c_str());
            } else {
                LOGE("Failed to create Windows mods directory: %s", ec.message().c_str());
            }
        }
        return modsPath.string();
    }
}
