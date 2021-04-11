/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Engine.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Engine.hpp"
#include "Application.hpp"
#include "StateManager.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "AudioManager.hpp"
#include "Physics.hpp"
#include "Imgui_System.hpp"
#include "JSON.hpp"
#include "HUD.hpp"
#include "HUD_Level.hpp"
#include "Tile_Map.hpp"
#include "LevelJson.hpp"

bool Engine::IsQuit;

bool Engine::Initialize()
{
	System_Initialize();

    gameTimer.Reset();
    IsQuit = false;
	srand(static_cast<unsigned int>(time(NULL)));
    return true;
}

void Engine::Update()
{
    while (!IsQuit)
    {
        dt = (float)gameTimer.GetElapsedSeconds();
        gameTimer.Reset();

		System_Update();

		Graphics_.BeginDraw();
		Graphics_.Draw();
		Graphics_.Tile_Draw();
		Graphics_.Button_Draw();
		Graphics_.HUD_Draw();
		Graphics_.EndDraw();
#ifdef _DEBUG
		IMGUI_.Draw();
#endif

    }
}

void Engine::Quit()
{
	System_Quit();
}

void Engine::System_Initialize()
{
	Application_.Initialize();
	StateManager_.Initialize();
	Graphics_.Initialize();
	Objectmanager_.Initialize();
#ifdef _DEBUG
	IMGUI_.Initialize();
#endif
	AudioManager_.Initialize();
	Physics_.Initialize();
 	HUD_.Initialize(new HUD_Level());
	JSON_.Initialize();
	LevelJson_.Initialize();
	
}

void Engine::System_Update()
{
	Application_.Update(dt);
	StateManager_.Update(dt);
	Graphics_.Update(dt);
	Objectmanager_.Update(dt);
    Physics_.Update(dt);
    Tile_Map_.Update(dt);
#ifdef _DEBUG
	IMGUI_.Update(dt);
#endif
	AudioManager_.Update(dt);
	HUD_.Update(dt);
}

void Engine::System_Quit()
{
	Application_.Quit();
	StateManager_.Quit();
	Graphics_.Quit();
#ifdef _DEBUG
	IMGUI_.Quit();
#endif
	AudioManager_.Quit();
	Physics_.Quit();
	HUD_.Quit();
	Objectmanager_.Quit();
}
