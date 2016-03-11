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

void MovingBehaviour::updateValues(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop) {
	_originPosition = pOrigin;
	_endPosition = pEnd;
	_speed = pSpeed;
	_loop = pLoop;
}

void MovingBehaviour::update(float step)
{
	if (glm::distance(_owner->getWorldPosition(), _originPosition) < glm::distance(_endPosition, _originPosition))
		//_owner->translate(glm::normalize(_endPosition - _originPosition) * _speed);
		_owner->setTransform(glm::mat4(_owner->getTransform()[0], _owner->getTransform()[1], _owner->getTransform()[2], _owner->getTransform()[3] + glm::vec4(glm::normalize(_endPosition - _originPosition) * _speed, 0.0f)));
	else {
		if (_loop) {
			glm::vec3 aux = _endPosition;
			_endPosition = _originPosition;
			_originPosition = aux;
		}
		else if(_owner->getName() != "Butterfly") delete _owner;
	}
	//_owner->rotate(step * glm::radians(_anglePerSec), glm::vec3(1.0f, 1.0f, 0.0f)); // rotates 45° per second
}
