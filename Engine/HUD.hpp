/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : HUD.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once
#include "State.hpp"
#include "Object.hpp"
#include <memory>
#include "Camera.hpp"
#include "StartCutScene.hpp"

class HUD
{
public:
	bool Initialize(State* HUD_State = nullptr);
	void Update(float dt);
	void Quit();

	std::vector<std::shared_ptr<Object>>& Get_HUD_Object_Manager() { return HUD_Object_Manager; }
	std::vector<std::shared_ptr<Object>>& Get_HUD_Button_Manager() { return HUD_Button_Manager; }
	void Add_HUD_Object(Object* obj);
	void Add_HUD_Button(Object* obj);
	bool IsChangeGameLevel() { return is_change_game_level; }

	State* Get_HUD_State() { return m_HUD_State; }
	State* Get_Current_Game_State() { return m_current_game_state; }
	//void Toggle_HUD_Active() { isHUDActive = !isHUDActive; }
	bool isHUDActive = false;
	Camera* Get_HUD_Camera() { return hud_camera; }
	void Toggle_HUD_Active() { isHUDActive = !isHUDActive; }
	void SetIsChangeGameLevel(bool condition) { is_change_game_level = condition; }
	bool HUDIntersectionCheck(vector2 mouse_pos);
	std::pair<Object*, CollisionState>& GetHUDSelect() { return m_selected_object; }


private:
	void SetPlayer(Object* obj);
	void HUD_Activing_Search();

	bool is_change_game_level = false;
	State* m_HUD_State = nullptr;
	State* m_current_game_state = nullptr;
	Camera* hud_camera = nullptr;

	std::pair<Object*, CollisionState> m_selected_object = { nullptr, CollisionState::NotCollided };
	std::vector<std::shared_ptr<Object>> HUD_Object_Manager;
	std::vector<std::shared_ptr<Object>> HUD_Button_Manager;
};

extern HUD HUD_;