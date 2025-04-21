#pragma once

#include <iostream>
#include <memory>
#include <SDL3/SDL.h>

struct SDL_Window_Deleter {
	void operator()(SDL_Window* window) {
		std::cout << "Destroying Window\n";
		SDL_DestroyWindow(window);
	}
};

class Window {
public:
	Window(void);
	Window(const char* t_title, int t_x, int t_y, int t_width, int t_height, Uint32 flags);
	SDL_Window* getWindow();
private:
	const char* m_title;
	int m_x{0}, m_y{0}, m_width{0}, m_height{0};
	std::unique_ptr<SDL_Window, SDL_Window_Deleter> m_window;
};
