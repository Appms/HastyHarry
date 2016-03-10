#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <glm.hpp>
#include "mge/behaviours/TriggerBehaviour.hpp"
#include "SFML/Audio.hpp"
#include "mge/core/SoundEngine.hpp"

class GameObject;

class Projectile : public TriggerBehaviour
{
	public:
		Projectile(float pRadius, std::string pAudioName, SoundType pSoundType, GameObject* pParent);
		Projectile(std::string params);
		virtual ~Projectile();

	protected:
		void trigger();
		std::string _audioName;
		SoundType _soundType;
		GameObject* _parent;
};	

#endif // SOUNDTRIGGER_H
