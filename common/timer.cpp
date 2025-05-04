#include "timer.h"

Timer::Timer()
{
	_seconds = 0;
	_paused = false;
}

Timer::~Timer()
{

}

void Timer::Update(float deltaTime)
{
	if (!_paused) {
		_seconds += deltaTime;
	}
}

void Timer::StartTimer() {
	_paused = false;
}

void Timer::PauseTimer() {
	if (_paused) {
		_paused = false;
		return;
	}
	_paused = true;
}

void Timer::StopTimer() {
	_paused = true;
	_seconds = 0;
}

void Timer::StartOverTimer() {
	_paused = true;
	_seconds = 0;
	_paused = false;
}