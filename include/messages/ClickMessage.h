#pragma once

#include "message.h"
#include "FPair.h"

#include <cstdint>

class ClickMessage : public Message {
public:
    FPair mousePos;
    uint8_t button, clicks;
    ClickMessage(FPair mousePos, uint8_t button, uint8_t clicks) 
        : mousePos(mousePos), button(button), clicks(clicks) {}
};
