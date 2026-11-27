#include "AndroidPaths.hpp"
#include "../../core/Logger.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace AndroidPaths {

    // Reads /proc/self/cmdline to detect the exact APK package name automatically
    // (Works on vanilla GD, GD World, SubZero, Meltdown, or private servers)
    static std::string getPackageName() {
        std::ifstream cmdline("/proc/self/cmdline");
        std::string packageName;
        if (std::getline(cmdline, packageName, '\0')) {
            return packageName;
        }
        return "com.robtopx.geometryjump";
    }

    std::string getModsDirectory() {
        std::string pkg = getPackageName();
        std::string modsPath = "/storage/emulated/0/Android/data/" + pkg + "/files/mods";

        std::error_code ec;
        if (!fs::exists(modsPath, ec)) {
            if (fs::create_directories(modsPath, ec)) {
                LOGI("Created Android mods directory: %s", modsPath.c_str());
            } else {
                LOGE("Failed to create Android mods directory (%s): %s", modsPath.c_str(), ec.message().c_str());
            }
        }
        return modsPath;
    }
}
