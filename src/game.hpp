#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <time.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const bool IS_FPS_VISIBLE = false;
const uint32_t MAX_ENTITIES = 1000;

#include "renderer.hpp"
#include "window.hpp"
#include "timer.hpp"
#include "EntityManager.h"
#include "ship.hpp"
#include "asteroid.hpp"
#include "UISystem.h"
#include "PlayerSystem.h"
#include "KeyboardSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"


class Game {
public:
	const int SHIP_TOP_SPEED = 500; //TOP speed of ship, must be lower than the speed of shot
	const int SHIP_MIN_SPEED = -200; //Min speed in reverse (lower than forward speed)
	const int SHIP_TOP_ROTATION_SPEED = 540; //1.5 turn per second
	const int SHIP_SPEED = 15; //acceleration, instead of speed
	const int SHIP_ROT_SPEED = 10; //acceleration, instead of speed
	const float SHIP_SHOT_DELAY = .25; //How fast will the ship shoot?

	Game();
	~Game();

	bool initialize(const char* t_title, int t_x, int t_y, int t_width, int t_height, Uint32 flags);
	bool loadMedia(void);
	void start(void);
	void restart(void);
	void gameLoop(void);
	void generateAsteroids(void);
	FPair generateSingleAsteroidPos(void);
	Renderer getRenderer(void);
	Window getWindow(void);
	void checkCollisions(void);
	void deleteDeadAsteroids(void);
	void deleteAsteroids(void);
	void scoreUp(void);
	void restartScore(void);

	Timer fps_timer, cap_timer, step_timer;
	std::stringstream time_text, pause_text, score_text; //strings to print: fps, PAUSE and score
	bool quit = false, pause = false;
	double time_step;
	SDL_Color white_color = {255, 255, 255, 255};
	int counted_frames = 0;
	EntityManager entityManager;  // Only one instance of EntityManager

private:
	Ship* m_ship;
	std::unique_ptr<KeyboardSystem> keySystem;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<UISystem> uiSystem;
	std::unique_ptr<PlayerSystem> playerSystem;
	std::unique_ptr<PhysicsSystem> physicsSystem;
	std::unique_ptr<MovementSystem> movementSystem;
	std::unique_ptr<CollisionSystem> collisionSystem;
	Window m_window;
	Texture m_fps_text_texture, m_pause_text_texture, m_score_text_texture;
	Texture g_particle_shimmer_texture;
	TTF_Font* m_fps_ttf;
	TTF_Font* m_pause_ttf;
	TTF_Font* m_score_ttf;
	Uint32 last_tick = 0, tick = 0, m_score = 0;
	SDL_Event e; //event to catch keypresses

	void createShip(ShipType type);
};



#endif
