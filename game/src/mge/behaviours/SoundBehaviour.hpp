#ifndef KEYSBEHAVIOUR_H
#define KEYSBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "SFML/Audio.hpp"

class Sound;

class SoundBehaviour : public AbstractBehaviour
{
	public:
		SoundBehaviour();
		virtual ~SoundBehaviour();
		virtual void update( float step );

		void PlayAudio(sf::SoundBuffer &pBuffer, float pMinDistance = 5.0f, float pAttenuation = 10.0f);
		void StopAudio();
		float GetCreationTime();
		bool IsFinished();


    private:
		sf::Sound _sound;
		float _creationTime;
};	

#endif // KEYSBEHAVIOUR_H
