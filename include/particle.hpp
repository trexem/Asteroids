#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "gameObject.hpp"

const int TOTAL_PARTICLES = 40;

class Particle : public GameObject {
public:
	Particle(FPair t_pos);

	void render(void);
	void pause(void);
	void resume(void);
	void kill(void);

	void setPos(FPair t_pos);

	bool isDead();

private:

	int m_frame{0};
	int m_shim_frame{0};
	bool m_paused{false};
};
#endif // !_PARTICLE_H_