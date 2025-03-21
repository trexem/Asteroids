#include "ship.hpp"
#include <unordered_map>
#include <cmath>
#include <functional>

Ship::Ship() {
	is_moving = 0;
	m_last_shot = 0;
	m_pos.x = 0;
	m_pos.y = 0;
	m_rot_degrees = 0;
	m_vel = 0;
	m_rot_vel = 0;
	//m_texture = &Texture(m_renderer);
	//m_shot_texture = &Texture(m_renderer);
	m_texture = &g_ship_texture; 
	m_shot_texture = &g_shot_texture;
	m_collider.h = m_texture->getHeight();
	m_collider.w = m_texture->getWidth();
	    //initialize shots outside of the scree, so "dead" shots
	for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
		m_shots[i] = new Shot(-20, -20, 0, m_shot_texture);
	}
	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		double radians = m_rot_degrees * PI / 180;
		FPair p;
		p.x = (int)(m_pos.x + m_texture->getWidth() / 2 + sin(radians) * m_texture->getHeight() / 2);
		p.y = (int)(m_pos.y + m_texture->getHeight() - cos(radians) * m_texture->getHeight() / 2);
		particles[i] = new Particle(p);
		particles[i]->kill();
	}
}

Ship::~Ship() {
	for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
		delete m_shots[i];
	}
	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		delete particles[i];
	}
}

void Ship::handleInput(const bool* t_current_key_states) {
	    //There are so many ifs because it moved weirdly with separate ifs for each key
	    //So i have to add the cases for more than one key pressed at the same time
		using Action = std::function<void()>;
		std::unordered_map<SDL_Scancode, Action> keyActions = {
			{SDL_SCANCODE_UP, [&]() { m_vel += SHIP_SPEED; }},
			{SDL_SCANCODE_W, [&]() { m_vel += SHIP_SPEED; }},
			{SDL_SCANCODE_DOWN, [&]() { m_vel -= SHIP_SPEED; }},
			{SDL_SCANCODE_S, [&]() { m_vel -= SHIP_SPEED; }},
			{SDL_SCANCODE_LEFT, [&]() { 
				if (m_rot_vel > 0) {
					m_rot_vel = 0;
				}
				m_rot_vel -= SHIP_ROT_SPEED; 
			}},
			{SDL_SCANCODE_A, [&]() { 
				if (m_rot_vel > 0) {
					m_rot_vel = 0;
				}
				m_rot_vel -= SHIP_ROT_SPEED; 
			}},
			{SDL_SCANCODE_RIGHT, [&]() { 
				if (m_rot_vel < 0) {
					m_rot_vel = 0;
				}
				m_rot_vel += SHIP_ROT_SPEED; 
			}},
			{SDL_SCANCODE_D, [&]() { 
				if (m_rot_vel < 0) {
					m_rot_vel = 0;
				}
				m_rot_vel += SHIP_ROT_SPEED; 
			}},
			{SDL_SCANCODE_SPACE, [&]() { if (m_last_shot > SHIP_SHOT_DELAY) { shoot(); m_last_shot = 0; }}},
		};
	
		for (const auto& [key, action] : keyActions) {
			if (t_current_key_states[key]) {
				action();
			}
		}
		if (!(t_current_key_states[SDL_SCANCODE_UP] || t_current_key_states[SDL_SCANCODE_DOWN] ||
			  t_current_key_states[SDL_SCANCODE_W] || t_current_key_states[SDL_SCANCODE_S])) {
			//If no UP or DOWN are pressed we desacelerate
			m_vel *= 0.95;
			if (std::abs(m_vel) < 1) { //if the number is too small we round down to 0
				m_vel = 0;
			}
		}
		if (!(t_current_key_states[SDL_SCANCODE_LEFT] || t_current_key_states[SDL_SCANCODE_RIGHT] ||
			  t_current_key_states[SDL_SCANCODE_A] || t_current_key_states[SDL_SCANCODE_D])) {
				//If no LEFT or RIGHT are pressed we desacelerate the turn
			m_rot_vel *= 0.95;
			if (std::abs(m_rot_vel) < 1) {     //if the number is too small we round down to 0
				m_rot_vel = 0;
			}
		}
	}

void Ship::move(double t_time_step) {
	m_last_shot += t_time_step;
	for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
		if (m_shots[i] != nullptr) {
			m_shots[i]->move(t_time_step);
		}
	}
	checkShots();
	    //convert degrees to radians
	double radians = m_rot_degrees * PI / 180;
	    //top max and min speed
	if (m_vel > SHIP_TOP_SPEED) {
		m_vel = SHIP_TOP_SPEED;
	} else if (m_vel < SHIP_MIN_SPEED) {
		m_vel = SHIP_MIN_SPEED;
	}
	    //top max and min rotation speed
	if (m_rot_vel > SHIP_TOP_ROTATION_SPEED) {
		m_rot_vel = SHIP_TOP_ROTATION_SPEED;
	} else if (m_rot_vel < -SHIP_TOP_ROTATION_SPEED) {
		m_rot_vel = -SHIP_TOP_ROTATION_SPEED;
	}
	    //move X
	m_pos.x += m_vel * t_time_step * sin(radians);
	if (m_pos.x > SCREEN_WIDTH - m_texture->getWidth()) {
		m_pos.x = SCREEN_WIDTH - m_texture->getWidth();
		m_vel = 0;
		m_rot_vel = 0;
	} else if (m_pos.x < 0) {
		m_pos.x = 0;
		m_vel = 0;
		m_rot_vel = 0;
	}
	    //move Y, -= because Y axis is inverted
	m_pos.y -= m_vel * t_time_step * cos(radians);
	if (m_pos.y > SCREEN_HEIGHT - m_texture->getHeight()) {
		m_pos.y = SCREEN_HEIGHT - m_texture->getHeight();
		m_vel = 0;
		m_rot_vel = 0;
	} else if (m_pos.y < 0) {
		m_pos.y = 0;
		m_vel = 0;
		m_rot_vel = 0;
	}
	//set collider (x,y) to m_pos
	m_collider.x = m_pos.x;
	m_collider.y = m_pos.y;
	    //move degrees
	m_rot_degrees += m_rot_vel * t_time_step;
	    //to avoid going to infinite degrees we stay between -180 and 180
	if (m_rot_degrees > 180) {
		m_rot_degrees -= 360;
	} else if (m_rot_degrees < -180) {
		m_rot_degrees += 360;
	}

	if (m_vel < 30) {
		is_moving = false;
	}
	else {
		is_moving = true;
	}
}

void Ship::setPos(double t_x, double t_y, double t_rot_degrees) {
	m_pos.x = t_x - m_texture->getWidth() / 2;
	m_pos.y = t_y - m_texture->getHeight() / 2;
	m_rot_degrees = t_rot_degrees;
}

void Ship::pause() {
	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		particles[i]->pause();
	}
}

void Ship::resume() {
	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		particles[i]->resume();
	}
}

void Ship::restart(void) {
	setPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	m_vel = 0;
	m_rot_vel = 0;
	for (int i = 0; i < SHIP_MAX_SHOTS; ++i) {
		if (m_shots[i] != nullptr) {
			m_shots[i]->setPos({ -20, -20 }, 0);
		}
	}
	for (int i = 0; i < TOTAL_PARTICLES; ++i) {
		particles[i]->kill();
	}
}

void Ship::render(void) {
	    //x, y, null to show the entire ship, degrees, null to take the center as the pivot to rotate, and no flipped image
	m_texture->renderEx((int)m_pos.x, (int)m_pos.y, nullptr, m_rot_degrees, nullptr, SDL_FLIP_NONE);
	renderParticles();
	renderShots();
}

void Ship::renderParticles() {
	for (int i = 0; i < TOTAL_PARTICLES; ++i) {
		if (particles[i]->isDead()) {
			if (is_moving) {
				double radians = m_rot_degrees * PI / 180;
				FPair p;
				p.x = (int)(m_pos.x + m_texture->getWidth() / 2 - sin(radians) * m_texture->getHeight() / 2);
				p.y = (int)(m_pos.y + m_texture->getHeight() / 2 + cos(radians) * m_texture->getHeight() / 2);
				particles[i]->setPos(p);
			}
		}
	}
	for (int i = 0; i < TOTAL_PARTICLES; ++i) {
		if (!particles[i]->isDead()) {
			particles[i]->render();
		}
	}
}
    //Shoot a singleshot from the tip of the spaceship
void Ship::shoot() {
	for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
 		if (m_shots[i] == nullptr) {
			double radians = m_rot_degrees * PI / 180;
			int x = (int) (m_pos.x + m_texture->getWidth() / 2 + sin(radians) * m_texture->getHeight() / 2);
			int y = (int) (m_pos.y + m_texture->getHeight() / 2 - cos(radians) * m_texture->getHeight() / 2);
			    //create new shot with the position of the tip off the spaceship
			m_shots[i] = new Shot(x, y, m_rot_degrees, m_shot_texture);
			i = SHIP_MAX_SHOTS; //could also be an "exit for" or "break"
		}
	}
}
void Ship::free(void) {
	m_texture->free();
	m_shot_texture->free();
}
    //get the texture of the ship
//Texture Ship::getTexture() {
//	return m_texture;
//}
//Calls to render all shots that are not dead
void Ship::renderShots() {
	for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
		if (m_shots[i] != nullptr) {
			m_shots[i]->render();
		}
	}
}

void Ship::checkShots(){
	for (int i = 0; i < SHIP_MAX_SHOTS; ++i) {
		if (m_shots[i] != nullptr) {
			if (m_shots[i]->isDead()) {
				m_shots[i] = nullptr;
			}
		}
	}
}
