/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Engine.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include "Timer.hpp"


class Engine
{
public:
    bool Initialize();
    void Update();
    void Quit();

	void System_Initialize();
	void System_Update();
	void System_Quit();

    static bool IsQuit;
    /*template <typename SYSTEM>
    SYSTEM* GetSystemByTemplate() const;*/

private:
    Timer gameTimer{};
    float dt = 0;
};
