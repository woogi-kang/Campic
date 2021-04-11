/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Input.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once
#include <bitset>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "vector2.hpp"
#include "Object.hpp"
#include "ObjectDepth.hpp"

enum class ObjectDepth;

class Input
{
private:
    static std::bitset<GLFW_KEY_LAST> key_pressed;
    static std::bitset<GLFW_KEY_LAST> key_triggered;
    static std::bitset<GLFW_KEY_LAST> key_released;

    static vector2 mousePos;
    static vector2 windowSize;

    static std::bitset<GLFW_MOUSE_BUTTON_LAST> mouse_pressed;
    static std::bitset<GLFW_MOUSE_BUTTON_LAST> mouse_triggered;
    static std::bitset<GLFW_MOUSE_BUTTON_LAST> mouse_doubleclick;
    static std::bitset<GLFW_MOUSE_BUTTON_LAST> mouse_released;

    static bool mouse_trigger;
    static bool key_trigger;
	static double x_offset_;
	static double y_offset_;

public:
    static void Triggerd_Reset();
    static void Pressed_Reset();

    static void Initialize(int width = 0, int height = 0);
    static void Reset(void);

    static void SetKeyPressed(int key, int action);
    static bool IsKeyPressed(int key);
    static bool IsKeyTriggered(int key);
    static bool IsKeyReleased(int key);
    static bool IsKeyAnyPressed();
    static bool IsKeyAnyTriggered();
    static bool IsKeyAnyReleased();

    static void SetMousePos(double x_pos, double y_pos);
    static vector2 GetMousePos();

    static void SetMousePressed(int button, int action);
	static void SetMouseWheelScroll(double x_offset, double y_offset);

    static bool IsMousePressed(int button);
    static bool IsMouseTriggered(int button);
    static bool IsMouseDoubleClicked(int button);
    static bool IsMouseReleased(int button);
	static double MouseWheelScroll();

	static Object* ImGuiObjectClicker();
	static Object* ClickObject(ObjectDepth type);
	static Object* ClickHUDButton();
};
