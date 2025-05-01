#pragma once

#include "EntityType.h"
#include "FPair.h"
#include "message.h"

class PickupsSpawnMessage : public Message {
public:
    FPair position;
    EntityType type;
    unsigned int lvl;
    PickupsSpawnMessage(FPair pos, unsigned int l, EntityType t) : 
        position(pos), lvl(l), type(t) {}
};
