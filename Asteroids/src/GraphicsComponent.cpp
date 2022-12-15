#include "GraphicsComponent.h"
#include "PositionComponent.h"

GraphicsComponent::GraphicsComponent(SDL_Renderer* t_renderer, SDL_Texture* t_texture,
	int t_width, int t_height) : 
	m_renderer(t_renderer),
	m_texture(t_texture),
	m_width(t_width),
	m_height(t_height) {}

void GraphicsComponent::update() {}

void GraphicsComponent::render() {
	auto position = entity()->getComponents<PositionComponent>()[0];

	SDL_Rect rect{ position->x(), position->y(), m_width, m_height };

	SDL_RenderCopy(m_renderer, m_texture, nullptr, &rect);
}
