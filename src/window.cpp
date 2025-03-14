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
	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, m_title);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, m_x);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, m_y);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, m_width);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, m_height);
	m_window = std::unique_ptr<SDL_Window, SDL_Window_Deleter>(SDL_CreateWindowWithProperties(props));
	if (!m_window) {
		std::cout << "Window could not be created! SDL_Error:" << SDL_GetError() << '\n';
	}
}

SDL_Window* Window::getWindow() {
	return &*m_window;
}
