#pragma once
#include "RenderComponent.h"
#include <SDL3/SDL.h>

class AnimationComponent : public RenderComponent {
	AnimationComponent(SDL_Renderer* t_renderer, SDL_Texture* t_texture, int t_num_frames);

	void update(int d_t);
	void render();

private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	int m_num_frames;

	int m_frame_width = 32;
	int m_frame_height = 32;

	int m_frame_duration = 100;

	int m_current_frame = 0;
	int m_elapsed_time = 0;
};

