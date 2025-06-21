#ifndef PLATFORM_H
#define PLATFORM_H

// Platform detection (heavily plagerizing glfw3.h)
#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__))
    #define PLAT_WINDOWS
#elif (defined(__APPLE__)) // idk if there are other Apple-related defines
    #define PLAT_APPLE
#else // LINUX
    #define PLAT_LINUX
#endif

#endif // PLATFORM_H
