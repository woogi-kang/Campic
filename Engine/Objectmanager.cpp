/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Objectmanager.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Objectmanager.hpp"
#include <algorithm>
#include "Player.hpp"
#include "Physics.hpp"
#include "Capture.hpp"
#include "State.hpp"

Objectmanager Objectmanager_;

bool Objectmanager::Initialize()
{
    if (!objects_.empty())
    {
        for (auto& obj : objects_)
        {
            for (auto& component : obj->GetComponent())
            {
                component->Initialize(obj.get());
            }
        }
    }
    return true;
}

void Objectmanager::Update(float dt)
{
	if (!StateManager_.IsPause())
	{
		auto size = objects_.size();

		for (auto object = objects_.begin(); object != objects_.end();)
		{
			if (object->get()->IsSlowMode())
			{
				if (tick % 2 == 0)
				{
					tick++;
					return;
				}
			}

			for (auto components : object->get()->GetComponent())
			{
				components->Update(dt);
			}

			if (size != objects_.size())
				break;

			SetPlayer(object->get());
			SetCaptureObject(object->get());

			if (object->get()->IsDead())
			{
				object = objects_.erase(object);
			}
			else
			{
				++object;
			}
		}
		++tick;

		if (tick >= 100)
		{
			tick = 0;
		}
	}
}

void Objectmanager::Quit()
{
	for(auto& i : objects_)
	{
		for(auto& j : i->GetComponent())
		{
			j->Delete();
		}
	}
    objects_.clear();
}

void Objectmanager::AddObject(Object* obj)
{
    std::shared_ptr<Object> temp(obj);

    objects_.push_back(temp);

    for (auto component : objects_[objects_.size() - 1]->GetComponent())
    {
        component->Initialize(objects_[objects_.size() - 1].get());
    }

    std::stable_sort(objects_.begin(), objects_.end(),
                     [](auto& obj1, auto& obj2)
                     {
                         return obj1->GetTransform().GetDepth() > obj2->GetTransform().GetDepth();
                     });
}

void Objectmanager::AddObject(std::shared_ptr<Object> obj)
{
    objects_.push_back(obj);

    for (auto component : objects_[objects_.size() - 1]->GetComponent())
    {
        component->Initialize(objects_[objects_.size() - 1].get());
    }

    std::stable_sort(objects_.begin(), objects_.end(),
                     [](auto& obj1, auto& obj2)
                     {
                         return obj1->GetTransform().GetDepth() > obj2->GetTransform().GetDepth();
                     });
}

void Objectmanager::SetCaptureObject(Object* obj)
{
    if (obj->GetComponentByTemplate<Capture>() != nullptr)
    {
        StateManager_.GetCurrentState()->SetCaptureObjectPointer(obj);
    }
}

void Objectmanager::SetPlayer(Object* obj)
{
    if (obj->GetObjectType() == ObjectType::Player)
    {
        StateManager_.GetCurrentState()->SetPlayerObjectPointer(obj);
    }
}

Object* Objectmanager::GetCaptureCamera()
{
	for(auto& obj : objects_)
	{
		if(obj->GetObjectType() == ObjectType::Capture_Camera_main)
		{
			return obj.get();
		}
	}
	return nullptr;
}

void Objectmanager::RemoveObject()
{
    objects_.clear();
}
