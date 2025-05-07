#pragma once

#include "message.h"
#include "FPair.h"

#include <cstdint>

class ClickMessage : public Message {
public:
    FPair mousePos;
    uint8_t button, clicks;
    bool isDown;
    ClickMessage(FPair mousePos, bool down, uint8_t button, uint8_t clicks) 
        : mousePos(mousePos), button(button), clicks(clicks), isDown(down) {}
};
