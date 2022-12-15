#pragma once
#include "Entity.h"
class Component
{
public:
	virtual ~Component() = default;

	//Update Component's state
	virtual void update(int d_t) = 0;

	//Access associated entity
	Entity* entity() const;

private:
	Entity* m_entity = nullptr;

	friend class Entity;
};

