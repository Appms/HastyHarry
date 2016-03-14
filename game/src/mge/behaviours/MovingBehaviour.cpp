#include <vector>

#include "mge/behaviours/MovingBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/util/Utility.hpp"

MovingBehaviour::MovingBehaviour(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop) :AbstractBehaviour(),
_originPosition(pOrigin), _endPosition(pEnd), _speed(pSpeed), _loop(pLoop) {}


MovingBehaviour::MovingBehaviour(glm::vec3 pStart, std::string params) : AbstractBehaviour()
{
	std::vector<std::string> str = Utility::Split(params, ',');
	_originPosition = pStart;
	_endPosition = _originPosition + Utility::StrToVec(str[0], str[1], str[2]);
	_speed = atof(str[3].c_str());
	
	if (0 == strcmp(str[4].c_str(), "True"))
	{
		_loop = true;
	}
	else if (0 == strcmp(str[4].c_str(), "False"))
	{
		_loop = false;
	}
}

MovingBehaviour::~MovingBehaviour() {}

void MovingBehaviour::updateValues(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop) {
	_originPosition = pOrigin;
	_endPosition = pEnd;
	_speed = pSpeed;
	_loop = pLoop;
}

void MovingBehaviour::update(float step)
{
	if (glm::distance(_owner->getWorldPosition(), _originPosition) < glm::distance(_endPosition, _originPosition))
		_owner->setTransform(glm::mat4(
			_owner->getTransform()[0],
			_owner->getTransform()[1],
			_owner->getTransform()[2],
			_owner->getTransform()[3] + glm::vec4(glm::normalize(_endPosition - _originPosition) * _speed * step, 0.0f)));
	else {
		if (_loop) {
			glm::vec3 aux = _endPosition;
			_endPosition = _originPosition;
			_originPosition = aux;
		}
	}
}
