cmake_minimum_required(VERSION 3.21)
project(GDModLoader VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Automatically download Dobby for inline hooking
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

# 2. Build the core shared library with all your new files
add_library(LoaderCore SHARED
    src/main.cpp
    core/Loader.cpp
    core/ModManager.cpp
    core/HookManager.cpp
    platform/android/AndroidPaths.cpp
)

target_include_directories(LoaderCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/android
)

# 3. Android 64-bit output naming (libloader.so) & NDK system libraries
if(ANDROID)
    set_target_properties(LoaderCore PROPERTIES OUTPUT_NAME "loader")

    find_library(ANDROID_LOG_LIB log)
    find_library(ANDROID_EGL_LIB EGL)
    find_library(ANDROID_GLES_LIB GLESv2)

    target_link_libraries(LoaderCore PRIVATE
        dobby
        ${ANDROID_LOG_LIB}
        ${ANDROID_EGL_LIB}
        ${ANDROID_GLES_LIB}
    )
endif()
