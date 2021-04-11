/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Application.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Application.hpp"
#include <iostream>
#include "Engine.hpp"
#include <string>
#include "Imgui_System.hpp"

Application Application_;

namespace
{
    void Window_Exit(GLFWwindow* window);
    void errorCallback(int errorCode, const char* errorDescription);
    void window_resized(GLFWwindow* window, int width, int height);
    void KeyCallback(GLFWwindow* window, int key, int scancode
                     , int action, int mods);
    void MousePositionCallback(GLFWwindow* window, double x_pos, double y_pos);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void MouseWheelScroll(GLFWwindow* window, double x_offset, double y_offset);
	void DropCallBack(GLFWwindow* window, int count, const char** paths);
}

bool Application::Initialize()
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
    {
        std::cerr << "GLFW Initialize failed" << '\n';
        return false;
    }
	screenSize = vector2{1280, 960};
	
	// We use OpenGL 3.3 version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // We don't use the past OpenGL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* window make 
    4 parameter : full screen mode -> if want to use glfwGetPrimaryMonitor(),
     5 patameter : share resources*/

	//TODO get fullscreen is true or false and adjust the size when make window
    window = glfwCreateWindow(static_cast<int>(screenSize.x), static_cast<int>(screenSize.y), "CamPic",
                              nullptr, nullptr);

	//mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowMonitor(window, nullptr, (mode->width / 2) - static_cast<int>(screenSize.x / 2),
                         (mode->height / 2) - static_cast<int>(screenSize.y / 2),
                         static_cast<int>(screenSize.x), static_cast<int>(screenSize.y), 0);

	FullScreen();

    if (!window)
    {
        std::cerr << "Fail to make window" << '\n';
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, window_resized);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, MouseWheelScroll);
	glfwSetDropCallback(window, DropCallBack);
	glfwSetWindowCloseCallback(window, Window_Exit);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    Input::Initialize(static_cast<int>(real_screenSize.x), static_cast<int>(real_screenSize.y));

    glewExperimental = GL_TRUE;
    GLenum errorCode = glewInit();

    if (GLEW_OK != errorCode)
    {
        std::cerr << "Error: Fail to Initialize GLFW" <<
            glewGetErrorString(errorCode) << '\n';

        glfwTerminate();
        return false;
    }
    glfwSwapInterval(true);
	srand((unsigned)time(NULL));

	int w, h;
	glfwGetWindowSize(window, &w, &h);
	real_screenSize.x = static_cast<float>(w);
	real_screenSize.y = static_cast<float>(h);

    return true;
}

void Application::Update(float dt)
{
	fpsEllapsedTime += dt;
	++fpsFrames;
	if (fpsEllapsedTime >= 1.0f)
	{		
		fpsEllapsedTime = 0;
		fpsFrames = 0;
	}



	Input::Triggerd_Reset();
        Input::Pressed_Reset();
#ifdef _DEBUG
#else
	glfwSwapBuffers(window);
#endif

    PollEvent();
}

void Application::Key_Poll_Event()
{
    if (Input::IsKeyTriggered(GLFW_KEY_F11))
    {
        FullScreen();
		glfwSwapInterval(1);
    }
}

void Application::Quit()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::FullScreen()
{
	fullScreenMode = !fullScreenMode;

    if (fullScreenMode)
    {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        monitor = glfwGetPrimaryMonitor();
        glfwSetWindowMonitor(window, monitor, 0, 0, 
			static_cast<int>(mode->width), static_cast<int>(mode->height), 0);

		int w, h;
		glfwGetWindowSize(window, &w, &h);
		real_screenSize.x = (float)w;
		real_screenSize.y = (float)h;

		glfwSwapInterval(1);

                Input::Initialize(static_cast<int>(real_screenSize.x), static_cast<int>(real_screenSize.y));
    }
    else
    {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(window, nullptr, (mode->width / 2) - static_cast<int>(screenSize.x / 2),
                             (mode->height / 2) - static_cast<int>(screenSize.y / 2),
                             static_cast<int>(screenSize.x), static_cast<int>(screenSize.y), 0);

		int w, h;
		glfwGetWindowSize(window, &w, &h);
		real_screenSize.x = (float)w;
		real_screenSize.y = (float)h;
		glfwSwapInterval(1);
                Input::Initialize(static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));

    }
}

void Application::PollEvent()
{
	//glfwSwapInterval(true);
    glfwPollEvents();
    Key_Poll_Event();
}

namespace
{
    void Window_Exit(GLFWwindow* window)
    {
        Engine::IsQuit = true;
    }

    void errorCallback(int errorCode, const char* errorDescription)
    {
        fprintf(stderr, "Error: %s\n", errorDescription);
    }

    void window_resized(GLFWwindow* window, int width, int height)
    {
    }

    void KeyCallback(GLFWwindow* window, int key, int scancode
                     , int action, int mods)
    {
		Input::SetKeyPressed(key, action);
    }

	void DropCallBack(GLFWwindow* window, int count, const char** paths)
	{
		std::string path(paths[0]);
	}

    void MousePositionCallback(GLFWwindow* window, double x_pos, double y_pos)
    {
        Input::SetMousePos(x_pos, y_pos);
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
		Input::SetMousePressed(button, action);
    }

	void MouseWheelScroll(GLFWwindow* window, double x_offset, double y_offset)
    {
		Input::SetMouseWheelScroll(x_offset, y_offset);
    }
}
