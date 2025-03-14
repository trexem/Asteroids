#pragma once
#include "RenderComponent.h"
#include <SDL3/SDL.h>

class GraphicsComponent : public RenderComponent {
	GraphicsComponent(SDL_Renderer* t_renderer, SDL_Texture* t_texture,
		int t_width, int t_height);

	void update();
	void render();

private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	int m_width;
	int m_height;
};

