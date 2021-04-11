/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Application.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Input.hpp"
#include "vector2.hpp"
#include "State.hpp"

class Graphics;
class Objectmanager;


class Application
{
public:
    bool Initialize();
    void Update(float dt);
    void Quit();

	const GLFWvidmode* GetGLFWvidmode() { return mode; }
	GLFWwindow* GetWindow() { return window; }
	vector2 GetScreenSize() { return real_screenSize; }
	vector2 GetSavedScreenSize() { return screenSize; }

    void Key_Poll_Event();
    void PollEvent();
    void FullScreen();
	std::string GetFPS() { return fps; }

private:
	float fpsEllapsedTime = 0;
	int fpsFrames = 0;

	const GLFWvidmode* mode = nullptr;
    GLFWmonitor* monitor = nullptr;

    GLFWwindow* window = nullptr;
    bool fullScreenMode = false;

	std::string fps;
    std::string title;
    vector2 screenSize;
	vector2 real_screenSize;
};

extern Application Application_;
