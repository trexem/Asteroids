#include "particle.hpp"

Particle::Particle(int x, int y) {
	setPos(x, y);
	m_texture = &g_particle_texture;
}

Particle::Particle(Pos p) {
	setPos(p.x, p.y);
	m_texture = &g_particle_texture;
}

void Particle::render() {
	if (m_frame % 6 == 0 || m_shim_frame < 3) {
		g_particle_shimmer_texture.render(m_pos_x,m_pos_y);
		if (!m_paused)
			m_shim_frame++;
	}
	else {
		m_texture->render(m_pos_x, m_pos_y);
		if (!m_paused)
			m_shim_frame = 0;
	}
	if(!m_paused)
		m_frame++;
}

void Particle::setPos(int x, int y) {
	m_pos_x = x - 5 + (rand() % 10);
	m_pos_y = y - 5 + (rand() % 10);
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