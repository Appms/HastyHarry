#include <vector>

#include "mge/behaviours/MovingBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/util/Utility.hpp"

MovingBehaviour::MovingBehaviour(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop) :AbstractBehaviour(),
_originPosition(pOrigin), _endPosition(pEnd), _speed(pSpeed), _loop(pLoop) {}


MovingBehaviour::MovingBehaviour(std::string params) : AbstractBehaviour()
{
	std::vector<std::string> str = Utility::Split(params, ',');
	_originPosition = Utility::StrToVec(str[0], str[1], str[2]);
	_endPosition = Utility::StrToVec(str[3], str[4], str[5]);
	_speed = atof(str[6].c_str());
	
	if (0 == strcmp(str[7].c_str(), "True"))
	{
		_loop = true;
	}
	else if (0 == strcmp(str[7].c_str(), "False"))
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

void MovingBehaviour::setDestroyFlag() { _destroyAtEnd = true; }

void MovingBehaviour::update(float step)
{
	//if (_owner->UniqueId == "14adc0dc-19fe-4dcc-838a-acdb7fcb5dd3") {
		//std::cout << "Hi!";
	//}

	float d1 = glm::distance(_owner->getWorldPosition(), _originPosition);
	float d2 = glm::distance(_endPosition, _originPosition);

	if (d1 < d2)
		_owner->setTransform(glm::mat4(
			_owner->getTransform()[0],
			_owner->getTransform()[1],
			_owner->getTransform()[2],
			_owner->getTransform()[3] + glm::vec4(glm::normalize(_endPosition - _originPosition) * _speed * step, 0.0f)));
		//_owner->translate(glm::normalize(_endPosition - _originPosition) * _speed * step);
	else {
		if (_loop) {
			glm::vec3 aux = _endPosition;
			_endPosition = _originPosition;
			_originPosition = aux;
		}
		else if (_destroyAtEnd) delete _owner;
	}
}
