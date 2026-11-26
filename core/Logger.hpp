#pragma once

#if defined(__ANDROID__)
    #include <android/log.h>
    #define LOG_TAG "GDModLoader"

    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
    #include <cstdio>
    #define LOGI(...) do { printf("[INFO] "  __VA_ARGS__); printf("\n"); } while (0)
    #define LOGW(...) do { printf("[WARN] "  __VA_ARGS__); printf("\n"); } while (0)
    #define LOGE(...) do { printf("[ERROR] " __VA_ARGS__); printf("\n"); } while (0)
#endif
