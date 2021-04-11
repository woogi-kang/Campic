/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : AudioManager.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once

#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_dsp.h>
#include <fmod_dsp_effects.h>
#include <fmod_codec.h>
#include <fmod.h>
#include <string>
#include <unordered_map>
#include <math.h>
#include <stdlib.h> 
#include <time.h>

float ChangeSemitone(float frequency, float variation);
float RandomBetween(float min, float max);

class AudioManager
{
public:
	bool Initialize();
	void Update(float dt);
	void Quit();

	void LoadSFX(const std::string& path);
	void LoadSong(const std::string& path);
	void PlaySFX(const std::string& path, float volume);
	void PlaySong(const std::string& path);
	void ChnageSFX(const std::string& path);
	void ChnageSong(const std::string& path);
	void StopSFXs();
	void StopSongs();
	void SetMasterVolume(float volume);
	void SetSFXsVolume(float volume); 
	void SetSongsVolume(float volume);

	bool IsSFXPlaying();
	bool IsBGMPlaying();
	bool IsMasterPlaying();

	void SetTimer(bool timer_);

	FMOD::ChannelGroup* GetBGMChannel() { return groups[CATEGORY_SONG]; }
	FMOD::ChannelGroup* GetSFXChannel() { return groups[CATEGORY_SFX]; } 
	FMOD::ChannelGroup* GetMasterChannel() { return master; } 

	enum Category { CATEGORY_SFX, CATEGORY_SONG, CATEGORY_COUNT };
	enum FadeState { FADE_NONE, FADE_IN, FADE_OUT };

private:  
	typedef std::unordered_map<std::string, FMOD::Sound*> SoundMap;  
	
	void Load(Category type, const std::string& path);    
	
	FMOD::System* system = nullptr;
	FMOD::ChannelGroup* master = nullptr;
	FMOD::ChannelGroup* groups[CATEGORY_COUNT];
	FMOD::Channel* currentSong = nullptr;

	SoundMap sounds[CATEGORY_COUNT];
	FMOD_MODE modes[CATEGORY_COUNT];
	
	std::string currentSongPath; 
	std::string nextSongPath;   
	
	FadeState fade;

	const float fadeTime = 0.1f;
	bool timer = false;
	float time = 0.f;
};

inline float ChangeSemitone(float frequency, float variation)
{
	static float semitone_ratio = pow(2.0f, 1.0f / 12.0f);  
	return frequency * pow(semitone_ratio, variation);
}

inline float RandomBetween(float min, float max)
{
	if (min == max) return min;  
	float n = (float)rand() / (float)RAND_MAX;  
	return min + n * (max - min);
}

extern AudioManager AudioManager_;