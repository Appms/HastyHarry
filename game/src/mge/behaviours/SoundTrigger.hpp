#ifndef SOUNDTRIGGER_H
#define SOUNDTRIGGER_H

#include <glm.hpp>
#include "mge/behaviours/TriggerBehaviour.hpp"
#include "SFML/Audio.hpp"
#include "mge/core/SoundEngine.hpp"

class GameObject;

class SoundTrigger : public TriggerBehaviour
{
	public:
		SoundTrigger(GameObject* pActivator, float pRadius, std::string pAudioName, SoundType pSoundType);
		virtual ~SoundTrigger();

	protected:
		void trigger();
};	

#endif // TRIGGERBEHAVIOUR_H
