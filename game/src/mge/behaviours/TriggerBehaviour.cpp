#include "mge/behaviours/TriggerBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"
#include "mge/core/Timer.hpp"
#include <sfml/window/event.hpp>
#include <iostream>
#include <string>

TriggerBehaviour::TriggerBehaviour()
{
	_triggered = false;
}

TriggerBehaviour::~TriggerBehaviour()
{
	_activator = NULL;
}

void TriggerBehaviour::update( float pStep )
{
	if (!_triggered && glm::length2(_owner->getWorldPosition() - _activator->getWorldPosition()) < _radius * _radius)
	{
		_triggered = true;
		trigger();
	}
}
