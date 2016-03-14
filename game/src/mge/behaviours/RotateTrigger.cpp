#include "mge/behaviours/RotateTrigger.hpp"
#include "mge/core/GameObject.hpp"
#include <sfml/window/event.hpp>
#include <string>
#include <vector>
#include "mge/util/Utility.hpp"
#include "mge/core/Level.hpp"
#include "mge/behaviours/RotatingBehaviour.hpp"

RotateTrigger::RotateTrigger(float pRadius, glm::vec3 pAxis, float pAngles, GameObject* pParent) : TriggerBehaviour()
{
	_radius = pRadius;

	_axis = pAxis;
	_anglePerSec = pAngles;

	_activator = Level::CurrentPlayer;
	_parent = pParent;
}

RotateTrigger::RotateTrigger(std::string params) : TriggerBehaviour()
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

	_axis = Utility::StrToVec(str[2], str[3], str[4]);
	_anglePerSec = atof(str[5].c_str());
	
}

RotateTrigger::~RotateTrigger()
{
	_activator = NULL;
}

void RotateTrigger::trigger()
{
	_parent->setBehaviour(new RotatingBehaviour(_axis, _anglePerSec));
}


