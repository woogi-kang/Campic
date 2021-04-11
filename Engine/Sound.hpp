/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Sound.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once
#include "Component.hpp"
#include "AudioManager.hpp"

class AudioManager;

class Sound : public Component
{
public:
	Sound(std::string path_, AudioManager::Category category_ = AudioManager::CATEGORY_SFX, float volume_ = 0.3f)
	: path(path_), category(category_)
	{
		paths.push_back(path);
		AudioManager_.LoadSFX(path);
	}

	bool Initialize(Object* Ob) override;
	void Update(float dt) override;
	void Delete() override;

	void Play(std::string path, float volume);
	void AddSound(std::string path_);

	std::vector<std::string> GetSoundPaths() const { return paths; }
	AudioManager::Category& GetCategory() { return category; };

private:
	std::vector<std::string> paths;
	std::string path;

	AudioManager::Category category;
};

