#include "mge/core/Timer.hpp"

sf::Clock Timer::_clock;
float Timer::_currentTime = 0.0f;
float Timer::_deltaTime = 0.0f;

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

float Timer::deltaTime()
{
	return _deltaTime;
}
