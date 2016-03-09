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
		static float _slowmotionFactor;
		static bool _paused;

	private: // disable creation of time object
		Timer(){}

	public: // functons
		static void update();
		static float now();
		static float deltaTime();
		static bool Slowmotion;
		static void Pause();
		static void UnPause();
		static bool IsPaused();
		static void TogglePause();

};

#endif // TIME_H
