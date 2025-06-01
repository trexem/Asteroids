#pragma once

#ifndef NDEBUG
    #include <SDL3/SDL_log.h>
    #define DEBUG_LOG(...) SDL_Log(__VA_ARGS__)
#else
    #define DEBUG_LOG(...) ((void)0)
#endif
