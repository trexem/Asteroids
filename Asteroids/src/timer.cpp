#include "timer.hpp"

Timer::Timer() {
	m_start_ticks = 0;
	m_paused_ticks = 0;
	m_paused = false;
	m_started = false;
}

void Timer::start() {
	m_started = true;
	m_paused = false;
	m_start_ticks = SDL_GetTicks();
	m_paused_ticks = 0;
}
void Timer::stop() {
	m_started = false;
	m_paused = false;
	m_start_ticks = 0;
	m_paused_ticks = 0;
}
void Timer::pause() {
	if (m_started && !m_paused) {
		m_paused = true;
		m_paused_ticks = SDL_GetTicks() - m_start_ticks;
		m_start_ticks = 0;
	}
}
void Timer::unpause() {
	if (m_started && m_paused) {
		m_paused = false;
		m_start_ticks = SDL_GetTicks() - m_paused_ticks;
		m_paused_ticks = 0;
	}
}

Uint32 Timer::getTicks() {
	if (m_started) {
		if (m_paused) {
			return 0;
		} else {
			return SDL_GetTicks() - m_start_ticks;
		}
	}
	return 0;
}

bool Timer::isStarted() {
	return m_started;
}
bool Timer::isPaused() {
	return m_paused;
}
