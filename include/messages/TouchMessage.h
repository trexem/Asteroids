#pragma once

#include "message.h"
#include "FPair.h"

class TouchMessage : public Message {
public:
	FPair pos;
	Uint64 id;
	bool isDown;
	TouchMessage(FPair pos, bool down, Uint64 id) : pos(pos), isDown(down), id(id) {}
};
