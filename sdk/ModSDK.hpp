#pragma once
#include "../core/Logger.hpp"
#include "../core/HookManager.hpp"
#include "StyleManager.hpp"

#if defined(_WIN32)
    #define MOD_EXPORT extern "C" __declspec(dllexport)
#else
    #define MOD_EXPORT extern "C" __attribute__((visibility("default")))
#endif

#define MOD_MAIN() MOD_EXPORT void mod_main()
