#include "mge/behaviours/MoveTrigger.hpp"
#include "mge/core/GameObject.hpp"
#include <sfml/window/event.hpp>
#include <string>
#include <vector>
#include "mge/util/Utility.hpp"
#include "mge/core/Level.hpp"
#include "mge/behaviours/MovingBehaviour.hpp"

MoveTrigger::MoveTrigger(float pRadius, glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop, GameObject* pParent) : TriggerBehaviour()
{
	_radius = pRadius;

	_originPosition = pOrigin;
	_endPosition = pEnd;
	_speed = pSpeed;
	_loop = pLoop;

	_activator = Level::CurrentPlayer;
	_parent = pParent;
}

MoveTrigger::MoveTrigger(glm::vec3 pStart, std::string params) : TriggerBehaviour()
{
	_activator = Level::CurrentPlayer;

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

	//radius name type
	_radius = atof(str[1].c_str());
	_originPosition = pStart;

	_endPosition = _originPosition + Utility::StrToVec(str[2], str[3], str[4]);
	_speed = atof(str[5].c_str());

	if (0 == strcmp(str[6].c_str(), "True"))
	{
		_loop = true;
	}
	else if (0 == strcmp(str[6].c_str(), "False"))
	{
		_loop = false;
	}
}

MoveTrigger::~MoveTrigger()
{
	_activator = NULL;
}

void MoveTrigger::trigger()
{
	if (_parent) _parent->setBehaviour(new MovingBehaviour(_originPosition, _endPosition, _speed, _loop));
}


