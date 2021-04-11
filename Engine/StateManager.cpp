/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : StateManager.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "StateManager.hpp"
#include "State.hpp"
#include "Graphics.hpp"
#include <map>
#include "Player.hpp"
#include "Physics.hpp"
#include <fstream>
#include "HUD.hpp"
#include "Capture.hpp"

StateManager StateManager_;

bool StateManager::Initialize()
{
	m_currentState = nullptr;

	return true;
}

void StateManager::AddStage(std::string ID, State* state)
{
	states.insert(std::make_pair(ID, state));

	if (m_currentState == nullptr)
	{
		if(auto& first_level = states.find(ID)->second;
			first_level->information_ == State_Information::Splash)
		{
			m_currentState = first_level.get();
		}
		else
		{
			m_currentState = first_level.get();
		}
		m_currentState->SetLevelIndicator(ID);
		m_currentState->Initialize();
	}
}

void StateManager::ChangeStage()
{
	std::string next_level = "Level";
	std::string save = m_currentState->GetLevelIndicator();

	Capture::IsChangeCaptureCount = true;

	m_currentState->ShutDown();
	m_currentState->ResetLevelChange();
	m_currentState = states.find(next_level)->second.get();

	m_currentState->LoadLevel(save);
	m_currentState->SetLevelIndicator(save);
	SetCurrentLevelCaptureLimit();
#ifdef _DEBUG
#else
        
#endif
	m_currentState->CreateCaptureCamera();
	if (m_currentState->GetCurrentStateInfo() == State_Information::Game)
	{
		m_currentState->CreatePlayer();
	}

	HUD_.SetIsChangeGameLevel(true);

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::BackToMenu()
{	
	m_currentState->ShutDown();
	m_currentState->ResetBackToMenu();
	m_currentState = states.find("LevelSelector")->second.get();

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::BackToMainMenu()
{
	m_currentState->ShutDown();
	m_currentState->ResetBackToMenu();
	m_currentState = states.find("MainMenu")->second.get();

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::ToCredit()
{
	m_currentState->ShutDown();
	m_currentState->ResetBackToMenu();
	m_currentState = states.find("Credit")->second.get();

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::ToHowToPlay()
{
	m_currentState->ShutDown();
	m_currentState->ResetBackToMenu();
	m_currentState = states.find("HowToPlay")->second.get();

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::ToStartScene()
{
	m_currentState->ShutDown();
	m_currentState->ResetBackToMenu();
	m_currentState = states.find("StartCutScene")->second.get();

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::ToEndScene()
{
	m_currentState->ShutDown();
	m_currentState->ResetBackToMenu();
	m_currentState = states.find("EndCutScene")->second.get();

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::ToLoseScene()
{
    prev_level = m_currentState->GetLevelIndicator();
	chapter = m_currentState->GetChapter().first;

    m_currentState->ShutDown();
    m_currentState->ResetBackToMenu();
    m_currentState = states.find("LoseScene")->second.get();

    m_currentState->Initialize();

    Physics_.ResetPreviousSize();
}

void StateManager::ToChapChange1()
{
	m_currentState->ShutDown();
	m_currentState->ResetBackToMenu();
	m_currentState = states.find("ChapterChange1")->second.get();

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::ToChapChange2()
{
	m_currentState->ShutDown();
	m_currentState->ResetBackToMenu();
	m_currentState = states.find("ChapterChange2")->second.get();

	m_currentState->Initialize();

	Physics_.ResetPreviousSize();
}

void StateManager::Restart()
{
	m_restart = true;
}

void StateManager::Pause()
{
	m_pause = true;
}

void StateManager::SetCurrentLevelCaptureLimit()
{
	std::string file_path = "asset/JsonFiles/Levels/";
	file_path.append(GetCurrentState()->GetLevelIndicator()+"/");
	file_path.append("capture_limit.txt");
	std::string num_limit;

	std::ifstream myfile(file_path.data());

	if(myfile.is_open())
	{
		std::getline(myfile, num_limit);
		GetCurrentState()->SetCaptureLimit(stoi(num_limit));

		myfile.close();
	}
}

void StateManager::Update(float dt)
{
	if (m_currentState->GetCurrentStateInfo() == State_Information::Game)
	{
		if (Input::IsKeyTriggered(GLFW_KEY_ESCAPE))
			TogglePause();
	}

     if (m_currentState->IsLevelChange())
     {
         ChangeStage();
         if (m_currentState->GetPlayerObjectPointer())
             m_currentState->GetPlayerObjectPointer()->GetTransform().SetTranslation(m_currentState->GetStartPosition());
     }

	if (m_currentState->IsBackToMenu())
		BackToMenu();


	if (GetCurrentState()->GetCaptureLimit() < 0)
	{
		ToLoseScene();
	}

	m_currentState->Update(dt);
}

void StateManager::Quit()
{
	states.clear();
}
