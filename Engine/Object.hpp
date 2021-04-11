/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Object.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include <vector>
#include "Component.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Sprite.hpp"
#include "Animation.hpp"
#include "Collision.hpp"
#include "Camera.hpp"
#include "RigidBody.hpp"
#include "Sound.hpp"

enum class ObjectType {
    Player,
    Wall,
    Door,
    Item_Static,
    Item_Dynamic,
    Camera,
    Background,
    Projectile,
    Font,
    Capture_Obj,
    Tile,
    Button,
    HUD_Button,
    Polaroid,
    Capture_Camera_main,
    Capture_Camera,
    Start_Pos,
    Reset_Pos,
    Trigger_Obj,
    Trigger,
    None,
    Falling_Limit,
    Obstacle,
    Bus,
    JumpIng,
}; 

enum Position
{
	X,Y,Both,Nope
};


class Object
{
public:
	Object()
	{}

	Object(const Object& obj);

    void AddComponent(Component* component);
    void AddInitComponent(Component* component);


public:
	bool IsChangeCollisionBoxScale() { return should_change_collision_box; }
	float GetCollisionBoxOffset() { return collision_box_offset; }
	bool IsXposOfCollisionOffset() { return is_x_pos_of_collision_box; }
	bool IsOffsetXpos() { return is_x_pos; }
    void SetMesh(Mesh mesh);
    void SetTranslation(const vector2& position);
    void SetScale(const vector2& scale);
    void SetRotation(const float& rotation);
    void SetDepth(const float& depth);
	void SetParent(const Transform* transform_);
	void SetIsDead(bool condition);
	void SetContainAreaCondition(bool condition);
	void SetSlowModeCondition(bool condition);
	void SetIsOutSideCondition(bool condition);
	void SetChangeCollisionBox(bool condition, float offset = 0.0f , bool is_x_pos = false)
	{
		is_x_pos_of_collision_box = is_x_pos;
		if(condition)
		{
			should_change_collision_box = condition;
			collision_box_offset = offset;
		}
		else
		{
			should_change_collision_box = condition;
			collision_box_offset = 0.0f;
		}
	}
	void SetIsChangePosition(bool condition, float offset_ =0.0f, bool is_x_pos_ = false)
	{
		if(condition == true)
		{
			is_chagne_position = condition;
			offset = offset_;
			is_x_pos = is_x_pos_;
		}
		else
		{
			is_chagne_position = false;
		}
	}

	bool Isvisible();
	bool IsContainArea() { return is_contain_area; }
	bool IsSlowMode() { return is_slowmode; }
	bool IsOutSide() { return is_outsise_capture; }

	bool IsChangePosition() { return is_chagne_position; }
	float GetChangeOffset() { return offset; }

	void SetVisible();
	void SetInvisible();

	void SetSpecificPosition(float position, bool is_xpos = false);

public:
	bool IsDead() { return isdead; }
	std::vector<Component*> GetComponent() { return m_component; }
    Mesh& GetMesh();
    Transform& GetTransform();
	ObjectType GetObjectType() {return object_type;}
	vector2 GetConstScaleSize() { return const_scale; }
    void SetObjectType(ObjectType obj_type) { object_type = obj_type; }
    void SetObjectDead() { isdead = true; }

    template <typename COMPONENT>
    COMPONENT* GetComponentByTemplate() const;

private:
	bool is_slowmode = false;
	bool is_contain_area = false;
	bool isdead = false;
	bool is_outsise_capture = true;

	bool should_change_collision_box = false;
	bool is_x_pos_of_collision_box = false;
	float collision_box_offset = 0.0f;

	bool is_chagne_position = false;
	float offset = 0.0f;
	bool is_x_pos = false;


	vector2 const_scale{};
    Mesh m_mesh{};
    Transform m_transform{};
    std::vector<Component*> m_component;
	ObjectType object_type = ObjectType::None;
};

template <typename COMPONENT>
COMPONENT* Object::GetComponentByTemplate() const
{
    for (auto i : m_component)
    {
        if (typeid(COMPONENT) == typeid(*i))
        {
            return dynamic_cast<COMPONENT*>(i);
        }
    }
    return nullptr;
}
