#include "mge/core/Timer.hpp"
#include <iostream>

sf::Clock Timer::_clock;
float Timer::_currentTime = 0.0f;
float Timer::_deltaTime = 0.0f;
float Timer::_slowmotionFactor = 0.5f;

bool Timer::Slowmotion = false;
bool Timer::_paused = false;

void Timer::update()
{
	float newTime = _clock.getElapsedTime().asSeconds();
	_deltaTime = newTime - _currentTime;
	_currentTime = newTime;
}

float Timer::now()
{
	return _currentTime;
}

void Timer::Pause()
{
	//TODO Add Pausing Audio
	_paused = true;
}

void Timer::UnPause()
{
	//TODO ADD unpausing audio
	_paused = false;
}

void Timer::TogglePause()
{
	if (_paused) {
		UnPause();
	} else {
		Pause();
	}
}

bool Timer::IsPaused()
{
	return _paused;
}

float Timer::deltaTime()
{
	//HACK This should fix some weird stuff
	if (_deltaTime > 3.0f)
		return 0.0f;
	
	return _paused ? 0.0f : (Slowmotion ? _deltaTime * _slowmotionFactor : _deltaTime);
}
