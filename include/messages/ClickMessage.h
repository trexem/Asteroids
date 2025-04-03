#pragma once
#ifndef __CLICK_MESSAGE_H_
#define __CLICK_MESSAGE_H_

#include "message.h"
#include "FPair.h"

class ClickMessage : public Message {
public:
    FPair mousePos;
    uint8_t button, clicks;
    ClickMessage(FPair mousePos, uint8_t button, uint8_t clicks) 
        : mousePos(mousePos), button(button), clicks(clicks) {}
};

#endif // !__CLICK_MESSAGE_H_