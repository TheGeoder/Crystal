cmake_minimum_required(VERSION 3.21)
project(GDModLoader VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Fetch Dobby for inline hooking
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

# 2. Main Loader Core (Built on ALL 4 platforms)
add_library(LoaderCore SHARED
    src/main.cpp
    core/Loader.cpp
    core/ModManager.cpp
    core/HookManager.cpp
    sdk/StyleManager.cpp
)

target_include_directories(LoaderCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk
)

target_link_libraries(LoaderCore PRIVATE dobby)

# 3. Platform-specific targets & injection libraries
if(WIN32)
    set_target_properties(LoaderCore PROPERTIES OUTPUT_NAME "LoaderCore")
    target_sources(LoaderCore PRIVATE platform/windows/WindowsPaths.cpp)
    target_link_libraries(LoaderCore PRIVATE dbghelp)

    # Windows XInput9_1_0.dll proxy injector
    add_library(XInputProxy SHARED platform/windows/XInputProxy.cpp)
    set_target_properties(XInputProxy PROPERTIES OUTPUT_NAME "XInput9_1_0")

elseif(ANDROID)
    set_target_properties(LoaderCore PROPERTIES OUTPUT_NAME "loader")
    target_sources(LoaderCore PRIVATE platform/android/AndroidPaths.cpp)

    find_library(ANDROID_LOG_LIB log)
    find_library(ANDROID_EGL_LIB EGL)
    find_library(ANDROID_GLES_LIB GLESv2)
    target_link_libraries(LoaderCore PRIVATE ${ANDROID_LOG_LIB} ${ANDROID_EGL_LIB} ${ANDROID_GLES_LIB})

    # Android libinject.so bootstrapper
    add_library(InjectBootstrap SHARED platform/posix/InjectBootstrap.cpp)
    set_target_properties(InjectBootstrap PROPERTIES OUTPUT_NAME "inject")
    target_link_libraries(InjectBootstrap PRIVATE ${ANDROID_LOG_LIB})

elseif(IOS)
    set_target_properties(LoaderCore PROPERTIES
        OUTPUT_NAME "LoaderCore"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
    )
    target_sources(LoaderCore PRIVATE platform/ios/IOSPaths.cpp)
    target_link_options(LoaderCore PRIVATE "-Wl,-undefined,dynamic_lookup")

    # iOS libinject.dylib bootstrapper
    add_library(InjectBootstrap SHARED platform/posix/InjectBootstrap.cpp)
    set_target_properties(InjectBootstrap PROPERTIES
        OUTPUT_NAME "inject"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
    )

elseif(APPLE)
    set_target_properties(LoaderCore PROPERTIES
        OUTPUT_NAME "LoaderCore"
        OSX_ARCHITECTURES "x86_64;arm64"
    )
    target_sources(LoaderCore PRIVATE platform/macos/MacPaths.cpp)
    target_link_options(LoaderCore PRIVATE "-Wl,-undefined,dynamic_lookup")

    # macOS libinject.dylib bootstrapper
    add_library(InjectBootstrap SHARED platform/posix/InjectBootstrap.cpp)
    set_target_properties(InjectBootstrap PROPERTIES
        OUTPUT_NAME "inject"
        OSX_ARCHITECTURES "x86_64;arm64"
    )
endif()
