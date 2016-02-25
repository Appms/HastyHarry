#include "mge/behaviours/SoundBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

#include <sfml/window/event.hpp>
#include <iostream>

SoundBehaviour::SoundBehaviour(std::string pSoundName, glm::vec3 pPosition)
{
	if (!_soundBuffer.loadFromFile(config::MGE_AUDIO_PATH + pSoundName)) {
		std::cout << "ERROR: Jumping sound failed to load" << std::endl;
	}
	else {
		_sound.setPosition(pPosition.x, pPosition.y, pPosition.z);
		_sound.setMinDistance(5.f);
		_sound.setAttenuation(10.f);

		_sound.setBuffer(_soundBuffer);
	}
}

SoundBehaviour::~SoundBehaviour()
{
}

void SoundBehaviour::update( float pStep )
{
	_timer += pStep;

	if (_timer > 1.0f) {
		_sound.play();
		_timer -= 1.0f;
	}

	//_sound.setPosition(_owner->getWorldPosition().x, _owner->getWorldPosition().y, _owner->getWorldPosition().z);
}
