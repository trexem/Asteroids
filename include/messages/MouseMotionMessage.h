#pragma once
#ifndef __MOUSE_MOTION_MESSAGE_H_
#define __MOUSE_MOTION_MESSAGE_H_

#include "message.h"
#include "FPair.h"

class MouseMotionMessage : public Message {
public:
    FPair mousePos;
    MouseMotionMessage(FPair mousePos) : mousePos(mousePos) {}
};

#endif // !__MOUSE_MOTION_MESSAGE_H_