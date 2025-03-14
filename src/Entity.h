#pragma once
#ifndef __ENTITY_H_
#define __ENTITY_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <SDL3/SDL.h>

#include "Component.h"

using ComponentMap = std::unordered_map<std::string, std::vector<Component*>>;

class Entity
{
public:
	virtual ~Entity() = default;

	//Update Entity and all its components
	void update(int d_t);
	virtual void render();

	//Add new component
	template <typename T, typename... Args>
	T* addComponent(Args&&... args) {
		auto component = new T{ std::forward<Args>(args)... };
		component->m_entity = this;
		m_components[typeid(T).name()].emplace_back(component);

		return component;
	}

	//Get a vector of components
	template <typename T>
	std::vector<T*> getComponents() const {
		std::vector<T*> results;
		auto range = m_components.equal_range(typeid(T).name());
		for (auto it = range.first; it != range.second; ++it) {
			for (auto& component : it->second) {
				results.emplace_back(static_cast<T*>(component));
			}
		}
		return results;
	}

private:
	ComponentMap m_components;
};
#endif // !__ENTITY_H_
