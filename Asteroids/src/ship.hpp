#pragma once
#ifndef _SHIP_H_
#define _SHIP_H_

#include "gameObject.hpp"
#include "shot.hpp"
#include "particle.hpp"

const int SHIP_MAX_SHOTS = 10;



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

	Shot* m_shots[SHIP_MAX_SHOTS];//pointer array of shots
private:
	    //Calls to render all shots that are not dead
	void renderShots();
	void generateParticles(bool b);
	void checkShots(void);
	Particle* particles[TOTAL_PARTICLES];
	void renderParticles();
	
	double m_vel{0}, m_rot_vel{0};
	Texture *m_shot_texture;
	double m_last_shot;
	bool is_moving;
};
#endif
