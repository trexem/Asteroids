#include "AnimationComponent.h"
#include "PositionComponent.h"

AnimationComponent::AnimationComponent(SDL_Renderer* t_renderer, SDL_Texture* t_texture, int t_num_frames) :
	m_renderer(t_renderer),
	m_texture(t_texture),
	m_num_frames(t_num_frames) {}

void AnimationComponent::update(int d_t) {
	m_elapsed_time += d_t;
	if (m_elapsed_time >= m_frame_duration) {
		m_current_frame = (m_current_frame + 1) % m_num_frames;
		m_elapsed_time = 0;
	}
}

void AnimationComponent::render() {

	auto position = entity()->getComponents<PositionComponent>()[0];

	SDL_Rect src_rect{ m_current_frame * m_frame_width, 0, m_frame_width, m_frame_height };
	SDL_Rect dest_rect{ position->x(), position->y(), m_frame_width, m_frame_height };
	
	SDL_RenderCopy(m_renderer, m_texture, &src_rect, &dest_rect);
}
