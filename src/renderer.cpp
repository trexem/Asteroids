#include "renderer.hpp"

Renderer::Renderer(SDL_Window* window, int index, Uint32 flags) {
	m_renderer = std::unique_ptr<SDL_Renderer, SDL_Renderer_Deleter>(SDL_CreateRenderer(window, index, flags));
	if (!m_renderer) {
		    //if error whielcreating renderer, print error
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
	}
}

void Renderer::changeColor(int red, int green, int blue, int alpha) {
	SDL_SetRenderDrawColor(&*m_renderer, red, blue, green, alpha);
}

SDL_Renderer* Renderer::getRenderer() {
	return &*m_renderer;
}
void Renderer::clear() {
	SDL_RenderClear(&*m_renderer);
}
void Renderer::render() {
	SDL_RenderPresent(&*m_renderer);
}
