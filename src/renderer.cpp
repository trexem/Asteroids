#include "renderer.hpp"

Renderer::Renderer(SDL_Window* window, const char * name) {
	m_renderer = std::unique_ptr<SDL_Renderer, SDL_Renderer_Deleter>(SDL_CreateRenderer(window, name));
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

void Renderer::drawDebugRect(const SDL_FRect& rect, SDL_Color color) {
    SDL_SetRenderDrawColor(&*m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderRect(&*m_renderer, &rect);
    // Reset to default color if needed
    SDL_SetRenderDrawColor(&*m_renderer, 0, 0, 0, 255);
}

void Renderer::drawDebugLine(float x1, float y1, float x2, float y2, SDL_Color color) {
	SDL_SetRenderDrawColor(&*m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(&*m_renderer, x1, y1, x2, y2);
	SDL_SetRenderDrawColor(&*m_renderer, 0, 0, 0, 255);
}

void Renderer::drawDebugCircle(float centerX, float centerY, float radius, SDL_Color color, int segments) {
	SDL_SetRenderDrawColor(&*m_renderer, color.r, color.g, color.b, color.a);
	float theta = TAU / float(segments);
    float c = cosf(theta); // pre-calculate the sine and cosine
    float s = sinf(theta);
    float x = radius; // start at angle = 0
    float y = 0;

    float startX = centerX + x;
    float startY = centerY + y;
    float prevX = startX;
    float prevY = startY;

    for (int i = 1; i <= segments; i++) {
        // apply the rotation matrix
        float t = x;
        x = c * x - s * y;
        y = s * t + c * y;

        float newX = centerX + x;
        float newY = centerY + y;

        SDL_RenderLine(&*m_renderer, prevX, prevY, newX, newY);

        prevX = newX;
        prevY = newY;
    }

    // Close the circle
    SDL_RenderLine(&*m_renderer, prevX, prevY, startX, startY);
	SDL_SetRenderDrawColor(&*m_renderer, 0, 0, 0, 255);
}
