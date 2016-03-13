#include "mge/behaviours/CollectTrigger.hpp"
#include "mge/core/Level.hpp"
#include "mge/core/SoundEngine.hpp"
#include "mge/core/GameObject.hpp"

#define TRIGGER_RADIUS 2.0f

CollectTrigger::CollectTrigger() : TriggerBehaviour()
{
	_radius = TRIGGER_RADIUS;
	_activator = Level::CurrentPlayer;
}

CollectTrigger::~CollectTrigger()
{
	_activator = NULL;
	delete _owner;
}

void CollectTrigger::trigger()
{
	SoundEngine::PlayAudio("collect", (GameObject*)Level::CurrentWorld, _owner->getWorldPosition());
	delete _owner;
}


