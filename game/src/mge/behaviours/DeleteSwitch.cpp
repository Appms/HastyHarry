#include "mge/behaviours/DeleteSwitch.hpp"
#include "mge/core/GameObject.hpp"
#include <sfml/window/event.hpp>
#include <string>
#include <vector>
#include "mge/util/Utility.hpp"
#include "mge/core/Level.hpp"
#include "mge/behaviours/MovingBehaviour.hpp"

DeleteSwitch::DeleteSwitch(GameObject* pParent) : SwitchBehaviour()
{
	_parent = pParent;
}

DeleteSwitch::DeleteSwitch(std::string params) : SwitchBehaviour()
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
}

DeleteSwitch::~DeleteSwitch() {}

void DeleteSwitch::trigger()
{
	if (!_triggered) {
		if (_parent) delete _parent;
		_triggered = true;
	}
}


