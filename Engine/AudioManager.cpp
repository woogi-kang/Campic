/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : AudioManager.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include "AudioManager.hpp"
#include <iostream>

AudioManager AudioManager_;

bool AudioManager::Initialize()
{
	currentSong = 0;
	fade = FADE_NONE;

	// Initialize system  
	FMOD::System_Create(&system);  
	system->init(100, FMOD_INIT_NORMAL, 0);

	// Create channels groups for each category  
	system->getMasterChannelGroup(&master); 
	for(int i = 0; i < CATEGORY_COUNT; ++i)
	{
		system->createChannelGroup(NULL, &groups[i]); master->addGroup(groups[i]);
	}
    // Set up modes for each category  
	modes[CATEGORY_SFX] = FMOD_DEFAULT;  
	modes[CATEGORY_SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

	LoadSong("asset/sounds/MainMenu_BGM.mp3");
	LoadSong("asset/sounds/BGM_Day.mp3");
	LoadSong("asset/sounds/BGM_Sunset.mp3");
	LoadSong("asset/sounds/BGM_Night.mp3");
    LoadSFX("asset/sounds/bus_leave.wav");
	LoadSFX("asset/sounds/Button.wav");
	LoadSFX("asset/sounds/pain.mp3");


	SetSFXsVolume(0.1f);
	std::cout << "AudioManager Initialization Successful" << std::endl;

	return true;
} 

void AudioManager::Update(float dt)
{
	//If a song is playing and we are fading in, increase the volume of the current song a bit. 
	//Once the volume reaches one, stop fading. 

	//If a song is playing and we are fading out, decrease the volume of the current song a bit. 
	//Once the volume reaches zero, stop the song, and stop fading.

	//If no song is playing, and there is a song set up to play next, start playing it: 

	// in seconds  

	if(timer)
	{
		time += dt;

		if(time > 1)
		{
			PlaySFX("asset/sounds/Camera_Capture.wav", 0.5f);
			timer = false;
			time = 0.f;
		}
	}
	
	
	if(currentSong != NULL && fade == FADE_IN)
	{
		float volume;    
		currentSong->getVolume(&volume);    
		float nextVolume = volume + dt / fadeTime;    
		
		if(nextVolume >= 1.0f)
		{
			currentSong->setVolume(1.0f);      
			fade = FADE_NONE;
		} 
		else
		{
			currentSong->setVolume(nextVolume);
		}
	} 
	
	else if(currentSong != NULL && fade == FADE_OUT)
	{
		float volume;    
		currentSong->getVolume(&volume);    
		float nextVolume = volume - dt / fadeTime;    
		
		if(nextVolume <= 0.0f)
		{
			currentSong->stop();      
			currentSong = NULL;      
			currentSongPath.clear();      
			fade = FADE_NONE;
		} 
		else
		{
			currentSong->setVolume(nextVolume);
		}
	} 
	
	else if(currentSong == NULL && !nextSongPath.empty())
	{
		PlaySong(nextSongPath);   
		nextSongPath.clear();
	} 
	
	system->update();
}

void AudioManager::Quit()
{
	// Release sounds in each category  
	for(int i = 0; i < CATEGORY_COUNT; ++i)
	{
		for (auto& temp : sounds[i])
		{
			temp.second->release();
		}
		sounds[i].clear();
	}

	// Release system  
	system->release();
}

void AudioManager::LoadSFX(const std::string& path)
{
	Load(CATEGORY_SFX, path);
}

void AudioManager::LoadSong(const std::string& path)
{
	Load(CATEGORY_SONG, path);
}

void AudioManager::PlaySFX(const std::string& path, float volume)
{
	// Try to find sound effect and return if not found  
	SoundMap::iterator sound = sounds[CATEGORY_SFX].find(path);  
	
	if (sound == sounds[CATEGORY_SFX].end()) 
		return;
	
	// Play the sound effect with these initial values  
	FMOD::Channel* channel = nullptr;
	channel->setChannelGroup(groups[CATEGORY_SFX]); 
	channel->setVolume(0.1f);

	system->playSound(sound->second, NULL, true, &channel);
	float frequency;  
	channel->getFrequency(&frequency); 
	channel->setFrequency(frequency);
	channel->setPaused(false);

}

void AudioManager::PlaySong(const std::string & path)
{
	// Ignore if this song is already playing  
	if(currentSongPath == path) return;
	
	// If a song is playing stop them and set this as the next song  
	if(currentSong != NULL)
	{
		StopSongs();    
		nextSongPath = path;    
		return;
	}

	// Find the song in the corresponding sound map  
	SoundMap::iterator sound = sounds[CATEGORY_SONG].find(path); 
	if (sound == sounds[CATEGORY_SONG].end()) 
		return;
 
	// Start playing song with volume set to 0 and fade in  
	currentSongPath = path;  
	system->playSound(sound->second, NULL, false, &currentSong);
	currentSong->setChannelGroup(groups[CATEGORY_SONG]);  
	currentSong->setVolume(0.5f);  
	currentSong->setPaused(false);  
	fade = FADE_IN; 
}

void AudioManager::ChnageSFX(const std::string & path)
{

}

void AudioManager::ChnageSong(const std::string& path)
{
	
}

void AudioManager::StopSFXs()
{
	groups[CATEGORY_SFX]->stop();
}

void AudioManager::StopSongs()
{
	if (currentSong != NULL)     
		fade = FADE_OUT;  
	nextSongPath.clear();
}

void AudioManager::SetMasterVolume(float volume)
{
	master->setVolume(volume);
}

void AudioManager::SetSFXsVolume(float volume)
{
	groups[CATEGORY_SFX]->setVolume(volume);
}

void AudioManager::SetSongsVolume(float volume)
{
	groups[CATEGORY_SONG]->setVolume(volume);
}

bool AudioManager::IsSFXPlaying()
{
	bool isPlaying;

	groups[CATEGORY_SFX]->isPlaying(&isPlaying);

	return isPlaying;
}

bool AudioManager::IsBGMPlaying()
{
	bool isPlaying;

	groups[CATEGORY_SONG]->isPlaying(&isPlaying);

	return isPlaying;
}

bool AudioManager::IsMasterPlaying()
{
	bool isPlaying;

	master->isPlaying(&isPlaying);

	return isPlaying;
}

void AudioManager::SetTimer(bool timer_)
{
	timer = timer_;
}

void AudioManager::Load(Category type, const std::string & path)
{
	if (sounds[type].find(path) != sounds[type].end())
		return;

	FMOD::Sound* sound;

	system->createSound(path.c_str(), modes[type], NULL, &sound);
	sounds[type].insert(std::make_pair(path, sound));
}
