#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <string>
#include <sstream>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

#include "renderer.hpp"
#include "window.hpp"
#include "timer.hpp"
#include "ship.hpp"
#include "asteroid.hpp"

class Game {
public:
	Game();
	~Game();

	bool initialize(const char* t_title, int t_x, int t_y, int t_width, int t_height, Uint32 flags);
	bool loadMedia(void);
	void start(void);
	void gameLoop(void);
	void generateAsteroids(void);
	Renderer getRenderer(void);
	Window getWindow(void);

	Timer fps_timer, cap_timer, step_timer;
	std::stringstream time_text, pause_text; //strings to print: fps and PAUSE
	bool quit = false, pause = false;
	double time_step;
	SDL_Color white_color = {255, 255, 255, 255};
	int counted_frames = 0;

private:
	Ship* m_ship;
	Asteroid* m_asteroids[TOTAL_ASTEROIDS];
	Window m_window;
	Renderer* m_renderer;
	Texture m_fps_text_texture, m_pause_text_texture;
	Texture g_particle_shimmer_texture;
	TTF_Font* m_fps_ttf;
	TTF_Font* m_pause_ttf;
	Uint32 last_tick = 0, tick = 0;
	SDL_Event e; //event to catch keypresses
};



#endif
