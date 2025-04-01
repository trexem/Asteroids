#pragma once
#ifndef __ANIMATION_MESSAGE_H_
#define __ANIMATION_MESSAGE_H_

#include <cstdint>

#include "Animation.h"
#include "message.h"

class AnimationMessage : public Message {
public:
    uint32_t eID;
    Animation animation;
    AnimationMessage(uint32_t eID, Animation animation) : eID(eID), animation(animation) {}
};

#endif // !__ANIMATION_MESSAGE_H_
