/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Physics.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : SoonWoo Jung
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include "Objectmanager.hpp"
#include "Collision.hpp"
#include "Reaction.hpp"
#include "Tile_Map.hpp"
#include <iostream>

class Physics
{
public:
	bool Initialize();
	void Update(float dt);

	static Object* capture_ground_obj;
	static Object* capture_left_obj;
	static Object* capture_right_obj;

	static Object* left_tile_obj;
	static Object* right_tile_obj;

	bool IsNextLevel() { return to_next; }

	//void PhysicsObjectUpdate(Objectmanager* objectmanager);
	std::vector<Object*> tile_list;
	void Quit();
	void ChangeRestitutionOfOjbect(Object object1, Object object2);

	// This function is used by intersection check. You do not use it for collision check.
	bool CollisionOnAxis(vector2 Axis, std::vector<vector2> owner, std::vector<vector2> object);
	// This function is used by upper function (CollisionOnAxis). It take interval between vertices. 
	void Interval(std::vector<vector2> vertices, vector2 Axis, float& min, float& max);
	// This function convert vector to line.
	std::vector<vector2> VectorToLine(Object object);
	// This is real intersection check which you use.
	bool IntersectionCheck(Object object1, Object object2);
	// This is intersection check which is AABB version for thin 
	bool IntersectionCheckAABB(Object* object1, Object* object2);
	bool IntersectionCheckAABBCenterPosition(Object* object1, Object* object2);
	bool IntersectionCheckAABBPositionBase(Object* object1, Object* object2);
	bool IntersectionCheckAABBPositionLeft(Object* object1, Object* object2);
	bool IntersectionCheckAABBPositionRight(Object* object1, Object* object2);
	bool IntersectionCheckNextPosition(Object* object1, Object* object2);
	bool IntersectionCheckAABBUpperCase(Object* object1, Object* object2);

	void SetNextLevel(bool condition)
	{
		to_next = condition;
	}
	void ResetPreviousSize() { previous_size = 0; }
	// This function return true if object is out of window area. 
	bool OutOfCheckBoundary(Object* object);


	bool TransparentCheck(Object* object)
	{
		float value = dot(vector2(0, 1), object->GetComponentByTemplate<RigidBody>()->GetViewingDirection());
		if (value > 0)
		{
			return true;
		}
		return false;
	}

	void TileCheck(Object* object);
	void GroundCheck(Object* object);

	bool IntersectionCheckAABBPositionBase(vector2 pos1_min, vector2 pos1_max, vector2 pos2_min, vector2 pos2_max);

	std::vector<Object*> TileCheck_return(Object* object)
	{
		std::vector<Object*> temp;
		float x_0 = object->GetTransform().GetTranslation().x;
		float y_0 = object->GetTransform().GetTranslation().y;
		float x_1 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[1].x;
		float x_2 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[0].x;
		float y_1 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[2].y;
		float y_2 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[1].y;
		if (object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().x > 0)
		{
			if (object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().y > 0)
			{
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_1}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_1}));
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_2}));
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_0}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_0}));
				if (Tile_Map_.GetSpecificTile({x_1, y_1 + TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1, y_1 + TILE_SIZE / 2}));
				if (Tile_Map_.GetSpecificTile({x_2, y_1 + TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2, y_1 + TILE_SIZE / 2}));
				if (Tile_Map_.GetSpecificTile({x_0, y_1 + TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_0, y_1 + TILE_SIZE / 2}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_1}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_1}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_2}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_0}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_0}));
			}
			else
			{
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_1}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_1}));
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_2}));
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_0}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_0}));
				if (Tile_Map_.GetSpecificTile({x_0, y_2 - TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_0, y_2 - TILE_SIZE / 2}));
				if (Tile_Map_.GetSpecificTile({x_1, y_2 - TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1, y_2 - TILE_SIZE / 2}));
				if (Tile_Map_.GetSpecificTile({x_2, y_2 - TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2, y_2 - TILE_SIZE / 2}));
			}
		}
		else
		{
			if (object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().y > 0)
			{
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_1}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_1}));
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_2}));
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_0}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_0}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_1}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_1}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_2}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_0}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_0}));
				if (Tile_Map_.GetSpecificTile({x_0, y_1 + TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_0, y_1 + TILE_SIZE / 2}));
				if (Tile_Map_.GetSpecificTile({x_1, y_1 + TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1, y_1 + TILE_SIZE / 2}));
				if (Tile_Map_.GetSpecificTile({x_2, y_1 + TILE_SIZE / 2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2, y_1 + TILE_SIZE / 2}));
			}
			else
			{
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_1}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_1}));
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_2}));
				if (Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_0}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_1 + TILE_SIZE / 2, y_0}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_1}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_1}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_0}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_0}));
				if (Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_2}) != nullptr)
					temp.push_back(Tile_Map_.GetSpecificTile({x_2 - TILE_SIZE / 2, y_2}));
			}
		}
		return temp;
	}

	std::vector<Object*> Return_Capture_Ground_Collision(Object* obj)
	{
		std::vector<Object*> temp;
		Collision* collision = obj->GetComponentByTemplate<Collision>();

		if (collision->GetIsCapobj())
		{
			float min = collision->GetCollisionTransform().GetTranslation().y - collision->GetCollisionTransform().GetScale().y / 2.0f;

			for (auto& obj_ : Objectmanager_.GetObjectMap())
			{
				if (obj_->GetObjectType() == ObjectType::Capture_Obj)
				{
					if (obj_->GetTransform().GetTranslation().y < min)
						temp.push_back(obj_.get());
				}
			}
		}

		return temp;
	}

	std::vector<Object*> Return_Ground_Collision(Object* obj)
	{
		std::vector<Object*> temp;
		Collision* collision = obj->GetComponentByTemplate<Collision>();

		float min = collision->GetCollisionTransform().GetTranslation().y - collision->GetCollisionTransform().GetScale().y / 2.0f;

		for (auto tile : Tile_Map_.GetPhysicalTiles())
		{
			if (tile.second->GetTransform().GetTranslation().y < min)
			{
				temp.push_back(tile.second);
			}
		}

		return temp;
	}

	std::vector<Object*> Return_Right_Collision(Object* obj)
	{
		std::vector<Object*> temp;
		Collision* collision = obj->GetComponentByTemplate<Collision>();

		float max = collision->GetCollisionTransform().GetTranslation().x + collision->GetCollisionTransform().GetScale().x / 2.0f;

		for (auto tile : Tile_Map_.GetPhysicalTiles())
		{
			if (tile.second->GetTransform().GetTranslation().x > max)
			{
				temp.push_back(tile.second);
			}
		}
		return temp;
	}

	std::vector<Object*> Return_Left_Collision(Object* obj)
	{
		std::vector<Object*> temp;
		Collision* collision = obj->GetComponentByTemplate<Collision>();

		float min = collision->GetCollisionTransform().GetTranslation().x - collision->GetCollisionTransform().GetScale().x / 2.0f;

		for (auto tile : Tile_Map_.GetPhysicalTiles())
		{
			if(tile.second->GetTransform().GetTranslation().x < min)
			{
				temp.push_back(tile.second);
			}
		}
		return temp;
	}


	std::vector<Object*> GroundCheck_return(Object* object)
	{
		std::vector<Object*> temp;
		float x_0 = object->GetTransform().GetTranslation().x;
		float x_1 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[1].x;
		float x_2 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[0].x;
		float y_2 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[1].y;
		for (int i = 1; i < 10; i += 2)
		{
			if (Tile_Map_.GetSpecificTile({x_1, y_2 - i * TILE_SIZE / 2}) != nullptr)
				temp.push_back(Tile_Map_.GetSpecificTile({x_1, y_2 - i * TILE_SIZE / 2}));
			if (Tile_Map_.GetSpecificTile({x_2, y_2 - i * TILE_SIZE / 2}) != nullptr)
				temp.push_back(Tile_Map_.GetSpecificTile({x_2, y_2 - i * TILE_SIZE / 2}));
			if (Tile_Map_.GetSpecificTile({x_0, y_2 - i * TILE_SIZE / 2}) != nullptr)
				temp.push_back(Tile_Map_.GetSpecificTile({x_0, y_2 - i * TILE_SIZE / 2}));
		}
		return temp;
	}


private:
	std::vector<Object*> collision_list;
	std::vector<Object*> ground_list;
	std::vector<Object*> static_list;
	std::vector<Object*> projectile_list;
	std::vector<Object*> capture_list;
	std::vector<Object*> dynamic_list;
	std::vector<Object*> checkpoint_list;
	std::vector<Object*> trigger_list;
        vector2 reset_position = 0;
        Object* limit_list = nullptr;
        Object* bus_object = nullptr;
        std::vector<Object*> doors;
        std::vector<Object*> obstacle_list;
    //Object* door = nullptr;
        int previous_size = 0;
        float time = 0;
        bool to_next = false;
    //vector2 windowsize = vector2{ 1280, 960 };

};

extern Physics Physics_;
