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

void GameObject::renderEx() {
	m_texture->renderEx((int)m_pos.x, (int)m_pos.y, nullptr, m_rot_degrees, nullptr, SDL_FLIP_NONE);
}

void GameObject::render() {
	m_texture->render((int)m_pos.x, (int)m_pos.y);
}

void GameObject::setPos(Pos t_pos, double t_rot_degrees) {
	m_pos = t_pos;
	m_rot_degrees = t_rot_degrees;
}

Pos GameObject::getPos() { return m_pos; }

double GameObject::getX() { return m_pos.x; }

double GameObject::getY() { return m_pos.y; }

double GameObject::getDegrees() {return m_rot_degrees;}
