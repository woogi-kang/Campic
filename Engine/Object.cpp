/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Object.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Object.hpp"
#include "control_angle.hpp"
#include <iostream>


Object::Object(const Object& obj)
{
	m_transform = obj.m_transform;
	m_mesh = obj.m_mesh;
	object_type = obj.object_type;

	for(auto compoent : obj.m_component)
	{
		if(auto temp_animation = obj.GetComponentByTemplate<Animation>();
			(temp_animation != nullptr) && (compoent == temp_animation))
		{
			m_component.push_back(new Animation(*temp_animation));
		}
		else if(auto temp_collision = obj.GetComponentByTemplate<Collision>();
			(temp_collision != nullptr) && (compoent == temp_collision))
		{
			m_component.push_back(new Collision(temp_collision->GetCollisionType()));
		}
		else if(auto temp_rigidbody = obj.GetComponentByTemplate<RigidBody>();
			(temp_rigidbody != nullptr) && (temp_rigidbody == compoent))
		{
			m_component.push_back(new RigidBody());
		}
		else if(auto temp_sprite = obj.GetComponentByTemplate<Sprite>();
			(temp_sprite != nullptr) && (temp_sprite == compoent))
		{
			m_component.push_back(new Sprite(temp_sprite->GetPath()));
		}
	}
}


void Object::AddComponent(Component* component)
{
    m_component.push_back(component);
}

void Object::AddInitComponent(Component* component)
{
    component->Initialize(this);
    m_component.push_back(component);
}

void Object::SetMesh(Mesh mesh)
{
    m_mesh = mesh;
}

void Object::SetTranslation(const vector2& position)
{
	m_transform.SetTranslation(position);
}

void Object::SetScale(const vector2& scale)
{
	if (m_transform.GetScale().x == 1.0f && m_transform.GetScale().y == 1.0f)
		const_scale = scale;

	m_transform.SetScale(scale);

}

void Object::SetRotation(const float& rotation)
{
	m_transform.SetRotation(rotation);
}

void Object::SetDepth(const float& depth)
{
	m_transform.SetDepth(depth);
}

void Object::SetParent(const Transform* transform_)
{
	m_transform.SetParent(transform_);

	auto temp = m_transform.GetWorldToModel();
	affine2d rotate_ = rotation_affine(-m_transform.GetRotation());

	temp *= rotate_;

	m_transform.SetTranslation({ temp.affine[0][2], temp.affine[1][2] });
	m_transform.SetScale({ temp.affine[0][0], temp.affine[1][1] });
}

void Object::SetIsDead(bool condition)
{
	isdead = condition;
}

void Object::SetContainAreaCondition(bool condition)
{
	is_contain_area = condition;
}

void Object::SetSlowModeCondition(bool condition)
{
	is_slowmode = condition;
}

void Object::SetIsOutSideCondition(bool condition)
{
	is_outsise_capture = condition;
}


bool Object::Isvisible()
{
	return m_mesh.IsVisible();
}

void Object::SetVisible()
{
	m_mesh.Visible();
}

void Object::SetInvisible()
{
	m_mesh.Invisible();
}

void Object::SetSpecificPosition(float position, bool is_xpos)
{
	vector2 temp = m_transform.GetTranslation();
	if (is_xpos)
	{
		temp.x = position;
		m_transform.SetTranslation(temp);
	}
	else
	{
		temp.y = position;
		m_transform.SetTranslation(temp);
	}
}

Mesh& Object::GetMesh()
{
    return m_mesh;
}

Transform& Object::GetTransform()
{
    return m_transform;
}

