#include "runGame.h"
#if defined(__ANDROID__)
extern "C" int SDL_main(int argc, char* argv[]) {
    return runGame(); // call your normal main
}
#endif
