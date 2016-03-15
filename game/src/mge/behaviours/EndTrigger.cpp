#include "mge/behaviours/EndTrigger.hpp"
#include "mge/core/GameObject.hpp"
#include <sfml/window/event.hpp>
#include <string>
#include <vector>
#include "mge/util/Utility.hpp"
#include "mge/core/Level.hpp"
#include "mge/behaviours/MovingBehaviour.hpp"

EndTrigger::EndTrigger(float pRadius, std::string pNextLevel) : TriggerBehaviour()
{
	_radius = pRadius;

	_nextLevel = pNextLevel;

	_activator = Level::CurrentPlayer;
}

EndTrigger::EndTrigger(std::string params) : TriggerBehaviour()
{
	_activator = Level::CurrentPlayer;

	std::vector<std::string> str = Utility::Split(params, ',');

	//radius name type
	_radius = atof(str[0].c_str());
	_nextLevel = str[1];
}

EndTrigger::~EndTrigger()
{
	_activator = NULL;
}

void EndTrigger::trigger()
{
	Level::Load(_nextLevel + ".xml", Level::CurrentWorld);
}


