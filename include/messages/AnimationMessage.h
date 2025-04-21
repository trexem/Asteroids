#pragma once

#include <cstdint>

#include "Animation.h"
#include "message.h"

class AnimationMessage : public Message {
public:
    uint32_t eID;
    Animation animation;
    AnimationMessage(uint32_t eID, Animation animation) : eID(eID), animation(animation) {}
};
