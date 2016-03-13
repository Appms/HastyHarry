#include <glm.hpp>

#include "mge/behaviours/TriggerBehaviour.hpp"
#include "mge/core/GameObject.hpp"

TriggerBehaviour::TriggerBehaviour()
{
	_triggered = false;
}

TriggerBehaviour::~TriggerBehaviour() {}

void TriggerBehaviour::update( float pStep )
{
	if (!_triggered && glm::length2(_owner->getWorldPosition() - _activator->getWorldPosition()) < _radius * _radius)
	{
		_triggered = true;
		trigger();
	}
}
