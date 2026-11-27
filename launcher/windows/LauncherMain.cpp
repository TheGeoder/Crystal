#include <windows.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    std::cout << "=== Crystal Installer / Launcher ===" << std::endl;

    // Default Steam path for Geometry Dash
    fs::path gdPath = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Geometry Dash";

    if (!fs::exists(gdPath / "GeometryDash.exe")) {
        std::cout << "Could not find GeometryDash.exe in default Steam folder." << std::endl;
        std::cout << "Place this .exe inside your Geometry Dash folder and run again." << std::endl;
        system("pause");
        return 1;
    }

    // Copy loader files into Geometry Dash folder
    fs::copy_file("XInput9_1_0.dll", gdPath / "XInput9_1_0.dll", fs::copy_options::overwrite_existing);
    fs::copy_file("LoaderCore.dll", gdPath / "LoaderCore.dll", fs::copy_options::overwrite_existing);

    std::cout << "Installed Crystal! Launching Geometry Dash..." << std::endl;
    ShellExecuteA(nullptr, "open", (gdPath / "GeometryDash.exe").string().c_str(), nullptr, gdPath.string().c_str(), SW_SHOWDEFAULT);
    return 0;
}
