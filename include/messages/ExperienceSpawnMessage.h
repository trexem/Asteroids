#pragma once

#include "FPair.h"
#include "message.h"

class ExperienceSpawnMessage : public Message {
public:
    FPair position;
    unsigned int amount;
    ExperienceSpawnMessage(FPair pos, unsigned int am) : position(pos), amount(am) {}
};
