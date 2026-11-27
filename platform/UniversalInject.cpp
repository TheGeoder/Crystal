// File: platform/UniversalInject.cpp
#include <string>
#include <filesystem>

// ============================================================================
// 1. WINDOWS: Acts as XInput9_1_0.dll proxy & injects LoaderCore.dll
// ============================================================================
#if defined(_WIN32)
    #include <windows.h>

    static HMODULE g_realXInput = nullptr;
    static HMODULE g_loaderCore = nullptr;

    using XInputGetState_t = DWORD(WINAPI*)(DWORD, void*);
    using XInputSetState_t = DWORD(WINAPI*)(DWORD, void*);
    using XInputGetCapabilities_t = DWORD(WINAPI*)(DWORD, DWORD, void*);

    static XInputGetState_t real_XInputGetState = nullptr;
    static XInputSetState_t real_XInputSetState = nullptr;
    static XInputGetCapabilities_t real_XInputGetCapabilities = nullptr;

    static void initWindowsProxy() {
        if (g_realXInput) return;

        char sysDir[MAX_PATH];
        GetSystemDirectoryA(sysDir, MAX_PATH);
        std::string realPath = std::string(sysDir) + "\\XInput9_1_0.dll";

        g_realXInput = LoadLibraryA(realPath.c_str());
        if (g_realXInput) {
            real_XInputGetState = reinterpret_cast<XInputGetState_t>(GetProcAddress(g_realXInput, "XInputGetState"));
            real_XInputSetState = reinterpret_cast<XInputSetState_t>(GetProcAddress(g_realXInput, "XInputSetState"));
            real_XInputGetCapabilities = reinterpret_cast<XInputGetCapabilities_t>(GetProcAddress(g_realXInput, "XInputGetCapabilities"));
        }

        // Inject LoaderCore.dll
        g_loaderCore = LoadLibraryA("LoaderCore.dll");
    }

    extern "C" __declspec(dllexport) DWORD WINAPI XInputGetState(DWORD dwUserIndex, void* pState) {
        initWindowsProxy();
        return real_XInputGetState ? real_XInputGetState(dwUserIndex, pState) : ERROR_DEVICE_NOT_CONNECTED;
    }

    extern "C" __declspec(dllexport) DWORD WINAPI XInputSetState(DWORD dwUserIndex, void* pVibration) {
        initWindowsProxy();
        return real_XInputSetState ? real_XInputSetState(dwUserIndex, pVibration) : ERROR_DEVICE_NOT_CONNECTED;
    }

    extern "C" __declspec(dllexport) DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, void* pCapabilities) {
        initWindowsProxy();
        return real_XInputGetCapabilities ? real_XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities) : ERROR_DEVICE_NOT_CONNECTED;
    }

    BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
        if (reason == DLL_PROCESS_ATTACH) {
            DisableThreadLibraryCalls(hModule);
            initWindowsProxy();
        }
        return TRUE;
    }

// ============================================================================
// 2. ANDROID: Acts as libinject.so & injects libloader.so
// ============================================================================
#elif defined(__ANDROID__)
    #include <dlfcn.h>
    #include <jni.h>
    #include <android/log.h>

    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
        __android_log_print(ANDROID_LOG_INFO, "GDInject", "UniversalInject loaded on Android! Injecting libloader.so...");

        void* core = dlopen("libloader.so", RTLD_NOW | RTLD_GLOBAL);
        if (!core) {
            __android_log_print(ANDROID_LOG_ERROR, "GDInject", "Failed to load libloader.so: %s", dlerror());
            return JNI_VERSION_1_6;
        }

        using JNIOnLoadFn = jint(*)(JavaVM*, void*);
        auto coreOnLoad = reinterpret_cast<JNIOnLoadFn>(dlsym(core, "JNI_OnLoad"));
        if (coreOnLoad) {
            return coreOnLoad(vm, reserved);
        }
        return JNI_VERSION_1_6;
    }

// ============================================================================
// 3. MACOS & IOS: Acts as libinject.dylib & injects libLoaderCore.dylib
// ============================================================================
#elif defined(__APPLE__)
    #include <dlfcn.h>
    #include <os/log.h>
    #include <mach-o/dyld.h>

    __attribute__((constructor))
    void on_apple_inject() {
        os_log_with_type(OS_LOG_DEFAULT, OS_LOG_TYPE_INFO, "UniversalInject loaded on Apple! Injecting libLoaderCore.dylib...");

        char exePath[1024];
        uint32_t size = sizeof(exePath);
        if (_NSGetExecutablePath(exePath, &size) == 0) {
            std::filesystem::path baseDir = std::filesystem::path(exePath).parent_path();
            std::filesystem::path corePath = baseDir / "libLoaderCore.dylib";

            if (!std::filesystem::exists(corePath)) {
                corePath = baseDir / "Frameworks" / "libLoaderCore.dylib";
            }

            void* core = dlopen(corePath.c_str(), RTLD_NOW | RTLD_GLOBAL);
            if (!core) {
                os_log_with_type(OS_LOG_DEFAULT, OS_LOG_TYPE_ERROR, "Failed to load libLoaderCore.dylib: %{public}s", dlerror());
            }
        }
    }
#endif
