#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <time.h>

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const bool IS_FPS_VISIBLE = true;
const uint32_t MAX_ENTITIES = 5000;

#include "renderer.hpp"
#include "window.hpp"
#include "timer.hpp"
#include "Camera.h"
#include "Colors.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "GameStateManager.h"
#include "GUISystem.h"
#include "PlayerSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "AbilitySystem.h"
#include "DamageSystem.h"
#include "AnimationSystem.h"
#include "AsteroidSystem.h"
#include "ExperienceSystem.h"
#include "LifeTimeSystem.h"
#include "OrbitSystem.h"


class Game {
public:
	const int SHIP_TOP_SPEED = 500; //TOP speed of ship, must be lower than the speed of shot
	const int SHIP_MIN_SPEED = -200; //Min speed in reverse (lower than forward speed)
	const int SHIP_TOP_ROTATION_SPEED = 540; //1.5 turn per second
	const int SHIP_SPEED = 15; //acceleration, instead of speed
	const int SHIP_ROT_SPEED = 10; //acceleration, instead of speed
	const float SHIP_SHOT_DELAY = .25; //How fast will the ship shoot?
	const float SHIP_BASE_RADIUS = 50;

	Game();
	~Game();

	bool initialize(const char* t_title, int t_x, int t_y, int t_width, int t_height, Uint32 flags);
	bool loadMedia(void);
	void start(void);
	void restart(void);
	void gameLoop(void);

	Timer fps_timer, cap_timer, step_timer;
	std::stringstream time_text, pause_text, score_text; //strings to print: fps, PAUSE and score
	bool quit = false, pause = false;
	double timeStep;
	int counted_frames = 0;
	EntityManager entityManager;  // Only one instance of EntityManager

private:
	Texture g_particle_shimmer_texture;
	Window m_window;
	std::unique_ptr<InputSystem> inputSystem;
	std::unique_ptr<GUISystem> guiSystem;
	std::unique_ptr<PlayerSystem> playerSystem;
	std::unique_ptr<PhysicsSystem> physicsSystem;
	std::unique_ptr<MovementSystem> movementSystem;
	std::unique_ptr<CollisionSystem> collisionSystem;
	std::unique_ptr<AbilitySystem> abilitySystem;
	std::unique_ptr<DamageSystem> damageSystem;
	std::unique_ptr<AnimationSystem> animationSystem;
	std::unique_ptr<AsteroidSystem> asteroidSystem;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<ExperienceSystem> xpSystem;
	std::unique_ptr<LifeTimeSystem> lifeTimeSystem;
	std::unique_ptr<OrbitSystem> orbitSystem;
	Camera camera;
	Uint32 last_tick = 0, tick = 0;
	SDL_Event e; //event to catch keypresses

	void createShip(ShipType shipType);
};
