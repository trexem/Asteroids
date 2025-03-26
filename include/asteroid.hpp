#pragma once
#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "gameObject.hpp"

const int TOTAL_ASTEROIDS = 20;

enum asteroid_type
{
	BIG_ASTEROID,
	MEDIUM_ASTEROID,
	SMALL_ASTEROID
};

class Asteroid : public GameObject {
public:
	Asteroid(FPair t_pos, asteroid_type t_asteroid_type);
	~Asteroid();

	void move(double t_time_step);
	void render(void);
	void setPos(FPair t_pos, double t_rot_degrees);
	void destroy(void);
	void free(void);

	bool isDead(void);
	bool isDying(void);

private:
	double m_dir_degrees{0}, m_vel{0};
	bool m_is_dying{ false }, m_is_dead{ false };
};

#endif // !_ASTEROID_H_
