/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Button.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : SoonWoo Jung
Secondary :
- End Header ----------------------------------------------------------------*/
#include "Button.hpp"

Button button_;

void Button::AddObject(Object* obj)
{
    std::shared_ptr<Object> temp(obj);

    m_button_container.push_back(temp);

    for (auto component : m_button_container[m_button_container.size() - 1]->GetComponent())
    {
        component->Initialize(m_button_container[m_button_container.size() - 1].get());
    }
}

void Button::AddObject(std::shared_ptr<Object> obj)
{
    m_button_container.push_back(obj);

    for (auto component : m_button_container[m_button_container.size() - 1]->GetComponent())
    {
        component->Initialize(m_button_container[m_button_container.size() - 1].get());
    }
}

bool Button::IntersectionCheck(vector2 mouse_pos)
{
    for(auto i : m_button_container)
    {
        if (i.get()->GetTransform().GetTranslation().x - i.get()->GetTransform().GetScale().x / 2 < mouse_pos.x
            && i.get()->GetTransform().GetTranslation().x + i.get()->GetTransform().GetScale().x / 2 > mouse_pos.x
            && i.get()->GetTransform().GetTranslation().y - i.get()->GetTransform().GetScale().y / 2 < mouse_pos.y
            && i.get()->GetTransform().GetTranslation().y + i.get()->GetTransform().GetScale().y / 2 > mouse_pos.y)
        {
			if (m_selected_object.second == CollisionState::NotCollided)
			{
				m_selected_object.first = i.get();
				m_selected_object.second = CollisionState::Collided;
			}
            return true;
        }
        else
        {
            m_selected_object.first = nullptr;
            m_selected_object.second = CollisionState::NotCollided;
        }
    }
    return false;
}
