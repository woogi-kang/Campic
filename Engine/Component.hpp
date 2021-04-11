/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Component.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/


#pragma once


class Object;

class Component
{
private:
public:
	/*Component()
		:object(nullptr)
	{};*/
	
    Object* object = nullptr;

    virtual bool Initialize(Object* Ob) = 0;
    virtual void Update(float dt) = 0;
    virtual void Delete() = 0; 
	//virtual ~Component() = 0;
};
