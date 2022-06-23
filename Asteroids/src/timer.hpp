#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_
#include <SDL.h>

class Timer {
public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();

	bool isStarted();
	bool isPaused();

private:
	Uint32 m_start_ticks {0};
	Uint32 m_paused_ticks{0};
	bool m_paused{false};
	bool m_started{false};
};

#endif
