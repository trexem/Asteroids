#pragma once

#include "message.h"
#include "abilities.h"

class LevelUpMessage : public Message {
public:
	ShipAbilities ability;
	LevelUpMessage(ShipAbilities a) : ability(a) {}
};
