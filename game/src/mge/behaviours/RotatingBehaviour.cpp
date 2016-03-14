#include <string>

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/util/Utility.hpp"

#include <vector>

RotatingBehaviour::RotatingBehaviour(float anglePerSec) :AbstractBehaviour()
{
	_anglePerSec = anglePerSec;
}

RotatingBehaviour::RotatingBehaviour(std::string params) : AbstractBehaviour()
{
	std::vector<std::string> str = Utility::Split(params, ',');
	_axis = glm::vec3(atof(str[0].c_str()), atof(str[1].c_str()), atof(str[2].c_str()));
	_anglePerSec = atof(str[3].c_str());
}

RotatingBehaviour::~RotatingBehaviour()
{
	//dtor
}

void RotatingBehaviour::update(float step)
{
	_owner->rotate(step * glm::radians(_anglePerSec), _axis);
}
