#include "../core/Loader.hpp"
#include "../core/Logger.hpp"
#include "../core/HookManager.hpp"
#include "../platform/android/AndroidPaths.hpp"
#include <jni.h>

// Stores the original MenuLayer::init function pointer
bool (*original_MenuLayer_init)(void* self) = nullptr;

// Runs when Geometry Dash loads the main menu
bool MenuLayer_init_hook(void* self) {
    LOGI("MenuLayer::init intercepted! GDModLoader v%s is active!", Loader::get().getVersion().c_str());

    // Call the original function so the main menu still renders normally
    return original_MenuLayer_init(self);
}

// Entry point called automatically when System.loadLibrary("loader") runs
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("=== GDModLoader JNI_OnLoad Triggered ===");

    // 1. Install a test hook on MenuLayer::init (ARM64 mangled symbol)
    HookManager::get().hookSymbol(
        "_ZN9MenuLayer4initEv",
        reinterpret_cast<void*>(&MenuLayer_init_hook),
        reinterpret_cast<void**>(&original_MenuLayer_init)
    );

    // 2. Find the mods folder and initialize the loader engine
    std::string modsDir = AndroidPaths::getModsDirectory();
    Loader::get().init(modsDir);

    return JNI_VERSION_1_6;
}
