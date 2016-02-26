#include "mge/behaviours/SoundBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

#include <sfml/window/event.hpp>
#include <iostream>

SoundBehaviour::SoundBehaviour(std::string pSoundName, glm::vec3 pPosition, bool playInstant = false, bool relative = false)
{
	if (!_soundBuffer.loadFromFile(config::MGE_AUDIO_PATH + pSoundName + ".wav")) {
		std::cout << "ERROR: " << pSoundName << " = sound failed to load" << std::endl;
	}
	else {
		_sound.setPosition(pPosition.x, pPosition.y, pPosition.z);
		_sound.setMinDistance(5.f);
		_sound.setAttenuation(10.f);
		_sound.setBuffer(_soundBuffer);
		_sound.setRelativeToListener(relative);
	}

	if (!playInstant)
	{
		_triggered = false;
		_sound.stop();
	}
	else
	{
		_triggered = true;
		_sound.play();
	}

}

SoundBehaviour::~SoundBehaviour()
{
}

void SoundBehaviour::update( float pStep )
{
	//_sound.setPosition(_owner->getWorldPosition().x, _owner->getWorldPosition().y, _owner->getWorldPosition().z);

	//if (_sound.getStatus() != sf::SoundSource::Status::Playing && _triggered)
	//{
		//_owner->SetTrigger();
		//std::cout << "Del" << std::endl;
		//delete this;
	//}
}

void SoundBehaviour::trigger(GameObject* pPlayer)
{
	_triggered = true;
	_sound.play();
}
