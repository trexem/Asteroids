#pragma once

#include "message.h"
#include "passiveAbilities.h"
#include "weaponAbilities.h"

class LevelUpMessage : public Message {
public:
	AbilityChoice choice;
	LevelUpMessage(AbilityChoice c) : choice(c) {}
};
