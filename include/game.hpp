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

//Forward declaring systems
class GraphicsSettingsMessage;
class RenderSystem;
class AsteroidSystem;
class SystemManager;

using CellMap = std::unordered_map<std::pair<int, int>, std::vector<uint32_t>, pair_hash>;

class Game {
public:

	Game();
	~Game();

	bool initialize(const char* t_title, int t_x, int t_y);
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
	std::unique_ptr<SystemManager> systemManager;
	RenderSystem* renderSystem;
	AsteroidSystem* asteroidSystem;
	bool shouldUpdateSettings{ false }, pendingFullScreenChange{ false };
	std::shared_ptr<GraphicsSettingsMessage> graphicsSettings;
	Camera camera;
	Uint32 last_tick = 0, tick = 0;
	SDL_Event e; //event to catch keypresses

	void createShip(ShipType shipType);
	void handleGraphicsSettingsMessage(std::shared_ptr<GraphicsSettingsMessage> msg);
	void updateGraphicsSettings();
	void updateFullScreen();

	CellMap spatialGrid;
};
