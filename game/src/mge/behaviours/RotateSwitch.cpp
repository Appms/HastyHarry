#include "mge/behaviours/RotateSwitch.hpp"
#include "mge/core/GameObject.hpp"
#include <sfml/window/event.hpp>
#include <string>
#include <vector>
#include "mge/util/Utility.hpp"
#include "mge/core/Level.hpp"
#include "mge/behaviours/RotatingBehaviour.hpp"

RotateSwitch::RotateSwitch(glm::vec3 pAxis, float pAngles, GameObject* pParent) : SwitchBehaviour()
{
	_axis = pAxis;
	_anglePerSec = pAngles;

	_parent = pParent;
}

RotateSwitch::RotateSwitch(std::string params) : SwitchBehaviour()
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

	_axis = Utility::StrToVec(str[1], str[2], str[3]);
	_anglePerSec = atof(str[4].c_str());
	
}

RotateSwitch::~RotateSwitch() {}

void RotateSwitch::trigger()
{
	if (!_triggered) {
		if (_parent) _parent->setBehaviour(new RotatingBehaviour(_axis, _anglePerSec));
		_triggered = true;
	}
}


