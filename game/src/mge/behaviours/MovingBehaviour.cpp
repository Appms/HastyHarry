#include <vector>

#include "mge/behaviours/MovingBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/util/Utility.hpp"

MovingBehaviour::MovingBehaviour(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop) :AbstractBehaviour(),
_originPosition(pOrigin), _endPosition(pEnd), _speed(pSpeed), _loop(pLoop) {}

MovingBehaviour::MovingBehaviour(std::string params) : AbstractBehaviour()
{
	std::cout << params << std::endl;
	std::vector<std::string> str = Utility::Split(params, ',');
	_originPosition = glm::vec3(atof(str[0].c_str()), atof(str[1].c_str()), atof(str[2].c_str()));
	_endPosition = glm::vec3(atof(str[3].c_str()), atof(str[4].c_str()), atof(str[5].c_str()));
	_speed = atof(str[6].c_str());
	
	//istringstream("true") >> std::boolalpha >> _loop;
	//istringstream("1") >> _loop;

	//_anglePerSec = atof(params.c_str());
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
			_owner->getTransform()[3] + 
				glm::vec4(glm::normalize(_endPosition - _originPosition) * _speed, 0.0f)));
	else {
		if (_loop) {
			glm::vec3 aux = _endPosition;
			_endPosition = _originPosition;
			_originPosition = aux;
		}
		else if(_owner->getName() != "Butterfly") delete _owner;
	}
}
