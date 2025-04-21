#pragma once

#include "message.h"
#include "FPair.h"

class MouseMotionMessage : public Message {
public:
    FPair mousePos;
    MouseMotionMessage(FPair mousePos) : mousePos(mousePos) {}
};
