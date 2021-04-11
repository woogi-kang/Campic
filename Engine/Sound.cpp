/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Sound.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#include "Sound.hpp"

bool Sound::Initialize(Object * Ob)
{
	object = Ob;

	return true;
}

void Sound::Update(float /*dt*/)
{
}

void Sound::Delete()
{
	paths.clear();
	path.clear();
}

void Sound::Play(std::string path, float volume)
{
	AudioManager_.PlaySFX(path, volume);
}

void Sound::AddSound(std::string path_)
{
	paths.push_back(path_);
	AudioManager_.LoadSFX(path_);
}

