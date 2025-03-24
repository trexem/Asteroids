#pragma once
#ifndef __APPLY_FORCE_MESSAGE_H_
#define __APPLY_FORCE_MESSAGE_H_

#include "message.h"
#include "FPair.h"

class ApplyForceMessage : public Message {
public:
    uint32_t eID;
    FPair force;
    ApplyForceMessage(uint32_t eID, FPair force) : eID(eID), force(force) {}
};

#endif //!__APPLY_FORCE_MESSAGE_H_
