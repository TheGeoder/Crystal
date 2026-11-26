#pragma once
#include <string>
#include <cstdint>

class HookManager {
public:
    static HookManager& get();

    // Finds a Geometry Dash function by its C++ symbol name inside libcocos2dcpp.so
    void* resolveSymbol(const std::string& symbolName);

    // Hooks a function at `targetAddress` with `detourFn`, saving the original call in `originalFn`
    bool createHook(void* targetAddress, void* detourFn, void** originalFn);

    // Shortcut: Resolves the symbol name AND hooks it in one call
    bool hookSymbol(const std::string& symbolName, void* detourFn, void** originalFn);

private:
    HookManager() = default;
    void* m_gdHandle = nullptr;
};
