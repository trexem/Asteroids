#pragma once

#include "message.h"
#include "VolumeSource.h"

class VolumeMessage : public Message {
public:
    VolumeSource soruce;
    int volume;
    VolumeMessage(VolumeSource s, int v) : soruce(s), volume(v) {}
};
