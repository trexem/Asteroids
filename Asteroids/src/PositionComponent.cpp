#include "PositionComponent.h"

PositionComponent::PositionComponent(int t_x, int t_y) {
	m_x = t_x;
	m_y = t_y;
}

void PositionComponent::update(int d_t) {
	m_x += d_t * m_x_velocity;
	m_y += d_t * m_y_velocity;
}

int PositionComponent::x() const { return m_x; }
int PositionComponent::y() const { return m_y; }

void PositionComponent::set_x(int t_x) { m_x = t_x; }
void PositionComponent::set_y(int t_y) { m_x = t_y; }

int PositionComponent::x_velocity() const { return m_x_velocity; }
int PositionComponent::y_velocity() const { return m_y_velocity; }

void PositionComponent::set_x_velocity(int t_x_vel) { m_x_velocity = t_x_vel; }
void PositionComponent::set_y_velocity(int t_y_vel) { m_y_velocity = t_y_vel; }
