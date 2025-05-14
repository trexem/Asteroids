#pragma once

#include "FPair.h"
#include "message.h"

class GraphicsSettingsMessage : public Message {
public:
    bool vsync, fullscreen;
    FPair prevSize, newSize;
    GraphicsSettingsMessage(bool f, bool v, FPair ps, FPair ns) 
        : fullscreen(f), vsync(v), prevSize(ps), newSize(ns) {}
};
