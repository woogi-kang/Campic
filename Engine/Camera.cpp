/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Camera.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Camera.hpp"
#include "matrix2.hpp"
#include "Input.hpp"
#include "State.hpp"
#include <iostream>
#include "Physics.hpp"


Camera::Camera(vector2 camera_center, vector2 camera_up)
	: center(camera_center), up(camera_up)
{}

bool Camera::Initialize(Object* Ob)
{
	if (object == nullptr)
	{
		object = Ob;
		if(!curr_level.empty())
			StateManager_.GetStateMap().find(curr_level)->second->SetCamera();

	}
	return true;
}

void Camera::Update(float dt)
{
#ifdef _DEBUG
#else
#endif
	if(base_obj != nullptr)
	{

		if (!Physics_.IsNextLevel())
		{
			if(base_obj->GetTransform().GetTranslation().x < min.x + center.x)
			{
				if (center.x > base_obj->GetTransform().GetTranslation().x + max.x)
				{
					if (abs(center.x - base_obj->GetTransform().GetTranslation().x + min.x) > 200)
						center.x -= 300 * dt;
					else
						center.x -= 100 * dt;
				}
			}
			else if (base_obj->GetTransform().GetTranslation().x > center.x + max.x)
			{
				if (center.x < base_obj->GetTransform().GetTranslation().x + min.x)
				{
					if (abs(center.x - base_obj->GetTransform().GetTranslation().x + max.x) > 200)
						center.x += 300 * dt;
					else
						center.x += 100 * dt;
				}
			}

			if (base_obj->GetTransform().GetTranslation().y < min.y + center.y)
			{
				if (center.y > base_obj->GetTransform().GetTranslation().y + max.y)
				{
					if (abs(center.y - base_obj->GetTransform().GetTranslation().y + min.y) > 200)
						center.y -= 300 * dt;
					else
						center.y -= 100 * dt;
				}
			}
			else if (base_obj->GetTransform().GetTranslation().y > center.y + max.y)
			{
				if (center.y < base_obj->GetTransform().GetTranslation().y + min.y)
				{
					if (abs(center.y - base_obj->GetTransform().GetTranslation().y + max.y) > 200)
						center.y += 300 * dt;
					else
						center.y += 100 * dt;
				}
			}
		}
	}

	if (Input::IsKeyTriggered(GLFW_KEY_C))
	{
		ResetUp();
	}
}

void Camera::Delete()
{
}

vector2 Camera::GetCenter() const
{
	return center;
}

void Camera::SetCenter(vector2 camera_center)
{
	center = camera_center;
}

vector2 Camera::GetUp() const
{
	return up;
}

vector2 Camera::GetRight() const
{
	return right;
}

void Camera::SetMinMaxSize(vector2 size)
{
	max = size;
	min = -size;
}

void Camera::ResetUp(vector2 camera_up)
{
	center = 0;
	up.x = camera_up.x;
	up.y = camera_up.y;
	right.x = camera_up.y;
	right.y = camera_up.x;
	zoom = 1.0f;
}

void Camera::MoveUp(float distance)
{
	center += up * distance;
}

void Camera::MoveRight(float distnace)
{
	center += right * distnace;
}

void Camera::Rotate(float angle)
{
	matrix2 rotate = rotation(angle);

	up = rotate * up;
	right = rotate * right;
}

affine2d Camera::CameraToWorld() const
{
	affine2d temp(right.x, up.x, 0, right.y, up.y, 0, center.x, center.y, 1);

	return temp;
}

affine2d Camera::WorldToCamera() const
{
	affine2d temp = { right.x, right.y, 0, up.x, up.y,0, 0,0,1 };
	affine2d translation = build_translation(-center.x, -center.y);

	temp *= translation;

	return temp;
}

