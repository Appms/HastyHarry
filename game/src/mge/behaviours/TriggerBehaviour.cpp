#include "mge/behaviours/TriggerBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"
#include "mge/core/Timer.hpp"
#include <sfml/window/event.hpp>
#include <iostream>
#include <string>

TriggerBehaviour::TriggerBehaviour(GameObject* pActivator, float pRadius)
{
	_triggered = false;
	_activator = pActivator;
	_radius = pRadius;
}

TriggerBehaviour::~TriggerBehaviour()
{
}

void TriggerBehaviour::update( float pStep )
{
	if (!_triggered && glm::length2(_owner->getWorldPosition() - _activator->getWorldPosition()) < _radius * _radius)
	{
		_triggered = true;
		trigger();
	}
}
