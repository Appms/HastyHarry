#include "mge/core/SoundEngine.hpp"
#include "mge/core/Level.hpp"
#include "mge/behaviours/SoundBehaviour.hpp"
#include "mge/config.hpp"
#include <stdlib.h>
#include "mge/core/GameObject.hpp"
#include "tinyxml\tinyxml.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include "mge/core/Mesh.hpp"
#include "mge/materials/ColorMaterial.hpp"

#define MAX_SOUNDS 16

std::map<std::string, SoundData> SoundEngine::_soundData;
GameObject* SoundEngine::_soundObjects[MAX_SOUNDS];
sf::Music SoundEngine::_music;
sf::Music SoundEngine::_voice;
std::string SoundEngine::_lastPlayedVoice;

void SoundEngine::Init(std::string pConfigFileName)
{
	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		_soundObjects[i] = new GameObject("AudioSource", glm::vec3(0, 0, 0));
		_soundObjects[i]->setBehaviour(new SoundBehaviour());
	}

	_music.setRelativeToListener(true);
	_voice.setRelativeToListener(true);

	_music.setLoop(true);
	_voice.setLoop(false);

	_music.setAttenuation(0);
	_voice.setAttenuation(0);

	_music.setMinDistance(1);
	_voice.setMinDistance(1);

	TiXmlDocument doc;
	std::string filePath = config::MGE_AUDIO_PATH + pConfigFileName;
	if (!doc.LoadFile(filePath.c_str()))
	{
		std::cout << "Level Loader: " << doc.ErrorDesc() << std::endl;
		return;
	}

	//audio
	TiXmlElement* root = doc.FirstChildElement();
	if (root == NULL)
	{
		std::cout << "Audio Loader: Failed to load file: No root element." << std::endl;
		doc.Clear();
		return;
	}

	//sounds, voices, tracks
	for (TiXmlElement* category = root->FirstChildElement(); category != NULL; category = category->NextSiblingElement())
	{
		//sound, voice, track
		for (TiXmlElement* element = category->FirstChildElement(); element != NULL; element = element->NextSiblingElement())
		{
			std::string audioname = "";
			SoundType type = SoundType::Sound;

			if (0 == strcmp(element->Value(), "sound"))
			{
				type = SoundType::Sound;
			}
			else if (0 == strcmp(element->Value(), "voice"))
			{
				type = SoundType::Voice;
			}
			else if (0 == strcmp(element->Value(), "track"))
			{
				type = SoundType::Music;
			}

			//soundname, filename, etc.
			for (TiXmlElement* attribute = element->FirstChildElement(); attribute != NULL; attribute = attribute->NextSiblingElement())
			{
				if (0 == strcmp(attribute->Value(), "audioname"))
				{
					audioname = attribute->GetText();
				}
				else if (0 == strcmp(attribute->Value(), "filenames"))
				{
					for (TiXmlElement* name = attribute->FirstChildElement(); name != NULL; name = name->NextSiblingElement())
					{
						if (0 == strcmp(name->Value(), "filename"))
						{
							_soundData[audioname].FileName.push_back(name->GetText());

							if (type == SoundType::Sound)
							{
								_soundData[audioname].SoundBuffer.push_back(sf::SoundBuffer());
								_soundData[audioname].SoundBuffer.back().loadFromFile(config::MGE_AUDIO_PATH + _soundData[audioname].FileName.back());
							}
						}
					}
				}
				else if (0 == strcmp(attribute->Value(), "mindistance"))
				{
					_soundData[audioname].MinDistance = atof(attribute->GetText());
				}
				else if (0 == strcmp(attribute->Value(), "attenuation"))
				{
					_soundData[audioname].Attenuation = atof(attribute->GetText());
				}
				else if (0 == strcmp(attribute->Value(), "priority"))
				{
					//istringstream("1") >> _soundData[audioname].Persistent;
					_soundData[audioname].Priority = (VoicePriority)atoi(attribute->GetText());
				}
			}

			_soundData[audioname].Type = type;
		}
	}

	doc.Clear();
}

void SoundEngine::PlayAudio(std::string pAudioName, GameObject* pParent, glm::vec3 pPosition)
{
	int index = -1;

	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		if (((SoundBehaviour*)_soundObjects[i]->getBehaviour())->IsFinished())
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		float oldest = 0.0f;

		for (int i = 0; i < MAX_SOUNDS; ++i)
		{
			if (oldest < ((SoundBehaviour*)_soundObjects[i]->getBehaviour())->GetCreationTime())
			{
				oldest = ((SoundBehaviour*)_soundObjects[i]->getBehaviour())->GetCreationTime();
				index = i;
			}
		}
	}

	int randInt =  rand() % (int)(_soundData[pAudioName].SoundBuffer.size());
	_soundObjects[index]->setParent(pParent);
	_soundObjects[index]->setLocalPosition(pPosition);
	
	//This is for seeing soundobject position
	/*
	GameObject* test = new GameObject("");
	_soundObjects[index]->add(test);
	test->setLocalPosition(glm::vec3(0, 0, 0));
	test->setMesh(Mesh::load(config::MGE_MODEL_PATH + "cube.obj"));
	test->setMaterial(new ColorMaterial(glm::vec3(1, 0, 1)));
	*/
	//

	((SoundBehaviour*)_soundObjects[index]->getBehaviour())->PlayAudio(_soundData[pAudioName].SoundBuffer[randInt], _soundData[pAudioName].MinDistance, _soundData[pAudioName].Attenuation);
}

void SoundEngine::PlayVoice(std::string pAudioName)
{
	if(_lastPlayedVoice == "" || (int)_soundData[_lastPlayedVoice].Priority < (int)_soundData[pAudioName].Priority || _voice.getStatus() != sf::Music::Playing)
	{
		int randInt = rand() % (int)(_soundData[pAudioName].FileName.size());
		_voice.stop();
		_voice.openFromFile(config::MGE_AUDIO_PATH + _soundData[pAudioName].FileName[randInt]);
		_voice.play();
		_lastPlayedVoice = pAudioName;
	}
}

void SoundEngine::PlayMusic(std::string pAudioName)
{
	//TODO Import volume from file
	_music.setVolume(1.0f);
	int randInt = rand() % (int)(_soundData[pAudioName].FileName.size());
	_music.stop();
	_music.openFromFile(config::MGE_AUDIO_PATH+_soundData[pAudioName].FileName[randInt]);
	_music.play();
}
