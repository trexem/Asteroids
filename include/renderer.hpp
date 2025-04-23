#pragma once

#include <iostream>
#include <memory>
#include <SDL3/SDL.h>

struct SDL_Renderer_Deleter {
	void operator()(SDL_Renderer* renderer) {
		std::cout << "Destroying renderer\n";
		SDL_DestroyRenderer(renderer);
	}
};

class Renderer {
public:
	Renderer();
	Renderer(SDL_Window* window, const char * name);
	void changeColor(int r, int g, int b, int alpha);
	SDL_Renderer* getRenderer(void);
	void clear(void);
	void render(void);
	void drawDebugRect(const SDL_FRect& rect, SDL_Color color);
	void drawDebugLine(float x1, float y1, float x2, float y2, SDL_Color color);
private:
	std::unique_ptr<SDL_Renderer, SDL_Renderer_Deleter> m_renderer;
};
