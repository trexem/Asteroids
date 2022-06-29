#pragma once
#ifndef SHOT_H_
#define SHOT_H_

#include "gameObject.hpp"

class Shot : public GameObject {
public:
	//Speed for the shot, must be faster than the max speed of spaceship
	const int SHOT_SPEED = 1000;
	//Constructor, we have to give initial position, rotation and give a texture
	Shot(int t_x, int t_y, double t_degrees, Texture* t_texture);

	//Move function, needs the time that has passed since last movement
	void move(double t_time_step);

	//Renders if it is not dead
	void render(void);

	void kill(void);

	//Returns true if shot is outside of the window + 15 pixels to be able to see the tail
	bool isDead(void);

private:
	bool m_is_dead;
};
#endif // !SHOT_H_

