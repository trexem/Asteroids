#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "texture.hpp"
#include "utils.hpp"

const int TOTAL_PARTICLES = 40;

class Particle {
public:
	Particle(int x, int y);
	Particle(Pos p);

	void render(void);
	void setPos(int x, int y);
	void pause(void);
	void resume(void);
	void kill(void);

	bool isDead();

private:
	int m_pos_x, m_pos_y;

	int m_frame;
	int m_shim_frame;
	bool m_paused;
	Texture* m_texture;
};
#endif // !_PARTICLE_H_