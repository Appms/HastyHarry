#include "mge/behaviours/MovingBehaviour.hpp"
#include "mge/core/GameObject.hpp"

MovingBehaviour::MovingBehaviour(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop) :AbstractBehaviour()
{
	_originPosition = pOrigin;
	_endPosition = pEnd;
	_speed = pSpeed;
	_loop = pLoop;
}

MovingBehaviour::MovingBehaviour(std::string params) : AbstractBehaviour()
{
	std::cout << params << std::endl;
	//_anglePerSec = atof(params.c_str());
}

MovingBehaviour::~MovingBehaviour()
{
	//dtor
}

void MovingBehaviour::update(float step)
{
	if (glm::distance(_owner->getWorldPosition(), _originPosition) < glm::distance(_endPosition, _originPosition))
		_owner->translate(glm::normalize(_endPosition - _originPosition) * _speed);
	else {
		if (_loop) {
			glm::vec3 aux = _endPosition;
			_endPosition = _originPosition;
			_originPosition = aux;
		}
		else delete _owner;
	}
	//_owner->rotate(step * glm::radians(_anglePerSec), glm::vec3(1.0f, 1.0f, 0.0f)); // rotates 45° per second
}
