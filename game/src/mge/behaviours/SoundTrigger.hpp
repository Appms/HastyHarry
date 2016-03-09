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
		SoundTrigger(float pRadius, std::string pAudioName, SoundType pSoundType, GameObject* pParent);
		SoundTrigger(std::string params);
		virtual ~SoundTrigger();

	protected:
		void trigger();
		std::string _audioName;
		SoundType _soundType;
		GameObject* _parent;
};	

#endif // SOUNDTRIGGER_H
