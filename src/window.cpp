#include "window.hpp"

Window::Window() {
	m_title = nullptr;
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;
}

Window::Window(const char* t_title, int t_x, int t_y, int t_width, int t_height, Uint32 flags) {
	m_title = t_title;
	m_x = t_x;
	m_y = t_y;
	m_width = t_width;
	m_height = t_height;
	m_window = std::unique_ptr<SDL_Window, SDL_Window_Deleter>(SDL_CreateWindow(m_title, m_x, m_y, m_width, m_height, flags));
	if (!m_window) {
		std::cout << "Window could not be created! SDL_Error:" << SDL_GetError() << '\n';
	}
}

SDL_Window* Window::getWindow() {
	return &*m_window;
}
