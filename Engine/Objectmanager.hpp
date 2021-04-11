/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Objectmanager.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include "Object.hpp"
#include <map>
#include "Object.hpp"
#include <string>
#include <memory>

class Objectmanager
{
public:
	Objectmanager()
	{
		objects_.clear();
	}

    bool Initialize();
    void Update(float dt);
    void Quit();
    void AddObject(Object* obj);
    void AddObject(std::shared_ptr<Object> obj);


    void RemoveObject();

    std::vector<std::shared_ptr<Object>>& GetObjectMap() { return objects_; }
    void SetCaptureObject(Object* obj);
    void SetPlayer(Object* obj);
	Object* GetCaptureCamera();

private:
	std::vector<std::shared_ptr<Object>> objects_;
	int tick = 0;
};

extern Objectmanager Objectmanager_;