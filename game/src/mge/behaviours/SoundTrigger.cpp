#include "mge/behaviours/SoundTrigger.hpp"
#include "mge/core/SoundEngine.hpp"
#include "mge/core/GameObject.hpp"
#include <sfml/window/event.hpp>
#include <string>
#include <vector>
#include "mge/util/Utility.hpp"
#include "mge/core/Level.hpp"

SoundTrigger::SoundTrigger(float pRadius, std::string pSoundName, SoundType pSoundType, GameObject* pParent) : TriggerBehaviour()
{
	_audioName = pSoundName;
	_soundType = pSoundType;
	_radius = pRadius;
	_parent = pParent;

	_activator = Level::CurrentPlayer;
}

SoundTrigger::SoundTrigger(std::string params) : TriggerBehaviour()
{
	_activator = Level::CurrentPlayer;

	std::vector<std::string> str = Utility::Split(params, ',');

	//radius name type
	_radius = atof(str[0].c_str());
	_audioName = str[1];
	_soundType = (SoundType)std::stoi(str[2]);

	if (_soundType == SoundType::Sound)
	{
		std::vector<GameObject*> gos = Level::GetGameObjects();

		for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); ++it)
		{
			if (0 == strcmp((*it)->UniqueId.c_str(), str[3].c_str()))
			{
				_parent = (*it);
				break;
			}
		}
	}
}

SoundTrigger::~SoundTrigger()
{
}

void SoundTrigger::trigger()
{
	switch (_soundType)
	{
	case SoundType::Music:
		SoundEngine::PlayMusic(_audioName);
		break;
	case SoundType::Voice:
		SoundEngine::PlayVoice(_audioName);
		break;
	case SoundType::Sound:
		SoundEngine::PlayAudio(_audioName, _parent);
		break;
	}
}


