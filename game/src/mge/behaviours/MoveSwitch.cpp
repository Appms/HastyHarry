#include "mge/behaviours/MoveSwitch.hpp"
#include "mge/core/GameObject.hpp"
#include <sfml/window/event.hpp>
#include <string>
#include <vector>
#include "mge/util/Utility.hpp"
#include "mge/core/Level.hpp"
#include "mge/behaviours/MovingBehaviour.hpp"

MoveSwitch::MoveSwitch(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop, GameObject* pParent) : SwitchBehaviour()
{
	_originPosition = pOrigin;
	_endPosition = _originPosition + pEnd;
	_speed = pSpeed;
	_loop = pLoop;

	_parent = pParent;
}

MoveSwitch::MoveSwitch(std::string params) : SwitchBehaviour()
{
	std::vector<std::string> str = Utility::Split(params, ',');

	std::vector<GameObject*> gos = Level::GetGameObjects();

	for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); ++it)
	{
		if (0 == strcmp((*it)->UniqueId.c_str(), str[0].c_str()))
		{
			_parent = (*it);
			break;
		}
	}

	_originPosition = Utility::StrToVec(str[1], str[2], str[3]);

	_endPosition = _originPosition + Utility::StrToVec(str[4], str[5], str[6]);
	_speed = atof(str[7].c_str());
	if (0 == strcmp(str[8].c_str(), "True"))
	{
		_loop = true;
	}
	else if (0 == strcmp(str[8].c_str(), "False"))
	{
		_loop = false;
	}
	
}

MoveSwitch::~MoveSwitch() {}

void MoveSwitch::trigger()
{
	if (!_triggered) {
		if(_parent) _parent->setBehaviour(new MovingBehaviour(_originPosition, _endPosition, _speed, _loop));
		_triggered = true;
	}
}


