#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "SFML/Audio.hpp"
#include <vector>
#include <string>
#include <map>
#include <glm.hpp>

class GameObject;

enum SoundType
{
	Sound,
	Music,
	Voice
};

enum VoicePriority
{
	Effect,
	Feedback,
	Dialogue,
	Opaque
};

struct SoundData
{
	std::vector<std::string> FileName;
	std::vector<sf::SoundBuffer> SoundBuffer;
	float Attenuation;
	float MinDistance;
	SoundType Type;
	VoicePriority Priority;
};

class SoundEngine
{
	private:
		static std::map<std::string, SoundData> _soundData;
		static GameObject* _soundObjects[16];
		static sf::Music _music;
		static sf::Music _voice;
		static std::string _lastPlayedVoice;

	public:
		static void Init(std::string pConfigFileName);
		static void PlayAudio(std::string pAudioName, GameObject* pParent, glm::vec3 pPosition = glm::vec3(0,0,0));
		static void PlayVoice(std::string pAudioName);
		static void PlayMusic(std::string pAudioName);
};

#endif // SOUNDENGINE_H
