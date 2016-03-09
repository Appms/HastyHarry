#include "mge/behaviours/SoundBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"
#include "mge/core/Timer.hpp"
#include <sfml/window/event.hpp>
#include <iostream>
#include <string>
#include "mge/core/SoundEngine.hpp"

SoundBehaviour::SoundBehaviour()
{
}

SoundBehaviour::~SoundBehaviour()
{
}

void SoundBehaviour::update( float pStep )
{
	_sound.setPosition(_owner->getWorldPosition().x, _owner->getWorldPosition().y, _owner->getWorldPosition().z);
}

void SoundBehaviour::PlayAudio(sf::SoundBuffer &pBuffer, float pMinDistance, float pAttenuation)
{
	_sound.setBuffer(pBuffer);
	_sound.setPosition(_owner->getWorldPosition().x, _owner->getWorldPosition().y, _owner->getWorldPosition().z);

	_sound.setMinDistance(pMinDistance);
	_sound.setAttenuation(pAttenuation);

	_creationTime = Timer::now();

	_sound.play();
}

void SoundBehaviour::StopAudio()
{
	_sound.stop();
}

float SoundBehaviour::GetCreationTime()
{
	return _creationTime;
}

bool SoundBehaviour::IsFinished()
{
	return _sound.getStatus() != sf::Music::Status::Playing;
}