#include "timer.h"

Timer::Timer()
{
	start_time = 0;
	stop_time = 0;
	is_running = false;
	is_stopped = false;
}

void Timer::start()
{
	start_time = SDL_GetTicks();
	is_running = true;
}

void Timer::reset(float t)
{
	start_time = 0;
	stop_time = 0;
	is_running = false;
	is_stopped = false;
}

void Timer::stop()
{
	is_running = false;
	is_stopped = true;
	start_time = 0;
}

float Timer::get_time()
{
	return SDL_GetTicks() - start_time;
}

bool Timer::get_state()
{
	return is_running;
}
