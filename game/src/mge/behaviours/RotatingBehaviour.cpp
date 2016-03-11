#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/core/GameObject.hpp"

RotatingBehaviour::RotatingBehaviour(float anglePerSec) :AbstractBehaviour()
{
	_anglePerSec = anglePerSec;
}

RotatingBehaviour::RotatingBehaviour(std::string params) : AbstractBehaviour()
{
	_anglePerSec = atof(params.c_str());
}

RotatingBehaviour::~RotatingBehaviour()
{
	//dtor
}

void RotatingBehaviour::update(float step)
{
	_owner->rotate(step * glm::radians(_anglePerSec), glm::vec3(1.0f, 1.0f, 0.0f)); // rotates 45° per second
}
