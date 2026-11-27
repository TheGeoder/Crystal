#include "MacPaths.hpp"
#include "../../core/Logger.hpp"
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

namespace MacPaths {
    std::string getModsDirectory() {
        const char* home = std::getenv("HOME");
        std::string baseDir = home ? home : "/tmp";

        fs::path modsPath = fs::path(baseDir) / "Library" / "Application Support" / "GeometryDash" / "loader" / "mods";

        std::error_code ec;
        if (!fs::exists(modsPath, ec)) {
            if (fs::create_directories(modsPath, ec)) {
                LOGI("Created macOS mods directory: %s", modsPath.string().c_str());
            } else {
                LOGE("Failed to create macOS mods directory: %s", ec.message().c_str());
            }
        }
        return modsPath.string();
    }
}
