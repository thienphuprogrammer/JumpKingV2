#include "ImpTimer.h"

ImpTimer ImpTimer_Create() {
	ImpTimer timer;

	timer.start_tick_ = 0;
	timer.paused_tick_ = 0;
	timer.is_paused_ = false;
	timer.is_started_ = false;

	timer.start = ImpTimer_start;
	timer.stop = ImpTimer_stop;
	timer.paused = ImpTimer_paused;
	timer.unpaused = ImpTimer_unpaused;

	timer.get_ticks = ImpTimer_get_ticks;

	timer.is_started = ImpTimer_is_started;
	timer.is_paused = ImpTimer_is_paused;

	return timer;
}

void ImpTimer_Destroy(ImpTimer* timer) {
	if (timer)
	{
		free(timer);
	}		
}

void ImpTimer_start(ImpTimer* timer) {
	timer->is_started_ = 1;
	timer->is_paused_ = 1;
	timer->start_tick_ = SDL_GetTicks();
}

void ImpTimer_stop(ImpTimer* timer) {
	timer->is_paused_ = 0;
	timer->is_started_ = 0;
}

void ImpTimer_paused(ImpTimer* timer) {
	if (timer->is_started_ && !timer->is_paused_) {
		timer->is_paused_ = 1;
		timer->paused_tick_ = SDL_GetTicks() - timer->start_tick_;
	}
}

void ImpTimer_unpaused(ImpTimer* timer) {
	if (timer->is_paused_) {
		timer->is_paused_ = 0;
		timer->start_tick_ = SDL_GetTicks() - timer->paused_tick_;
		timer->paused_tick_ = 0;
	}
}

int ImpTimer_get_ticks(ImpTimer* timer) {
	if (timer->is_started_) {
		if (timer->is_paused_) {
			return timer->paused_tick_;
		}
		else {
			return SDL_GetTicks() - timer->start_tick_;
		}
	}
	return 0;
}

bool ImpTimer_is_started(ImpTimer* timer) {
	return timer->is_started_;
}

bool ImpTimer_is_paused(ImpTimer* timer) {
	return timer->is_paused_;
}