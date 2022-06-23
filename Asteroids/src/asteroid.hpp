#pragma once
#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "texture.hpp"
#include "utils.hpp"

const int TOTAL_ASTEROIDS = 20;

enum asteroid_type
{
	BIG_ASTEROID,
	MEDIUM_ASTEROID,
	SMALL_ASTEROID
};

class Asteroid {
public:
	Asteroid(Pos t_pos, asteroid_type t_asteroid_type);
	~Asteroid();

	void move(double t_time_step);
	void setPos(Pos t_pos, double t_rot_degrees);
	void destroy(void);
	void render(void);
	void free(void);
	Pos getPos();
	double getX();
	double getY();

private:
	Pos m_pos;
	double m_rot_degrees{ 0 },m_dir_degrees, m_vel;
	Texture* m_texture;
};

#endif // !_ASTEROID_H_
