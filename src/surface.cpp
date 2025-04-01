#include "surface.hpp"

Surface g_ship_surface;
Surface g_shot_surface;
Surface g_particle_surface;
Surface g_particle_shimmer_surface;
Surface g_asteroid_big_surface;

bool Surface::loadFromFile(std::string t_path) {
    m_surface = std::shared_ptr<SDL_Surface>(IMG_Load(t_path.c_str()), SDL_DestroySurface);
    if (!m_surface) {
		std::cout << "Unable to load image " << t_path.c_str() << "! SDL ERROR: " << SDL_GetError() << '\n';
	} else {
        SDL_SetSurfaceColorKey(m_surface.get(), true, SDL_MapSurfaceRGBA(m_surface.get(), 0, 0xFF, 0xFF, 0));
    }
    return m_surface != nullptr;
}

bool Surface::loadFromText(std::string t_texture_text, SDL_Color t_text_color, TTF_Font* g_font) {
	m_surface = std::shared_ptr<SDL_Surface>(
        TTF_RenderText_Solid(g_font, t_texture_text.c_str(), 0, t_text_color),
        SDL_DestroySurface
    );
    if (!m_surface) {
        std::cout << "Unable to render text surface! SDL_ttf Error:" << SDL_GetError() << '\n';
    }
    return m_surface != nullptr;
}


