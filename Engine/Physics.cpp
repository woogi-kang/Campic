/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Physics.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : SoonWoo Jung
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Physics.hpp"
#include "RigidBody.hpp"
#include "Collision.hpp"
#include "Player.hpp"
#include "Application.hpp"
#include "Graphics.hpp"
#include "UI.hpp"
#include "Capture.hpp"
#include "Trigger.hpp"

Physics Physics_;
Object* Physics::capture_ground_obj = nullptr;
Object* Physics::capture_left_obj = nullptr;
Object* Physics::capture_right_obj = nullptr;

Object* Physics::left_tile_obj = nullptr;
Object* Physics::right_tile_obj = nullptr;

bool Physics::Initialize()
{
	AudioManager_.LoadSFX("asset/sounds/curtain.mp3");

    return true;
}

void Physics::Update(float dt)
{
    if (!Objectmanager_.GetObjectMap().empty())
    {
        if (Objectmanager_.GetObjectMap().size() != previous_size)
        {
            capture_list.clear();
            collision_list.clear();
            projectile_list.clear();
            static_list.clear();
            dynamic_list.clear();
			checkpoint_list.clear();
            trigger_list.clear();
            doors.clear();
            obstacle_list.clear();
            limit_list = nullptr;
			for (auto obj = Objectmanager_.GetObjectMap().begin(); obj != Objectmanager_.GetObjectMap().end(); obj++)
			{

					if (auto temp = obj->get()->GetComponentByTemplate<Collision>(); temp != nullptr)
					{
						if (temp->GetisGet())
						{
							obj = Objectmanager_.GetObjectMap().erase(obj);
						}
						else if (obj->get()->GetObjectType() == ObjectType::Player)
						{
							collision_list.push_back(obj->get());
						}
						else if (obj->get()->GetObjectType() == ObjectType::Item_Dynamic)
						{
							dynamic_list.push_back(obj->get());
						}
						else if (obj->get()->GetObjectType() == ObjectType::Item_Static)
						{
							static_list.push_back(obj->get());
						}
						else if (obj->get()->GetObjectType() == ObjectType::Projectile)
						{
							projectile_list.push_back(obj->get());
						}
						else if (obj->get()->GetObjectType() == ObjectType::Capture_Obj)
						{
							capture_list.push_back(obj->get());
						}
						else if (obj->get()->GetObjectType() == ObjectType::Door)
						{
							doors.push_back(obj->get());
						}
						else if (obj->get()->GetObjectType() == ObjectType::Reset_Pos)
						{
							checkpoint_list.push_back(obj->get());
						}
						else if (obj->get()->GetObjectType() == ObjectType::Trigger)
						{
							trigger_list.push_back(obj->get());
						}
						else if (obj->get()->GetObjectType() == ObjectType::Obstacle)
						{
							obstacle_list.push_back(obj->get());
						}
					}
					else if (obj->get()->GetObjectType() == ObjectType::Falling_Limit)
					{
						limit_list = obj->get();
					}

			}
            previous_size = static_cast<int>(Objectmanager_.GetObjectMap().size());
        }
        if (collision_list.size() > 0)
        {
			for (auto obj = Objectmanager_.GetObjectMap().begin(); obj != Objectmanager_.GetObjectMap().end(); ++obj)
			{
					if (obj->get()->IsChangeCollisionBoxScale())
					{
						if (!obj->get()->IsXposOfCollisionOffset())
							obj->get()->GetComponentByTemplate<Collision>()->GetCollisionTransform().SetSpecificPosition(obj->get()->GetCollisionBoxOffset());
						else
							obj->get()->GetComponentByTemplate<Collision>()->GetCollisionTransform().SetSpecificPosition(obj->get()->GetCollisionBoxOffset(), true);

						obj->get()->SetChangeCollisionBox(false);
					}

			}

            for (int i = 0; i < (int)collision_list.size(); i++)
            {
                auto p_collision = collision_list[i]->GetComponentByTemplate<Collision>();
                auto p_rigidbody = collision_list[i]->GetComponentByTemplate<RigidBody>();
                auto p_transform = collision_list[i]->GetComponentByTemplate<Collision>()->GetCollisionTransform();

                if (limit_list) 
                {

                    if (p_transform.GetTranslation().y < limit_list->GetTransform().GetTranslation().y)
                    {
                        p_collision->ChangeCollisionBoxTranslation(StateManager_.GetCurrentState()->GetCaptureObjectPointer()->GetComponentByTemplate<Capture>()->GetResetPosition());
                        p_collision->ChangeCollisionBoxTranslation(StateManager_.GetCurrentState()->GetCaptureObjectPointer()->GetComponentByTemplate<Capture>()->GetResetPosition());
                        p_collision->ChangeCollisionBoxScale(player_scale);
                        p_collision->SetIsGround(false);

						capture_ground_obj = nullptr;
                        p_collision->SetIsCapobj(false);
                        p_rigidbody->SetVelocity(0);
                    }
                }

                tile_list.clear();
                ground_list.clear();
                TileCheck(collision_list[i]);
                GroundCheck(collision_list[i]);
                if (ground_list.size() > 0)
                {
                    for (auto ground : ground_list)
                    {
                        if (p_transform.GetTranslation().x - p_transform.GetScale().x / 2 - ground->GetTransform().GetScale().x / 2
                            <= ground->GetTransform().GetTranslation().x - 3
                            && ground->GetTransform().GetTranslation().x + 3
                            <= p_transform.GetTranslation().x + p_transform.GetScale().x / 2 + ground->GetTransform().GetScale().x / 2
                            && ground->GetTransform().GetTranslation().y + ground->GetTransform().GetScale().y / 2
                            >= p_transform.GetTranslation().y - p_transform.GetScale().y / 2 + p_rigidbody->GetVelocity().y * dt
                            && ground->GetTransform().GetTranslation().y
                            <= p_transform.GetTranslation().y - p_transform.GetScale().y / 2)
                        {
                            StopReaction(collision_list[i], ground, true);
                        }
                    }
                }
                else
                    p_collision->SetIsGround(false);
                if (tile_list.size() > 0)
                {
                    for (auto tile : tile_list)
                    {
                        if (IntersectionCheckAABBUpperCase(collision_list[i], tile))
                        {
                            if(p_rigidbody->GetVelocity().y > 0)
                            {
                                p_rigidbody->SetVelocity({ p_rigidbody->GetVelocity().x ,0 });
                            }
                            p_collision->SetIsCelling(true);
                            break;
                        }
                        p_collision->SetIsCelling(false);

                        if (IntersectionCheckAABBPositionLeft(collision_list[i], tile))
                        {
                                    p_rigidbody->SetVelocity
                                        ({0, p_rigidbody->GetVelocity().y});
                            p_collision->SetIsLeftTile(true);
							left_tile_obj = tile;
                            break;
                        }
                        p_collision->SetIsLeftTile(false);
						left_tile_obj = nullptr;

                        if (IntersectionCheckAABBPositionRight(collision_list[i], tile))
                        {
                            p_rigidbody->SetVelocity
                            ({ 0, p_rigidbody->GetVelocity().y });
                            p_collision->SetIsRightTile(true);
							right_tile_obj = tile;
                            break;
                        }
						right_tile_obj = nullptr;
                        p_collision->SetIsRightTile(false);
                    }
                }
                else
                {
                    p_collision->SetIsLeftTile(false);
					left_tile_obj = nullptr;
                    p_collision->SetIsRightTile(false);
					right_tile_obj = nullptr;
                }
                if (capture_list.size() > 0)
                {
                    for (auto capture : capture_list)
                    {
                        if (capture->GetComponentByTemplate<Collision>()->GetFilter() == Filter::Speed)
                            p_rigidbody->SetXLimited(false);
                        if (capture->GetComponentByTemplate<Collision>()->GetFilter() == Filter::Jump)
                            p_rigidbody->SetYLimited(false);
                        if (p_transform.GetTranslation().x - p_transform.GetScale().x / 2 - 2.f*capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x
                            <= capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x
                            && capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x
                            <= p_transform.GetTranslation().x + p_transform.GetScale().x / 2 + 2.f*capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x
                            && p_transform.GetTranslation().y - p_transform.GetScale().y / 2 - capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y
                            <= capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y
                            && capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y
                            < p_transform.GetTranslation().y  + capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y + p_transform.GetScale().y / 2
                            )
                        {

                            p_collision->SetIsCapobj(false);
							capture_ground_obj = nullptr;

                            p_collision->SetIsCelling(false);
                            if (IntersectionCheckAABBUpperCase(collision_list[i], capture))
                            {
                            if (p_rigidbody->GetVelocity().y > 0)
                            {
                                    p_rigidbody->SetVelocity({ p_rigidbody->GetVelocity().x ,0 });
                                }
                            p_collision->SetIsCelling(true);
                            break;
                            }
                            if (p_transform.GetTranslation().x - p_transform.GetScale().x / 2 - capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x /2
                                <= capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - 3
                                && capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + 3
                                <= p_transform.GetTranslation().x + p_transform.GetScale().x / 2 + capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x /2
                                && capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y + capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y/2
                                >= p_transform.GetTranslation().y - p_transform.GetScale().y / 2 + p_rigidbody->GetVelocity().y * dt
                                && capture->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y
                                <= p_transform.GetTranslation().y - p_transform.GetScale().y / 2 )
                            {
									capture_ground_obj = capture;
                                    p_collision->SetIsCapobj(true);
                                    if(capture->GetComponentByTemplate<Collision>()->GetFilter() == Filter::Jump)
                                        p_rigidbody->SetYLimited(true);
                                    break;
                            }
                        }
                    }
                    for (auto capture : capture_list)
                    {
                        if (IntersectionCheckAABBPositionRight(collision_list[i], capture))
                        {
                            p_rigidbody->SetVelocity
                                ({0, p_rigidbody->GetVelocity().y});
                            p_collision->SetIsRight(true);
							capture_right_obj = capture;
                            break;
                        }
                        p_collision->SetIsRight(false);
						capture_right_obj = nullptr;
                        if (IntersectionCheckAABBPositionLeft(collision_list[i], capture))
                        {
                            p_rigidbody->SetVelocity
                                ({0, p_rigidbody->GetVelocity().y});
                            p_collision->SetIsLeft(true);
							capture_left_obj = capture;
                            break;
                        }
						capture_left_obj = nullptr;
                        p_collision->SetIsLeft(false);
                    }
                }

		if (!dynamic_list.empty()) {
			for (auto dynamic_obj : dynamic_list)
			{
				if (auto collision = dynamic_obj->GetComponentByTemplate<Collision>();
					collision != nullptr)
				{
					if (IntersectionCheckAABB(collision_list[i], dynamic_obj))
					{
						collision->SetIsCollideItem(true);
					}
					else
					{
						collision->SetIsCollideItem(false);
					}
				}

			}
		}
		if (!obstacle_list.empty()) {
			for (auto obstacle_obj : obstacle_list)
			{
				if (auto collision = obstacle_obj->GetComponentByTemplate<Collision>();
					collision != nullptr)
				{
					if (IntersectionCheckAABB(collision_list[i], obstacle_obj))
					{
						AudioManager_.PlaySFX("asset/sounds/pain.mp3", 0.3f);
                        obstacle_obj->SetIsDead(true);
                        collision_list[i]->GetComponentByTemplate<Player>()->SetRelease(0.f);
                        p_rigidbody->SetVelocity(50 * normalize(collision_list[i]->GetTransform().GetTranslation() - obstacle_obj->GetTransform().GetTranslation()));


						Capture::IsChangeCaptureCount = true;
                        StateManager_.GetCurrentState()->GetCaptureLimit() -= 3;
					}
				}

			}
		}
                if(!projectile_list.empty())
                {
                    for (int j = 0; j < projectile_list.size(); j++)
                    {
                        for (int k = j; k < projectile_list.size(); k++)
                        {
                            if (j != k)
                            {
                                if (IntersectionCheckAABB(projectile_list[j], projectile_list[k]))
                                {
                                    projectile_list[k]->SetIsDead(true);
                                    projectile_list[j]->SetIsDead(true);
                                }
                            }
                        }
                        if (IntersectionCheckAABB(collision_list[i], projectile_list[j]))
                        {
                            collision_list[i]->GetTransform().SetTranslation(StateManager_.GetCurrentState()->GetStartPosition());
                            p_collision->ChangeCollisionBoxTranslation(StateManager_.GetCurrentState()->GetStartPosition());
                            p_collision->ChangeCollisionBoxScale(player_scale);
                            p_collision->SetIsGround(false);
                            p_collision->SetIsCapobj(false);
							capture_ground_obj = nullptr;

                            p_rigidbody->SetVelocity(0);
                        }
                        if (static_list.size() > 0)
                        {
                            for (auto statics : static_list)
                            {
                                if (IntersectionCheckAABB(statics, projectile_list[j]))
                                {
                                    statics->SetIsDead(true);
                                    projectile_list[j]->SetIsDead(true);
                                }
                            }
                        }
                    }
                }
		if(!checkpoint_list.empty())
		{
		        for (auto check : checkpoint_list)
		        {
			        if (IntersectionCheckAABB(collision_list[i], check))
			        {
				        check->SetIsDead(true);
				        for(auto player : Objectmanager_.GetObjectMap())
				        {
					        if(player.get()->GetObjectType() == ObjectType::Capture_Camera_main)
					        {
						        player.get()->GetComponentByTemplate<Capture>()->SetResetPosition(check->GetTransform().GetTranslation());
					        }
				        }
			        }
		        }
		}
		if(!trigger_list.empty())
		{
		        for (auto trigger : trigger_list)
		        {
						if (IntersectionCheckAABBCenterPosition(trigger, collision_list[i]))
						{
								if (trigger->GetComponentByTemplate<Trigger>()->GetIsTriggerd() == false)
								{
									trigger->GetComponentByTemplate<Trigger>()->SetIsTriggerd(true);
									if(trigger->GetComponentByTemplate<Trigger>()->GetTriggerStyle() == TriggerStyle::CheckPoint)
									{
										if (!AudioManager_.IsSFXPlaying())
										{	
											AudioManager_.SetTimer(true);
											AudioManager_.PlaySFX("asset/sounds/curtain.mp3", 0.5f);
										}
										
									}
								}
						}
		        }
		}
		if (!doors.empty())
		{
			for (auto door : doors)
			{
				if (IntersectionCheckAABB(collision_list[i], door))
				{
					to_next = true;
				}
				if (to_next) {
					p_rigidbody->SetIsStopped(true);
					p_rigidbody->SetVelocity(0);
					collision_list[i]->SetInvisible();
					time += dt;
					if (time < 2.5f)
					{
						if (!bus_object)
						{
							bus_object = new Object();
							bus_object->SetTranslation(door->GetTransform().GetTranslation());
							bus_object->SetScale({ 400, 132 });
							bus_object->SetMesh(mesh::CreateBox(1, { 0,255,255, 255 }));
							bus_object->SetDepth(-0.6f);
							bus_object->SetObjectType(ObjectType::Bus);
							bus_object->AddInitComponent(new Sprite("asset/images/Objects/Bus.png"));
							Objectmanager_.AddObject(bus_object);
							AudioManager_.PlaySFX("asset/sounds/bus_leave.wav", 0.3f);
						}
						if (time > 0.5f)
						{
							bus_object->SetTranslation({ bus_object->GetTransform().GetTranslation().x + 20 ,bus_object->GetTransform().GetTranslation().y });
							collision_list[i]->GetTransform().SetTranslation(bus_object->GetTransform().GetTranslation());
						}
					}
					else
					{
						if (door->GetComponentByTemplate<UI>()->GetId() == "LevelSelector")
						{
							StateManager_.GetCurrentState()->BackToMenu();
							time = 0;
							break;
						}
						else if (door->GetComponentByTemplate<UI>()->GetId() == "EndCutScene")
						{
							StateManager_.ToEndScene();
							time = 0;
							break;
						}
						else if (door->GetComponentByTemplate<UI>()->GetId() == "ChapterChange1")
						{
							StateManager_.ToChapChange1();
							time = 0;
							break;
						}
						else if (door->GetComponentByTemplate<UI>()->GetId() == "ChapterChange2")
						{
							StateManager_.ToChapChange2();
							time = 0;
							break;
						}
						else
						{
							bus_object = nullptr;
							time = 0;
							door->GetComponentByTemplate<UI>()->TriggerLevelLock(door->GetComponentByTemplate<UI>()->GetId());
							StateManager_.GetCurrentState()->SetLevelIndicator(door->GetComponentByTemplate<UI>()->GetId());
							StateManager_.GetCurrentState()->ChangeLevel(StateManager_.GetCurrentState()->GetLevelIndicator());
							to_next = false;
							break;
						}
					}
				}
			}
		}
            }
        }
    }

	if(auto player = StateManager_.GetCurrentState()->GetPlayerObjectPointer();
		player != nullptr)
	{
		if ((player->GetComponentByTemplate<Collision>()->GetIsRight() || player->GetComponentByTemplate<Collision>()->GetIsRightTile())
			&& (player->GetComponentByTemplate<Collision>()->GetIsLeft() || player->GetComponentByTemplate<Collision>()->GetIsLeftTile()))
			player->GetComponentByTemplate<Collision>()->SetShouldRestartPos(true);
	}
}


void Physics::Quit()
{
	collision_list.clear();
	tile_list.clear();
	ground_list.clear();
	static_list.clear();
	projectile_list.clear();
	capture_list.clear();
	dynamic_list.clear();
	checkpoint_list.clear();
	trigger_list.clear();
}

void Physics::ChangeRestitutionOfOjbect(Object object1, Object object2)
{
    if (object1.GetObjectType() == ObjectType::None
        && object2.GetObjectType() == ObjectType::Wall)
    {
        object1.GetComponentByTemplate<Collision>()->SetJumpingitutionType(stop);
        object2.GetComponentByTemplate<Collision>()->SetJumpingitutionType(none);
    }
}


bool Physics::CollisionOnAxis(vector2 Axis, std::vector<vector2> owner, std::vector<vector2> object)
{
    float minA, maxA;
    float minB, maxB;
    Interval(owner, Axis, minA, maxA);
    Interval(object, Axis, minB, maxB);

    float d0 = minA - maxB;
    float d1 = minB - maxA;

    if (d0 > 0.0f || d1 > 0.0f)
    {
        return false;
    }
    return true;
}

void Physics::Interval(std::vector<vector2> vertices, vector2 Axis, float& min, float& max)
{
    min = max = dot(vertices[0], Axis);
    for (auto vertex : vertices)
    {
        float d = dot(vertex, Axis);
        if (d < min) min = d;
        else if (d > max) max = d;
    }
}

std::vector<vector2> Physics::VectorToLine(Object object)
{
    std::vector<vector2> object_line;
    if (object.GetComponentByTemplate<Collision>()->GetCollisionType() == box_)
    {
        object_line.emplace_back(object.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[0]
            - object.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[1]);
        object_line.emplace_back(object.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[2]
            - object.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[1]);
        object_line.emplace_back(object.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[3]
            - object.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[2]);
        object_line.emplace_back(object.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[0]
            - object.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[3]);
    }
    return object_line;
}

bool Physics::IntersectionCheck(Object object1, Object object2)
{
    std::vector<vector2> owner = object1.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS();
    std::vector<vector2> object = object2.GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS();
    std::vector<vector2> axis;
    for (auto i : VectorToLine(object1))
    {
        if (!CollisionOnAxis(vector2(-i.y, i.x), owner, object))
        {
            return false;
        }
    }
    for (auto i : VectorToLine(object2))
    {
        axis.emplace_back(vector2(-i.y, i.x));
        if (!CollisionOnAxis(vector2(-i.y, i.x), owner, object))
        {
            return false;
        }
    }
    return true;
}

bool Physics::IntersectionCheckAABB(Object* object1, Object* object2)
{
    std::vector<vector2> owner = object1->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS();
    std::vector<vector2> object = object2->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS();
    if (owner[0].x >= object[0].x && owner[0].x >= object[1].x)
        return false;
    if (owner[1].x <= object[0].x && owner[1].x <= object[1].x)
        return false;
    if (owner[1].y >= object[2].y && owner[1].y >= object[1].y)
        return false;
    if (owner[2].y <= object[2].y && owner[2].y <= object[1].y)
        return false;

    return true;
}

bool Physics::IntersectionCheckAABBCenterPosition(Object * object1, Object * object2)
{
    std::vector<vector2> owner = object1->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS();
    vector2 object = object2->GetTransform().GetTranslation();
    if (owner[0].x >= object.x )
        return false;
    if (owner[1].x <= object.x )
        return false;
    if (owner[1].y >= object.y )
        return false;
    if (owner[2].y <= object.y )
        return false;

    return true;
}

bool Physics::IntersectionCheckAABBUpperCase(Object* object1, Object* object2)
{
    vector2 min_obj, max_obj, min_pos, max_pos;
    min_obj = {
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2 + 3,
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y
    };
    max_obj = {
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2 - 3,
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y + 3 + object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };

    min_pos = {
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y - object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };
    max_pos = {
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y + object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };

    if ((min_obj.x >= max_pos.x) || (max_obj.x <= min_pos.x) ||
        (min_obj.y >= max_pos.y) || (max_obj.y <= min_pos.y))
    {
        return false;
    }
    return true;
}
bool Physics::IntersectionCheckAABBPositionBase(Object* object1, Object* object2)
{
    vector2 min_obj = {
        object1->GetTransform().GetTranslation().x - object1->GetTransform().GetScale().x / 2,
        object1->GetTransform().GetTranslation().y - 2 - object1->GetTransform().GetScale().y / 2
    };
    vector2 max_obj = {
        object1->GetTransform().GetTranslation().x + object1->GetTransform().GetScale().x / 2,
        object1->GetTransform().GetTranslation().y - object1->GetTransform().GetScale().y / 2
    };

    vector2 min_pos = {
        object2->GetTransform().GetTranslation().x - object2->GetTransform().GetScale().x / 2,
        object2->GetTransform().GetTranslation().y - object2->GetTransform().GetScale().y / 2
    };
    vector2 max_pos = {
        object2->GetTransform().GetTranslation().x + object2->GetTransform().GetScale().x / 2,
        object2->GetTransform().GetTranslation().y + object2->GetTransform().GetScale().y / 2
    };

    if ((min_obj.x >= max_pos.x) || (max_obj.x <= min_pos.x) ||
        (min_obj.y >= max_pos.y) || (max_obj.y <= min_pos.y))
    {
        return false;
    }
    return true;
}

bool Physics::IntersectionCheckAABBPositionBase(vector2 pos1_min, vector2 pos1_max, vector2 pos2_min, vector2 pos2_max)
{

	if ((pos1_min.x >= pos2_max.x) || (pos1_max.x <= pos2_min.x) ||
		(pos1_min.y >= pos2_max.y) || (pos1_max.y <= pos2_min.y))
	{
		return false;
	}
	return true;
}

bool Physics::IntersectionCheckAABBPositionLeft(Object* object1, Object* object2)
{
    vector2 min_obj, max_obj, min_pos, max_pos;
    min_obj = {
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - 3 - object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y - object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2 + 3
    };
    max_obj = {
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y + object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2 - 3
    };

    min_pos = {
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y - object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };
    max_pos = {
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y + object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };

    if ((min_obj.x >= max_pos.x) || (max_obj.x <= min_pos.x) ||
        (min_obj.y >= max_pos.y) || (max_obj.y <= min_pos.y))
    {
        return false;
    }
    return true;
}

bool Physics::IntersectionCheckAABBPositionRight(Object* object1, Object* object2)
{
    vector2 min_obj, max_obj, min_pos, max_pos;
    min_obj = {
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y - object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2 + 3
    };
    max_obj = {
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + 3 + object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y + object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2 - 3
    };

    min_pos = {
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y - object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };
    max_pos = {
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y + object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };

    if ((min_obj.x >= max_pos.x) || (max_obj.x <= min_pos.x) ||
        (min_obj.y >= max_pos.y) || (max_obj.y <= min_pos.y))
    {
        return false;
    }
    return true;
}

bool Physics::IntersectionCheckNextPosition(Object* object1, Object* object2)
{
    vector2 min_obj, max_obj, min_pos, max_pos;
    min_obj = {
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y - 5 - object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };
    max_obj = {
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y - object1->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };

    min_pos = {
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x - object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y - object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };
    max_pos = {
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x + object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2,
        object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y + object2->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2
    };

    if ((min_obj.x >= max_pos.x) || (max_obj.x <= min_pos.x) ||
        (min_obj.y >= max_pos.y) || (max_obj.y <= min_pos.y))
    {
        return false;
    }
    return true;
}

bool Physics::OutOfCheckBoundary(Object* object)
{
    auto size = Graphics_.GetDisplaySize();

    if (object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x +
        object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2 < size.x / 2 &&
        object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().x -
        object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().x / 2 > -size.x / 2 &&
        object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y +
        object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2 < size.y / 2 &&
        object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation().y -
        object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale().y / 2 > -size.y / 2)
    {
        return false;
    }
    return true;
}

void Physics::TileCheck(Object * object)
{
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
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_1 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_1 }));
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_2 }));
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_0 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_0 }));
            if (Tile_Map_.GetSpecificTile({ x_1 , y_1 + TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 , y_1 + TILE_SIZE / 2 }));
            if (Tile_Map_.GetSpecificTile({ x_2 , y_1 + TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 , y_1 + TILE_SIZE / 2 }));
            if (Tile_Map_.GetSpecificTile({ x_0 , y_1 + TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_0 , y_1 + TILE_SIZE / 2 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_1 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_1 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_2 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_0 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_0 }));
        }
        else
        {
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_1 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_1 }));
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_2 }));
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_0 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_0 }));
            if (Tile_Map_.GetSpecificTile({ x_0 , y_2 - TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_0 , y_2 - TILE_SIZE / 2 }));
            if (Tile_Map_.GetSpecificTile({ x_1 , y_2 - TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 , y_2 - TILE_SIZE / 2 }));
            if (Tile_Map_.GetSpecificTile({ x_2 , y_2 - TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 , y_2 - TILE_SIZE / 2 }));
        }
    }
    else
    {
        if (object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().y > 0)
        {
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_1 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_1 }));
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_2 }));
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_0 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_0 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_1 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_1 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_2 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_0 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_0 }));
            if (Tile_Map_.GetSpecificTile({ x_0 , y_1 + TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_0 , y_1 + TILE_SIZE / 2 }));
            if (Tile_Map_.GetSpecificTile({ x_1 , y_1 + TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 , y_1 + TILE_SIZE / 2 }));
            if (Tile_Map_.GetSpecificTile({ x_2 , y_1 + TILE_SIZE / 2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 , y_1 + TILE_SIZE / 2 }));
        }
        else
        {
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_1 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_1 }));
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_2 }));
            if (Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_0 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_1 + TILE_SIZE / 2, y_0 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_1 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_1 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_0 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_0 }));
            if (Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_2 }) != nullptr)
                tile_list.push_back(Tile_Map_.GetSpecificTile({ x_2 - TILE_SIZE / 2, y_2 }));
        }
    }
}

void Physics::GroundCheck(Object * object)
{
    float x_0 = object->GetTransform().GetTranslation().x;
    float x_1 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[1].x;
    float x_2 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[0].x;
    float y_2 = object->GetComponentByTemplate<Collision>()->GetCollisionCalculateTRS()[1].y;
    if (Tile_Map_.GetSpecificTile({ x_1 , y_2 - TILE_SIZE / 2 }) != nullptr)
        ground_list.push_back(Tile_Map_.GetSpecificTile({ x_1 , y_2 - TILE_SIZE / 2 }));
    if (Tile_Map_.GetSpecificTile({ x_2 , y_2 - TILE_SIZE / 2 }) != nullptr)
        ground_list.push_back(Tile_Map_.GetSpecificTile({ x_2 , y_2 - TILE_SIZE / 2 }));
    if (Tile_Map_.GetSpecificTile({ x_0 , y_2 - TILE_SIZE / 2 }) != nullptr)
        ground_list.push_back(Tile_Map_.GetSpecificTile({ x_0 , y_2 - TILE_SIZE / 2 }));
}