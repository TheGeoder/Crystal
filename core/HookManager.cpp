#include "HookManager.hpp"
#include "Logger.hpp"
#include <dlfcn.h>
#include <dobby.h>

HookManager& HookManager::get() {
    static HookManager instance;
    return instance;
}

void* HookManager::resolveSymbol(const std::string& symbolName) {
    if (!m_gdHandle) {
        m_gdHandle = dlopen("libcocos2dcpp.so", RTLD_NOLOAD | RTLD_NOW);
        if (!m_gdHandle) {
            LOGE("Could not find libcocos2dcpp.so in memory!");
            return nullptr;
        }
    }

    void* addr = dlsym(m_gdHandle, symbolName.c_str());
    if (!addr) {
        LOGW("Symbol not found: %s", symbolName.c_str());
    }
    return addr;
}

bool HookManager::createHook(void* targetAddress, void* detourFn, void** originalFn) {
    if (!targetAddress || !detourFn) return false;

    int status = DobbyHook(targetAddress, detourFn, originalFn);
    if (status != 0) {
        LOGE("DobbyHook failed at %p (error %d)", targetAddress, status);
        return false;
    }

    LOGI("Successfully hooked function at %p", targetAddress);
    return true;
}

bool HookManager::hookSymbol(const std::string& symbolName, void* detourFn, void** originalFn) {
    void* addr = resolveSymbol(symbolName);
    if (!addr) return false;
    return createHook(addr, detourFn, originalFn);
}
