#pragma once
#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "gameObject.hpp"
#include "texture.hpp"
#include "utils.hpp"

const int TOTAL_ASTEROIDS = 20;

enum asteroid_type
{
	BIG_ASTEROID,
	MEDIUM_ASTEROID,
	SMALL_ASTEROID
};

class Asteroid : public GameObject {
public:
	Asteroid(Pos t_pos, asteroid_type t_asteroid_type);
	~Asteroid();

	void move(double t_time_step);
	void setPos(Pos t_pos, double t_rot_degrees);
	void destroy(void);
	void free(void);

private:
	double m_dir_degrees{0}, m_vel{0};
};

#endif // !_ASTEROID_H_
