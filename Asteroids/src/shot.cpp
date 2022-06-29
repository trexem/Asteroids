#include "shot.hpp"

Shot::Shot(int t_x, int t_y, double t_degrees, Texture* t_texture) {
	m_pos.x = t_x;
	m_pos.y = t_y;
	m_rot_degrees = t_degrees;
	m_texture = t_texture;
	m_collider.h = m_texture->getHeight();
	m_collider.w = m_texture->getWidth();
	m_is_dead = false;
}

void Shot::move(double t_time_step) {
	if (!isDead()) {
		//Convert degrees to radians
		double radians = m_rot_degrees * PI / 180;
		m_pos.x += SHOT_SPEED * t_time_step * sin(radians);
		m_pos.y -= SHOT_SPEED * t_time_step * cos(radians);
		if (m_pos.x > SCREEN_WIDTH || m_pos.x < 0 || m_pos.y > SCREEN_HEIGHT || m_pos.y < 0) {
			m_is_dead = true;
		}
		m_collider.x = m_pos.x;
		m_collider.y = m_pos.y;
	}
}

void Shot::render() {
	if (!isDead()) {
		m_texture->renderEx((int)m_pos.x, (int)m_pos.y, nullptr, m_rot_degrees, nullptr, SDL_FLIP_NONE);
	}
}

void Shot::kill() { m_is_dead = true; }

bool Shot::isDead() { return m_is_dead; }
