#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <iostream>
#include <memory>
#include <SDL.h>

struct SDL_Renderer_Deleter {
	void operator()(SDL_Renderer* renderer) {
		SDL_DestroyRenderer(renderer);
	}
};

class Renderer {
public:
	Renderer();
	Renderer(SDL_Window* window, int index, Uint32 flags);
	void changeColor(int r, int g, int b, int alpha);
	SDL_Renderer* getRenderer(void);
	void clear(void);
	void render(void);
private:
	std::unique_ptr<SDL_Renderer, SDL_Renderer_Deleter> m_renderer;
};

#endif
