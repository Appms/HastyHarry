#ifndef KEYSBEHAVIOUR_H
#define KEYSBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "SFML/Audio.hpp"

class Sound;

class SoundBehaviour : public AbstractBehaviour
{
	public:
		SoundBehaviour(std::string pSound, glm::vec3 pPosition,bool playInstant, bool relative);
		virtual ~SoundBehaviour();
		virtual void update( float step );
		virtual void trigger(GameObject* pPlayer);

    private:
		sf::Sound _sound;
		sf::SoundBuffer _soundBuffer;
		bool _triggered;
};	

#endif // KEYSBEHAVIOUR_H
