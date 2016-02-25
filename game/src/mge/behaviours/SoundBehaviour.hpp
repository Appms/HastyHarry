#ifndef KEYSBEHAVIOUR_H
#define KEYSBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "SFML/Audio.hpp"

class Sound;

/**
 * KeysBehaviour allows you to move an object using the keyboard in its own local space.
 * Left right turns, forward back moves.
 */
class SoundBehaviour : public AbstractBehaviour
{
	public:
		SoundBehaviour(std::string pSound, glm::vec3 pPosition);
		virtual ~SoundBehaviour();
		virtual void update( float step );

    private:
		sf::Sound _sound;
		sf::SoundBuffer _soundBuffer;
		float _timer = 0.0f;
};

#endif // KEYSBEHAVIOUR_H
