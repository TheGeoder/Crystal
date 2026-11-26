set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build Universal Binary for Intel and Apple Silicon")
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version (Big Sur)")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,dynamic_lookup")
