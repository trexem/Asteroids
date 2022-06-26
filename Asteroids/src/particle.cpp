#include "particle.hpp"

Particle::Particle(Pos t_pos) : GameObject(t_pos) {
	m_texture = &g_particle_texture;
	m_frame = 0;
	m_shim_frame = 0;
	m_paused = false;
}

void Particle::render() {
	if (m_frame % 6 == 0 || m_shim_frame < 3) {
		g_particle_shimmer_texture.render(m_pos.x,m_pos.y);
		if (!m_paused)
			m_shim_frame++;
	}
	else {
		m_texture->render(m_pos.x, m_pos.y);
		if (!m_paused)
			m_shim_frame = 0;
	}
	if(!m_paused)
		m_frame++;
}

void Particle::setPos(Pos t_pos) {
	m_pos.x = t_pos.x - 5 + (rand() % 10);
	m_pos.y = t_pos.y - 5 + (rand() % 10);
	m_frame = rand() % 30;
}

void Particle::pause() { m_paused = true; }

void Particle::resume() { m_paused = false; }

void Particle::kill() {
	m_frame = 100;
}

bool Particle::isDead() {
	return m_frame > 40;
}