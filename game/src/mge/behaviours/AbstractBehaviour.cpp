#include <cassert>
#include <iostream>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"

AbstractBehaviour::AbstractBehaviour():_owner(NULL) {}

AbstractBehaviour::~AbstractBehaviour()
{
	_owner = NULL;
}

void AbstractBehaviour::setOwner (GameObject* pOwner) {
    _owner = pOwner;
}

