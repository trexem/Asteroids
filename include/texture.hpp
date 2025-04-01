#pragma once
#ifndef __TEXTURE_HPP_
#define __TEXTURE_HPP_

#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "surface.hpp"

struct SDL_Surface_Deleter {
	void operator()(SDL_Surface* surface) {
		SDL_DestroySurface(surface);
	}
};

class Texture {
public:
	Texture(void);
	Texture(SDL_Renderer* g_renderer);
	Texture(SDL_Renderer* renderer, Surface surface);
	~Texture(void);

	bool loadFromFile(std::string t_path);
	bool loadFromRenderedText(std::string t_texture_text, SDL_Color t_text_color, TTF_Font* g_font);
	void free(void);
	void render(int t_x, int t_y);
	void renderEx(int t_x, int t_y, SDL_FRect* t_clip, double t_angle, SDL_FPoint* t_center, SDL_FlipMode t_flip);
	void setAlphaMod(int alpha);

	int getWidth();
	int getHeight();

	SDL_Renderer* m_renderer{nullptr};
private:
	SDL_Texture* m_texture{nullptr};
	int m_width{0};
	int m_height{0};
};

extern Texture g_ship_texture;
extern Texture g_shot_texture;
extern Texture g_particle_texture;
extern Texture g_particle_shimmer_texture;
extern Texture g_asteroid_big_texture;


#endif
