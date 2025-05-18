#include "texture.hpp"

Texture g_ship_texture;
Texture g_shot_texture;
Texture g_rocket_texture;
Texture g_particle_texture;
Texture g_particle_shimmer_texture;
Texture g_asteroid_big_texture;

Texture::Texture() {
	m_renderer = nullptr;
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
}

Texture::Texture(SDL_Renderer* g_renderer) {
	m_renderer = g_renderer;
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
}

Texture::Texture(SDL_Renderer* renderer, Surface surface) {
	m_renderer = renderer;
	m_texture = SDL_CreateTextureFromSurface(m_renderer, surface.getSurface());
	// std::cout << "[LOAD] Created texture @ " << (void*)m_texture << "\n";
	m_width = surface.getWidth();
	m_height = surface.getHeight();
}

Texture::~Texture() {
	free();
}

bool Texture::loadFromFile(std::string t_path) {
	free();
	SDL_Texture* new_texture = nullptr;
	auto loaded_surface = std::unique_ptr<SDL_Surface, SDL_Surface_Deleter>(IMG_Load(t_path.c_str()));
	if (!loaded_surface) {
		std::cout << "Unable to load image " << t_path.c_str() << "! SDL ERROR: " << SDL_GetError() << '\n';
	} else {
		loadFromSurface(loaded_surface.get());
	}
	// std::cout << "[LOAD] Created texture @ " << (void*)m_texture << "\n";
	m_texture = new_texture;
	return m_texture != nullptr;
}

bool Texture::loadFromText(std::string t_texture_text, SDL_Color t_text_color, TTF_Font* g_font) {
	free();
	auto text_surface = std::unique_ptr<SDL_Surface, SDL_Surface_Deleter>(TTF_RenderText_Solid(g_font, t_texture_text.c_str(), 0, t_text_color));
	if (text_surface) {
		m_texture = SDL_CreateTextureFromSurface(m_renderer, &*text_surface);
		if (m_texture == NULL) {
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << '\n';
		} else {
			m_width = static_cast<float>(text_surface->w);
			m_height = static_cast<float>(text_surface->h);
		}
	} else {
		std::cout << "Unable to render text surface! SDL_ttf Error:" << SDL_GetError() << '\n';
	}
	// std::cout << "[LOAD] Created texture @ " << (void*)m_texture << "\n";
	return m_texture != NULL;
}

void Texture::free() {
	if (m_texture != nullptr) {
		// std::cout << "[FREE] Freeing texture @ " << (void*)m_texture << "\n";
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}

void Texture::render(int t_x, int t_y, float t_scale) {
	SDL_FRect render_quad = {
        static_cast<float>(t_x),
        static_cast<float>(t_y),
        m_width * t_scale,
        m_height * t_scale
    };
	SDL_RenderTexture(m_renderer, m_texture, NULL, &render_quad);
}

void Texture::renderEx(int t_x, int t_y, SDL_FRect* t_clip, double t_angle
	, SDL_FPoint* t_center, SDL_FlipMode t_flip, float t_scale, const SDL_Color* color) {
	if (color) {
        SDL_SetTextureColorMod(m_texture, color->r, color->g, color->b);
    }
	SDL_FRect render_quad = {
        static_cast<float>(t_x),
        static_cast<float>(t_y),
        m_width * t_scale,
        m_height * t_scale
    };
	SDL_RenderTextureRotated(m_renderer, m_texture, t_clip, &render_quad, t_angle, t_center, t_flip);
	if (color) {
        SDL_SetTextureColorMod(m_texture, 255, 255, 255);
    }
}

void Texture::renderEx(int t_x, int t_y, SDL_FRect* t_clip, double t_angle
	, SDL_FPoint* t_center, SDL_FlipMode t_flip, FPair size, const SDL_Color* color) {
	if (color) {
        SDL_SetTextureColorMod(m_texture, color->r, color->g, color->b);
    }
	SDL_FRect render_quad = {
        static_cast<float>(t_x),
        static_cast<float>(t_y),
        size.x,
        size.y
    };
	SDL_RenderTextureRotated(m_renderer, m_texture, t_clip, &render_quad, t_angle, t_center, t_flip);
	if (color) {
        SDL_SetTextureColorMod(m_texture, 255, 255, 255);
    }
}

float Texture::getWidth() {
	return m_width;
}

float Texture::getHeight() {
	return m_height;
}

void Texture::setAlphaMod(int alpha) {
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

void Texture::createEmptyTexture(int w, int h) {
	free();
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	m_width = static_cast<float>(w);
	m_height = static_cast<float>(h);
	if (!m_texture) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
    }
}

SDL_Texture* Texture::getTexture() {
	return m_texture;
}

void Texture::colorMod(const SDL_Color& color) {
	SDL_SetTextureColorMod(m_texture, color.r, color.g, color.b);
}

bool Texture::loadFromSurface(SDL_Surface* surface) {
	SDL_SetSurfaceColorKey(surface, true, SDL_MapSurfaceRGBA(surface, 0, 0xFF, 0xFF, 0));
	m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	if (m_texture == NULL) {
		std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << '\n';
	} else {
		m_width = static_cast<float>(surface->w);
		m_height = static_cast<float>(surface->h);
	}
	return m_texture != nullptr;
}
