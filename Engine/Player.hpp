/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Player.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include "Component.hpp"
#include <vector>
#include "Object.hpp"
#include "ObjectDepth.hpp"

struct vector2;

const vector2 player_scale = { 50.f,65.f };


class Player : public Component
{
public:

	bool Initialize(Object* Ob);
	void Update(float dt);
	void Delete();

	void SetIsMove(bool condition) { ismove = condition; }
        void SetRelease(float re) { m_release_time = re; }
	bool IsMove() { return ismove; }
        void MovePlayer();

	vector2 GetMouseOffset() { return offset; }
	//void PlayerMove(vector2 mouse_position);

private:
    float m_release_time = 0;
    bool m_twinkle = false;
	bool ismove = false;
        bool bus = true;
	void MouseTest();
        Object* bus_object = nullptr;
	//vector2 direction;
	vector2 prev_mouse_pos;
	vector2 offset{};
	vector2 save_pos{};
};
