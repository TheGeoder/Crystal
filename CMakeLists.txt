cmake_minimum_required(VERSION 3.21)
project(Crystal VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Fetch Dobby (pinned & stripped of examples/tests so cross-compiling works)
include(FetchContent)
FetchContent_Declare(
    dobby
    GIT_REPOSITORY https://github.com
    GIT_TAG        master
)
set(DOBBY_GENERATE_SHARED OFF CACHE BOOL "" FORCE)
set(DOBBY_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(DOBBY_BUILD_TEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(dobby)

# 2. Main Crystal Core Library
add_library(CrystalCore SHARED
    src/main.cpp
    core/Loader.cpp
    core/ModManager.cpp
    core/HookManager.cpp
    sdk/StyleManager.cpp
)

target_include_directories(CrystalCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk
)

target_link_libraries(CrystalCore PRIVATE dobby)

# 3. Universal Injection Target (Matches --target UniversalInject in library.yml)
add_library(UniversalInject SHARED platform/UniversalInject.cpp)

if(WIN32)
    set_target_properties(CrystalCore PROPERTIES OUTPUT_NAME "CrystalCore")
    target_sources(CrystalCore PRIVATE platform/windows/WindowsPaths.cpp)
    target_link_libraries(CrystalCore PRIVATE dbghelp)

    set_target_properties(UniversalInject PROPERTIES OUTPUT_NAME "XInput9_1_0")

elseif(ANDROID)
    set_target_properties(CrystalCore PROPERTIES OUTPUT_NAME "crystal")
    target_sources(CrystalCore PRIVATE platform/android/AndroidPaths.cpp)
    find_library(ANDROID_LOG_LIB log)
    target_link_libraries(CrystalCore PRIVATE ${ANDROID_LOG_LIB})

    set_target_properties(UniversalInject PROPERTIES OUTPUT_NAME "inject")
    target_link_libraries(UniversalInject PRIVATE ${ANDROID_LOG_LIB})

elseif(IOS)
    set_target_properties(CrystalCore PROPERTIES
        OUTPUT_NAME "CrystalCore"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
    )
    target_sources(CrystalCore PRIVATE platform/ios/IOSPaths.cpp)
    target_link_options(CrystalCore PRIVATE "-Wl,-undefined,dynamic_lookup")

    set_target_properties(UniversalInject PROPERTIES
        OUTPUT_NAME "inject"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
    )

elseif(APPLE)
    set_target_properties(CrystalCore PROPERTIES
        OUTPUT_NAME "CrystalCore"
        OSX_ARCHITECTURES "x86_64;arm64"
    )
    target_sources(CrystalCore PRIVATE platform/macos/MacPaths.cpp)
    target_link_options(CrystalCore PRIVATE "-Wl,-undefined,dynamic_lookup")

    set_target_properties(UniversalInject PROPERTIES
        OUTPUT_NAME "inject"
        OSX_ARCHITECTURES "x86_64;arm64"
    )
endif()
