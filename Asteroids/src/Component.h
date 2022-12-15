#pragma once
#ifndef __COMPONENT_H_
#define __COMPONENT_H_

class Entity;

class Component {
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
#endif // !__COMPONENT_H_
