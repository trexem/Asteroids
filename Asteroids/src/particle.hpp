#pragma on
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "texture.hpp"
const int TOTAL_PARTICLES = 40;
struct pos
{
	int x;
	int y;
};
class Particle {
public:
	Particle(int x, int y);
	Particle(pos p);

	void render();
	void setPos(int x, int y);
	void kill();

	bool isDead();

private:
	int m_pos_x, m_pos_y;

	int m_frame;
	int m_shim_frame;

	Texture* m_texture;
};
#endif // !_PARTICLE_H_