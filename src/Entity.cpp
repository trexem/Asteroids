#include "Entity.h"
#include "RenderComponent.h"

void Entity::update(int d_t) {
	for (auto& component_type : m_components) {
		for (auto& component : component_type.second) {
			component->update(d_t);
		}
	}
}
