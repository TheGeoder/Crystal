#include "IOSPaths.hpp"
#include "../../core/Logger.hpp"
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

namespace IOSPaths {
    std::string getModsDirectory() {
        const char* home = std::getenv("HOME");
        std::string sandboxRoot = home ? home : "";

        fs::path modsPath = fs::path(sandboxRoot) / "Documents" / "mods";

        std::error_code ec;
        if (!fs::exists(modsPath, ec)) {
            if (fs::create_directories(modsPath, ec)) {
                LOGI("Created iOS mods directory: %s", modsPath.string().c_str());
            } else {
                LOGE("Failed to create iOS mods directory: %s", ec.message().c_str());
            }
        }
        return modsPath.string();
    }
}
