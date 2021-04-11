/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Input.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Input.hpp"
#include <iostream>
#include <chrono>
#include "Graphics.hpp"
#include "HUD.hpp"
#include "HUD_Level.hpp"

bool Input::mouse_trigger;
bool Input::key_trigger;
double Input::x_offset_;
double Input::y_offset_;

std::bitset<GLFW_KEY_LAST> Input::key_pressed;
std::bitset<GLFW_KEY_LAST> Input::key_triggered;
std::bitset<GLFW_KEY_LAST> Input::key_released;

vector2 Input::mousePos;
vector2 Input::windowSize;

std::bitset<GLFW_MOUSE_BUTTON_LAST> Input::mouse_pressed;
std::bitset<GLFW_MOUSE_BUTTON_LAST> Input::mouse_triggered;
std::bitset<GLFW_MOUSE_BUTTON_LAST> Input::mouse_doubleclick;
std::bitset<GLFW_MOUSE_BUTTON_LAST> Input::mouse_released;


void Input::Triggerd_Reset()
{
    key_trigger = false;
    mouse_trigger = false;

    key_triggered.reset();
    mouse_triggered.reset();
    mouse_doubleclick.reset();

	x_offset_ = 0;
	y_offset_ = 0;
}

void Input::Pressed_Reset()
{
    mouse_released = false;
    key_released = false;

    mouse_doubleclick.reset();
    mouse_released.reset();

    x_offset_ = 0;
    y_offset_ = 0;
}

void Input::Initialize(int width, int height)
{
    key_pressed.reset();
    key_triggered.reset();
    key_released.reset();

    mouse_pressed.reset();
    mouse_triggered.reset();
    mouse_doubleclick.reset();
    mouse_released.reset();

    windowSize.x = static_cast<float>(width);
    windowSize.y = static_cast<float>(height);
}

void Input::Reset(void)
{
    key_pressed.reset();
    key_triggered.reset();
    key_released.reset();

    mouse_pressed.reset();
    mouse_triggered.reset();
    mouse_doubleclick.reset();
}

void Input::SetKeyPressed(int key, int action)
{
    if (key <= -1 || key >= 348)
        return;
    if (action == GLFW_PRESS)
    {
        key_trigger = true;
        key_pressed.set(key);
        key_released.reset(key);
    }
    else if (action == GLFW_RELEASE)
    {
        key_trigger = false;
        key_released.set(key);
        key_pressed.reset(key);
    }

    if (key_trigger)
    {
       key_triggered.set(key);
    }
    else
    {
        key_triggered.reset(key);
    }
}

bool Input::IsKeyPressed(int key)
{
    return key_pressed[key];
}

bool Input::IsKeyTriggered(int key)
{
    return key_triggered[key];
}

bool Input::IsKeyReleased(int key)
{
    return key_released[key];
}

bool Input::IsKeyAnyPressed()
{
    return key_pressed.any();
}

bool Input::IsKeyAnyTriggered()
{
    return key_triggered.any();
}

bool Input::IsKeyAnyReleased()
{
    return key_released.any();
}

void Input::SetMousePos(double x_pos, double y_pos)
{
	vector2 viewport_size = { (float)Application_.GetGLFWvidmode()->width, (float)Application_.GetGLFWvidmode()->height };

	float ratio_x = viewport_size.x / windowSize.x;
	float ratio_y = viewport_size.y / windowSize.y;

	if(HUD_Level::IsOptionWindowOpen)
	{
		mousePos.x = static_cast<float>(-((windowSize.x / 2.0f) - x_pos)) * ratio_x;
		mousePos.y = static_cast<float>((windowSize.y / 2.0f) - y_pos) * ratio_y;
	}
	else
	{
		mousePos.x = static_cast<float>(-((windowSize.x / 2.0f) - x_pos)) * ratio_x + Graphics::camera_center.x;
		mousePos.y = static_cast<float>((windowSize.y / 2.0f) - y_pos) * ratio_y + Graphics::camera_center.y;		
	}
}

vector2 Input::GetMousePos()
{
    return mousePos;
}

void Input::SetMousePressed(int button, int action)
{
    if (action == GLFW_PRESS)
    {
        mouse_trigger = true;

        mouse_pressed.set(button);
        mouse_released.reset(button);
    }
    else if (action == GLFW_RELEASE)
    {
        static auto before = std::chrono::system_clock::now();
        auto now = std::chrono::system_clock::now();

        double diff_ms = std::chrono::duration<double, std::milli>(now - before).count();
        before = now;

        if (diff_ms > 10 && diff_ms < 200)
        {
            mouse_doubleclick.set(button);
        }
        else
        {
            mouse_doubleclick.reset(button);
        }

        mouse_trigger = false;

        mouse_pressed.reset(button);
        mouse_released.set(button);
    }

    if (mouse_trigger)
    {
        mouse_triggered.set(button);
    }
    else
    {
        mouse_triggered.reset(button);
    }
}

void Input::SetMouseWheelScroll(double x_offset, double y_offset)
{
	x_offset_ = x_offset;
	y_offset_ = y_offset;
}

bool Input::IsMousePressed(int button)
{
    return mouse_pressed[button];
}

bool Input::IsMouseTriggered(int button)
{
    return mouse_triggered[button];
}

bool Input::IsMouseDoubleClicked(int button)
{
    return mouse_doubleclick[button];
}

bool Input::IsMouseReleased(int button)
{
    return mouse_released[button];
}

double Input::MouseWheelScroll()
{
	return y_offset_;
}

Object* Input::ImGuiObjectClicker()
{
	for (auto& obj : Objectmanager_.GetObjectMap())
	{
		if (GetMousePos().x < obj->GetTransform().GetTranslation().x + obj->GetTransform().GetScale().x &&
			GetMousePos().x > obj->GetTransform().GetTranslation().x - obj->GetTransform().GetScale().x &&
			GetMousePos().y < obj->GetTransform().GetTranslation().y + obj->GetTransform().GetScale().y &&
			GetMousePos().y > obj->GetTransform().GetTranslation().y - obj->GetTransform().GetScale().y)
		{
			if (obj.get()->GetObjectType() != ObjectType::Capture_Camera_main)
			{
				return obj.get();
			}
		}
	}
	return nullptr;
}

Object* Input::ClickObject(ObjectDepth type)
{
	for (auto& obj : Objectmanager_.GetObjectMap())
	{
		if (GetMousePos().x < obj->GetTransform().GetTranslation().x + obj->GetTransform().GetScale().x &&
			GetMousePos().x > obj->GetTransform().GetTranslation().x - obj->GetTransform().GetScale().x &&
			GetMousePos().y < obj->GetTransform().GetTranslation().y + obj->GetTransform().GetScale().y &&
			GetMousePos().y > obj->GetTransform().GetTranslation().y - obj->GetTransform().GetScale().y)
		{
			switch(type)
			{
			case ObjectDepth::CAPTURE_OBJECT :
			{
				if (obj->GetTransform().GetDepth() == -0.9f)
					return obj.get();
				break;
			}

			case ObjectDepth::POLAROID:
			{
				if (obj->GetTransform().GetDepth() == 0.9f)
					return obj.get();
				break;
			}

			case ObjectDepth::GAME_OBJECT:
			{
				if (obj->GetTransform().GetDepth() == -0.5f)
					return obj.get();
				break;
			}

			case ObjectDepth::HUD_OBJECT:
			{
				if (obj->GetTransform().GetDepth() == -0.2f)
					return obj.get();
				break;
			}

			case ObjectDepth::HUD_OBJECT2:
			{
				if (obj->GetTransform().GetDepth() == -0.3f)
					return obj.get();
				break;
			}

			case ObjectDepth::HUD_BUTTON:
			{
				if (obj->GetTransform().GetDepth() == -0.65f)
					return obj.get();
				break;
			}

			case ObjectDepth::BACKGROUND:
			{
				if (obj->GetTransform().GetDepth() == -0.1f)
					return obj.get();
				break;
			}
			default:
				break;
			}
		}
	}
	return nullptr;
}

Object* Input::ClickHUDButton()
{
	for (auto& obj : HUD_.Get_HUD_Object_Manager())
	{
		if (GetMousePos().x < obj->GetTransform().GetTranslation().x + obj->GetTransform().GetScale().x &&
			GetMousePos().x > obj->GetTransform().GetTranslation().x - obj->GetTransform().GetScale().x &&
			GetMousePos().y < obj->GetTransform().GetTranslation().y + obj->GetTransform().GetScale().y &&
			GetMousePos().y > obj->GetTransform().GetTranslation().y - obj->GetTransform().GetScale().y)
		{
			if (obj->GetTransform().GetDepth() == -0.65f)
				return obj.get();
		}
	}
	return nullptr;
}