#pragma once
#ifndef __SURFACE_HPP_
#define __SURFACE_HPP_

#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

class Surface {
public:
    Surface() {}
    bool loadFromFile(std::string t_path);
    bool loadFromText(std::string t_texture_text, SDL_Color t_text_color, TTF_Font* g_font);

    SDL_Surface* getSurface() { return m_surface.get(); }
    float getWidth() { return static_cast<float>(m_surface->w); }
	float getHeight() { return static_cast<float>(m_surface->h); }

    SDL_Renderer* m_renderer{nullptr};

private:
    std::shared_ptr<SDL_Surface> m_surface{nullptr};
};

extern Surface g_ship_surface;
extern Surface g_shot_surface;
extern Surface g_particle_surface;
extern Surface g_particle_shimmer_surface;
extern Surface g_asteroid_big_surface;

#endif // !__SURFACE_HPP_