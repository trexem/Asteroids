#include "asteroid.hpp"

Pos g_screen_center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

Asteroid::Asteroid(Pos t_pos, asteroid_type t_asteroid_type) {
	m_pos = t_pos;
	m_rot_degrees = rand() % 180;
	m_vel = rand() % 100 + 50;
	switch (t_asteroid_type)
	{
	case BIG_ASTEROID:
		m_texture = &g_asteroid_big_texture;
		break;
	case MEDIUM_ASTEROID:
		break;
	case SMALL_ASTEROID:
		break;
	default:
		break;
	}
	m_dir_degrees = atan((g_screen_center.x - m_pos.x - m_texture->getWidth() / 2) / (g_screen_center.y - m_pos.y - m_texture->getHeight() / 2)) - .087 + (rand() % 175) / 174;
	if (g_screen_center.y - m_pos.y < 0) {
		m_dir_degrees += PI;
	}
}

Asteroid::~Asteroid(){

}

void Asteroid::move(double t_time_step) {
	m_pos.x += m_vel * t_time_step * sin(m_dir_degrees);
	m_pos.y += m_vel * t_time_step * cos(m_dir_degrees);
}

void Asteroid::setPos(Pos t_pos, double t_rot_degrees) {
	m_pos = t_pos;
	m_rot_degrees = t_rot_degrees;
	m_dir_degrees = atan((g_screen_center.y - m_pos.y) / (g_screen_center.x - m_pos.x)) - 1 + (rand() % 200) / 100;
}

void Asteroid::destroy() {
	//Animation on destroy
}

void Asteroid::free() {
	m_texture->free();
}
