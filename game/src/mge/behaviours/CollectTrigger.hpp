#ifndef COLLECTTRIGGER_H
#define COLLECTTRIGGER_H

#include <glm.hpp>
#include "mge/behaviours/TriggerBehaviour.hpp"
#include "SFML/Audio.hpp"
#include "mge/core/SoundEngine.hpp"

class GameObject;

class CollectTrigger : public TriggerBehaviour
{
	public:
		CollectTrigger();
		virtual ~CollectTrigger();

	protected:
		void trigger();
};	

#endif // COLLECTTRIGGER_H
