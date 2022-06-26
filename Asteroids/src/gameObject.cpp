#include "gameObject.hpp"

GameObject::GameObject() {
	m_pos = { 0,0 };
	m_texture = nullptr;
}

GameObject::GameObject(Pos p) {
	m_pos = p;
	m_texture = nullptr;
}

GameObject::~GameObject() {

}

void GameObject::render() {
	m_texture->render(m_pos.x, m_pos.y);
}

void GameObject::setPos(Pos p) {
	m_pos = p;
}

Pos GameObject::getPos() { return m_pos; }