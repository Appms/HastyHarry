#include "mge/behaviours/SoundTrigger.hpp"
#include "mge/core/SoundEngine.hpp"
#include "mge/core/GameObject.hpp"
#include <sfml/window/event.hpp>
#include <string>

SoundTrigger::SoundTrigger(GameObject* pActivator, float pRadius, std::string pSoundName, SoundType pSoundType) : TriggerBehaviour(pActivator, pRadius)
{
}

SoundTrigger::~SoundTrigger()
{
}

void SoundTrigger::trigger()
{
	
}


