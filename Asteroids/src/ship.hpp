#pragma once
#ifndef _SHIP_H_
#define _SHIP_H_

#include "gameObject.hpp"
#include "particle.hpp"

const int SHIP_MAX_SHOTS = 10;

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

	    //Returns true if shot is outside of the window + 15 pixels to be able to see the tail
	bool isDead(void);

};

class Ship : public GameObject {
public:
	    //TOP speed of ship, must be lower than the speed of shot
	const int SHIP_TOP_SPEED = 500;
	    //Min speed in reverse (lower than forward speed)
	const int SHIP_MIN_SPEED = -200;
	const int SHIP_TOP_ROTATION_SPEED = 540; //1.5 turn per second
	const int SHIP_SPEED = 15; //acceleration, instead of speed
	const int SHIP_ROT_SPEED = 10; //acceleration, instead of speed
	const float SHIP_SHOT_DELAY = .25; //How fast will the ship shoot?
	    //Constructor, only needs a pointer to  set the renderer
	Ship();

	Ship(SDL_Renderer * t_renderer);
	    //Desctructor, we also delete all shots
	~Ship(void);
	    //Handle inputs that control the spaceship
	void handleInput(const Uint8* t_current_key_states);
	    //Move function, needs the time that has passed since last movement
	void move(double t_time_step);
//Set position function, x, y and rotation
	void setPos(double t_x, double t_y, double t_rot_degrees);
	//Pause particles
	void pause(void);
	//Resume particles
	void resume(void);
	    //restart spaceship to the center, also restarts shots
	void restart(void);
	    //Render function in
	void render(void);
	    //Shoot a singleshot from the tip of the spaceship
	void shoot(void);
	    //free textures
	void free(void);
private:
	    //Calls to render all shots that are not dead
	void renderShots();
	void generateParticles(bool b);
	Particle* particles[TOTAL_PARTICLES];
	void renderParticles();
	Shot* m_shots[SHIP_MAX_SHOTS];//pointer array of shots
	double m_vel{0}, m_rot_vel{0};
	Texture *m_shot_texture;
	SDL_Renderer* m_renderer;
	double m_last_shot;
	bool is_moving;
};
#endif
