#ifndef TIME_H
#define TIME_H

#include <SFML/System.hpp>

//Wrapper around sf::Clock to provide the elapsed time overall, and elapsed time since the last frame update

class Timer
{
	private: // data members
		static sf::Clock _clock;
		static float _currentTime;
		static float _deltaTime;

	private: // disable creation of time object
		Timer(){}

	public: // functons
		static void update();
		static float now();
		static float deltaTime();

};

#endif // TIME_H
