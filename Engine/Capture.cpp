/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Capture.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#include "Capture.hpp"
#include "Input.hpp"
#include "Graphics.hpp"
#include "State.hpp"
#include "HUD.hpp"
#include "Player.hpp"
#include "Physics.hpp"
#include <iostream>

bool Capture::IsChangeCaptureCount = true;

bool Capture::Initialize(Object* Ob)
{
	if (object == nullptr)
	{
		object = Ob;

		vector2 size = object->GetTransform().GetScale();
		zoomobject = new Object();
		zoomobject->SetMesh(mesh::CreateBox());
		zoomobject->SetTranslation({
		   object->GetTransform().GetTranslation().x, object->GetTransform().GetTranslation().y - size.y / 2 + 20
			});
		zoomobject->SetDepth(object->GetTransform().GetDepth());
		zoomobject->SetScale({ 260, 30 });
		zoomobject->SetObjectType(ObjectType::Capture_Camera);
		zoomobject->AddComponent(new Sprite("asset/images/Objects/ZoomLevel.png"));
		zoomobject->AddInitComponent(new Sound("asset/sounds/Camera_Capture.wav"));
		zoomobject->GetComponentByTemplate<Sound>()->AddSound("asset/sounds/Zoom_In.wav");
		zoomobject->GetComponentByTemplate<Sound>()->AddSound("asset/sounds/Zoom_Out.wav");
		temp_local_bar_scale = zoomobject->GetTransform().GetScale();

		zoombutton = new Object();
		zoombutton->SetTranslation({object->GetTransform().GetTranslation().x + 2.5f,
			object->GetTransform().GetTranslation().y - size.y / 2 + 35});
		zoombutton->SetMesh(mesh::CreateBox());
		zoombutton->SetDepth(object->GetTransform().GetDepth() - 0.05f);
		zoombutton->SetScale({ 25, 30 });
		zoombutton->SetObjectType(ObjectType::Capture_Camera);
		zoombutton->AddComponent(new Sprite("asset/images/Objects/ZoomDial.png"));

		zoomobject->SetParent(&object->GetTransform());
		zoombutton->SetParent(&object->GetTransform());

		Objectmanager_.AddObject(zoomobject);
		Objectmanager_.AddObject(zoombutton);
	}

	const_zoom = zoom;
	zoom_min_value = 0.5f;
	zoom_max_value = 2.5f;
	return true;
}

void Capture::Update(float dt)
{
	if (player == nullptr)
	{
		player = StateManager_.GetCurrentState()->GetPlayerObjectPointer();
	}
	else
	{
		if (player->GetComponentByTemplate<Collision>()->GetShouldRestartPos())
		{
			player->SetTranslation(reset_pos);
			player->GetComponentByTemplate<Collision>()->SetShouldRestartPos(false);
			return;
		}
	}

	//std::cout << Input::GetMousePos();

	CameraZoom();
	ZoomObjectUpdate(dt);
	CaptureObjectMove();

	if (player != nullptr)
	{
		CameraZoomInOut();
		if (IsCaptureArea())
		{
			if (StateManager_.GetCurrentState()->GetCaptureLimit() > 0)
			{
				if (Input::IsMouseTriggered(GLFW_MOUSE_BUTTON_LEFT))
				{
					IsChangeCaptureCount = true;
					cheese = true;
					Capturing();
					CreatePolaroidObject();
					CreateCaptureObject();
					zoomobject->GetComponentByTemplate<Sound>()->Play("asset/sounds/Camera_Capture.wav", 0.4f);
				}
			}
		}
		SetOrigianlSize();
		SlowMode();
	}
}

void Capture::Delete()
{
	delete zoombutton;
	delete zoomobject;
	delete player;
	delete ground_object;

	original_scale.clear();
	capture_area_contian_object.clear();
	temporary_obj_storage.clear();
	polaroid_object.clear();
}

void Capture::SetVisibleCaptureObj()
{
	object->GetMesh().Visible();
	zoomobject->GetMesh().Visible();
	zoombutton->GetMesh().Visible();
	isvisible = true;
}

void Capture::SetInvisibleCaptureObj()
{
	object->GetMesh().Invisible();
	zoomobject->GetMesh().Invisible();
	zoombutton->GetMesh().Invisible();
	isvisible = false;
}

void Capture::CaptureObjectMove()
{
	vector2 mouse_pos = Input::GetMousePos();
	Player* player = StateManager_.GetCurrentState()->GetPlayerObjectPointer()->GetComponentByTemplate<Player>();

	vector2 check = object->GetTransform().GetTranslation();

	if (player->GetMouseOffset().x != 0 || player->GetMouseOffset().y != 0)
	{
		object->SetTranslation(mouse_pos + player->GetMouseOffset());
	}
	else
	{
		object->SetTranslation(mouse_pos);
	}

	if (player->GetMouseOffset().x != 0 || player->GetMouseOffset().y != 0)
	{
		if (abs(check.x - object->GetTransform().GetTranslation().x) > 50 || abs(
			check.y - object->GetTransform().GetTranslation().y) > 50)
		{
			object->GetTransform().SetTranslation(check);
		}
	}
}

void Capture::SetZoomMaxMin(float max, float min)
{
	if (min != 0.0f)
		zoom_min_value = min;

	zoom_max_value = max;
}

bool Capture::IsCaptureArea()
{
	bool result = false;
	capture_area_contian_object.clear();
	original_scale.clear();

	vector2 object_pos = object->GetTransform().GetTranslation();
	vector2 object_size = object->GetTransform().GetScale() / 2;
	vector2 min_pos = { object_pos.x - object_size.x, object_pos.y - object_size.y };
	vector2 max_pos = { object_pos.x + object_size.x, object_pos.y + object_size.y };

	if (auto obj_collision = player->GetComponentByTemplate<Collision>();
		obj_collision != nullptr)
	{
		vector2 save_min_obj = { obj_collision->GetCollisionTransform().GetTranslation().x - obj_collision->GetCollisionTransform().GetScale().x / 2.0f,
		obj_collision->GetCollisionTransform().GetTranslation().y - obj_collision->GetCollisionTransform().GetScale().y / 2.0f };

		vector2 save_max_obj = { obj_collision->GetCollisionTransform().GetTranslation().x + obj_collision->GetCollisionTransform().GetScale().x / 2.0f,
		obj_collision->GetCollisionTransform().GetTranslation().y + obj_collision->GetCollisionTransform().GetScale().y / 2.0f };

		if (player != nullptr)
		{
			if ((save_min_obj.x >= min_pos.x) && (save_max_obj.x <= max_pos.x) &&
				(save_min_obj.y >= min_pos.y) && (save_max_obj.y <= max_pos.y))
			{
				if (player->GetObjectType() != ObjectType::Player)
				{
					capture_area_contian_object.push_back(player);

					player->SetContainAreaCondition(true);
					player->SetIsOutSideCondition(false);

					result = true;
				}
				else
				{
					vector2 reset_min;
					vector2 reset_max;

					if (auto collsion = player->GetComponentByTemplate<Collision>();
						collsion != nullptr)
					{
						reset_min = {
						   reset_pos.x - collsion->GetCollisionTransform().GetScale().x,
						   reset_pos.y - collsion->GetCollisionTransform().GetScale().y
						};

						reset_max = {
						   reset_pos.x + collsion->GetCollisionTransform().GetScale().x,
						   reset_pos.y + collsion->GetCollisionTransform().GetScale().y
						};
					}

					if ((save_max_obj.x >= reset_max.x) || (save_min_obj.x <= reset_min.x) ||
						(save_max_obj.y >= reset_max.y) || (save_min_obj.y <= reset_min.y))
					{
						capture_area_contian_object.push_back(player);
					}
					player->SetContainAreaCondition(true);
					player->SetIsOutSideCondition(false);
					result = true;
				}
			}
			else if ((min_pos.x >= save_max_obj.x) || (min_pos.y >= save_max_obj.y) ||
				(max_pos.x <= save_min_obj.x) || (max_pos.y <= save_min_obj.y))
			{
				player->SetSlowModeCondition(false);
				player->SetIsOutSideCondition(true);
				player->SetIsChangePosition(false);
				player->SetContainAreaCondition(false);
			}
			original_scale.push_back(std::make_pair(player->GetConstScaleSize(), player));
		}
	}
	return result;
}

void Capture::Capturing()
{
	for (auto obj : capture_area_contian_object)
	{
		vector2 reset_max = reset_pos + vector2{ 75,75 };
		vector2 reset_min = reset_pos - vector2{ 75,75 };
		vector2 max = obj->GetTransform().GetTranslation() + obj->GetTransform().GetScale() / 2.0f;
		vector2 min = obj->GetTransform().GetTranslation() - obj->GetTransform().GetScale() / 2.0f;

		if (Physics_.IntersectionCheckAABBPositionBase(reset_min, reset_max, min, max))
		{
			continue;
		}

		--StateManager_.GetCurrentState()->GetCaptureLimit();
		Object* temp = new Object(*obj);
		temp->GetComponentByTemplate<RigidBody>()->SetGravity(0);

		if (!temp->Isvisible())
			temp->SetVisible();

		if (zoom < 1.0f)
		{
			temp->GetComponentByTemplate<Collision>()->ChangeCollisionBoxScale(
				obj->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetScale());
		}
		else
		{
			temp->GetComponentByTemplate<Collision>()->ChangeCollisionBoxScale(
				obj->GetComponentByTemplate<Collision>()->GetConstCollisionScale() * zoom);
		}

		temp->GetComponentByTemplate<RigidBody>()->SetVelocity(0);
		temp->GetComponentByTemplate<Collision>()->ChangeCollisionBoxTranslation(temp->GetTransform().GetTranslation());

		if (m_c_filter == Filter::Jump)
		{
			temp->GetComponentByTemplate<Collision>()->SetFilter(Filter::Jump);
			temp->GetMesh().ChangeColor({ 0, 0, 255, 255 });
		}
		if (m_c_filter == Filter::Speed)
		{
			temp->GetComponentByTemplate<Collision>()->SetFilter(Filter::Speed);
			temp->GetMesh().ChangeColor({ 0, 255, 0, 255 });
		}

		temp->SetObjectType(ObjectType::Capture_Obj);

		if (obj->GetObjectType() == ObjectType::Projectile)
		{
			obj->SetIsDead(true);
		}

		if (auto temp_animation = temp->GetComponentByTemplate<Animation>();
			temp_animation != nullptr)
		{
			temp_animation->SetIsActive(false);
		}

		if (auto temp_collision = temp->GetComponentByTemplate<Collision>();
			temp_collision != nullptr)
		{
			temp_collision->SetIsGround(false);
		}

		if (obj->GetObjectType() == ObjectType::Player)
		{
			player->SetTranslation(reset_pos);
			player->GetComponentByTemplate<Collision>()->ChangeCollisionBoxTranslation(reset_pos);
			player->GetComponentByTemplate<Collision>()->ChangeCollisionBoxScale(player_scale);
			player->GetComponentByTemplate<RigidBody>()->SetVelocity(0);
			player->GetComponentByTemplate<Collision>()->SetIsGround(false);
			player->GetComponentByTemplate<Collision>()->SetIsCapobj(false);
			Physics::capture_ground_obj = nullptr;

		}

		temporary_obj_storage.push_back(std::make_pair(temp, obj->GetTransform().GetScale()));
		iscreate = true;
	}
}

void Capture::CreateCaptureObject()
{
	if (!temporary_obj_storage.empty())
	{
		for (auto obj : temporary_obj_storage)
		{
			Objectmanager_.AddObject(obj.first);
		}
	}

	if (!polaroid_object.empty())
	{
		for (auto obj : polaroid_object)
		{
			Objectmanager_.AddObject(obj->GetObject());
		}
	}
	temporary_obj_storage.clear();
	polaroid_object.clear();
}

void Capture::CreatePolaroidObject()
{
	if (iscreate)
	{
		for (auto obj : temporary_obj_storage)
		{
			Polaroid* temp = new Polaroid(obj.first, obj.second);
			polaroid_object.push_back(temp);
		}
		iscreate = false;
	}
}

void Capture::CameraZoom()
{
	float zoom_ = static_cast<float>(Input::MouseWheelScroll());

	temp_zoom = zoom_;

	if (zoom_ != 0)
	{
		if (zoom == 1.0f && zoom_ > 0)
		{
			zoomobject->GetComponentByTemplate<Sound>()->Play("asset/sounds/Zoom_In.wav", 0.4f);
			float temp = zoom_max_value - const_zoom;
			float t = temp / 3.0f;
			zoom += t;
		}
		else if (zoom == 1.0f && zoom_ < 0)
		{
			zoomobject->GetComponentByTemplate<Sound>()->Play("asset/sounds/Zoom_In.wav", 0.2f);
			float temp = 1.0f - zoom_min_value;
			float t = temp / 3.0f;
			zoom -= t;
		}
		else
		{
			if (zoom_ > 0)
			{
				if (zoom > 1.0f)
				{
					zoomobject->GetComponentByTemplate<Sound>()->Play("asset/sounds/Zoom_In.wav", 0.2f);
					float temp = zoom_max_value - const_zoom;
					float t = temp / 3.0f;
					zoom += t;
				}
				else
				{
					zoomobject->GetComponentByTemplate<Sound>()->Play("asset/sounds/Zoom_In.wav", 0.2f);
					float temp = 1.0f - zoom_min_value;
					float t = temp / 3.0f;
					zoom += t;
				}

				if (zoom > zoom_max_value)
				{
					zoom = zoom_max_value;
				}
			}
			else if (zoom_ < 0)
			{
				if (zoom > 1.0f)
				{
					zoomobject->GetComponentByTemplate<Sound>()->Play("asset/sounds/Zoom_In.wav", 0.2f);
					float temp = zoom_max_value - const_zoom;
					float t = temp / 3.0f;
					zoom -= t;
				}
				else
				{
					zoomobject->GetComponentByTemplate<Sound>()->Play("asset/sounds/Zoom_In.wav", 0.2f);
					float temp = 1.0f - zoom_min_value;
					float t = temp / 3.0f;
					zoom -= t;
				}

				if (zoom < zoom_min_value)
				{
					zoom = zoom_min_value;
				}
			}
		}
	}
}

void Capture::CameraZoomInOut()
{
	ground_object = nullptr;

	auto ground_objects = Physics_.GroundCheck_return(player);

	if (!ground_objects.empty())
	{
		ground_object = ground_objects[0];

		for (auto ground_obj : ground_objects)
		{
			if (ground_obj->GetTransform().GetTranslation().y > ground_object->GetTransform().GetTranslation().y)
				ground_object = ground_obj;
		}
	}

	if (ground_object != nullptr)
		save_ground_obj = ground_object;

	if (!player->IsOutSide())
	{
		player->GetTransform().SetScale(player->GetConstScaleSize() * zoom);

		if (ground_object != nullptr)
			save_ground_obj = ground_object;

		if (auto temp_collision = player->GetComponentByTemplate<Collision>();
			temp_collision != nullptr)
		{
			float y_pos = 0.0f;
			if(temp_collision->GetIsGround() || temp_collision->GetIsCapobj())
			{			
				if (Physics::capture_ground_obj != nullptr)
				{
					if(auto c_ground_collision = Physics::capture_ground_obj->GetComponentByTemplate<Collision>();
						c_ground_collision != nullptr)
					{
						y_pos = c_ground_collision->GetCollisionTransform().GetTranslation().y
							+ c_ground_collision->GetCollisionTransform().GetScale().y / 2.0f;
					}
				}
				else if (ground_object != nullptr)
				{
					y_pos = ground_object->GetTransform().GetTranslation().y + ground_object->GetTransform().GetScale().y / 2.0f;
				}

				if (std::abs(y_pos - (temp_collision->GetCollisionTransform().GetTranslation().y - temp_collision->GetCollisionTransform().GetScale().y / 2.0f)) < 5.0f)
				{
					bool is_change_x_pos = false;
					if (zoom > 1.0f)
					{
						is_change_x_pos = false;
						temp_collision->SetCollisionScale(temp_collision->GetConstCollisionScale());
						temp_collision->SetIsFollow(false, y_pos + temp_collision->GetConstCollisionScale().y/ 2.0f);
						player->SetIsChangePosition(true, y_pos + player->GetTransform().GetScale().y / 2.0f - 8.0f);
					}
					else if (zoom == 1.0f)
					{
						temp_collision->SetCollisionScale(temp_collision->GetConstCollisionScale());
						temp_collision->SetIsFollow(false, y_pos + temp_collision->GetConstCollisionScale().y / 2.0f);
						player->SetIsChangePosition(false);

						if (temp_zoom > 0)
							is_change_x_pos = true;
					}
					else if (zoom < 1.0f)
					{
						temp_collision->SetCollisionScale(temp_collision->GetConstCollisionScale() * zoom);
						temp_collision->SetIsFollow(true);
						player->SetSpecificPosition(y_pos + temp_collision->GetCollisionTransform().GetScale().y / 2.0f + 1.0f);
						player->SetIsChangePosition(false);

						if (temp_zoom > 0)
							is_change_x_pos = true;
					}

					if(is_change_x_pos)
					{
						if (Physics::capture_left_obj != nullptr)
						{
							if(temp_collision->GetIsLeft())
							{
								if(auto c_left_collision = Physics::capture_left_obj->GetComponentByTemplate<Collision>();
									c_left_collision != nullptr)
								{
									float x_pos = c_left_collision->GetCollisionTransform().GetTranslation().x
										+ c_left_collision->GetCollisionTransform().GetScale().x / 2.0f;
									player->SetIsChangePosition(true, x_pos + temp_collision->GetCollisionTransform().GetScale().x / 2.0f, true);
									player->SetChangeCollisionBox(true, x_pos + temp_collision->GetCollisionTransform().GetScale().x / 2.0f, true);
									
								}
							}
						}
						else if (Physics::capture_right_obj != nullptr)
						{
							if (temp_collision->GetIsRight())
							{
								if (auto c_right_collision = Physics::capture_right_obj->GetComponentByTemplate<Collision>();
									c_right_collision != nullptr)
								{
									float x_pos = c_right_collision->GetCollisionTransform().GetTranslation().x
										- c_right_collision->GetCollisionTransform().GetScale().x / 2.0f;
									player->SetIsChangePosition(true, x_pos - temp_collision->GetCollisionTransform().GetScale().x / 2.0f, true);
									player->SetChangeCollisionBox(true, x_pos - temp_collision->GetCollisionTransform().GetScale().x / 2.0f, true);								
								}
							}
						}
						else if (Physics::left_tile_obj != nullptr)
						{
							if (temp_collision->GetIsLeftTile())
							{
								float x_pos = Physics::left_tile_obj->GetTransform().GetTranslation().x + Physics::left_tile_obj->GetTransform().GetScale().x / 2.0f;
								player->SetIsChangePosition(true, x_pos + temp_collision->GetCollisionTransform().GetScale().x / 2.0f + 2.0f, true);
								player->SetChangeCollisionBox(true, x_pos + temp_collision->GetCollisionTransform().GetScale().x / 2.0f + 2.0f, true);
							}
						}
						else if (Physics::right_tile_obj != nullptr)
						{
							if (temp_collision->GetIsRightTile())
							{
								float x_pos = Physics::right_tile_obj->GetTransform().GetTranslation().x - Physics::right_tile_obj->GetTransform().GetScale().x / 2.0f;
								player->SetIsChangePosition(true, x_pos - temp_collision->GetCollisionTransform().GetScale().x / 2.0f - 2.0f, true);
								player->SetChangeCollisionBox(true, x_pos - temp_collision->GetCollisionTransform().GetScale().x / 2.0f - 2.0f, true);

							}
						}
						else
						{
							player->SetChangeCollisionBox(false);
						}
					}
				}
			}
			else
			{
				temp_collision->SetIsFollow(true);
				player->SetIsChangePosition(false);

				if (zoom > 1.0f)
				{
					temp_collision->SetCollisionScale(temp_collision->GetConstCollisionScale());
				}
				else if (zoom == 1.0f)
				{
					temp_collision->SetCollisionScale(temp_collision->GetConstCollisionScale());
				}
				else if (zoom < 1.0f)
				{
					temp_collision->SetCollisionScale(temp_collision->GetConstCollisionScale() * zoom);
				}
			}
		}
	}
}

void Capture::SetOrigianlSize()
{
	for (auto obj : original_scale)
	{
		if (obj.second->IsOutSide())
		{
			obj.second->SetScale(obj.second->GetConstScaleSize());

			if(auto temp_collision = obj.second->GetComponentByTemplate<Collision>();
				temp_collision != nullptr)
			{
				float y_pos = 0.0f;

				if(temp_collision->GetIsGround() || temp_collision->GetIsCapobj())
				{
					if(Physics::capture_ground_obj != nullptr)
					{
						if (auto c_ground_collision = Physics::capture_ground_obj->GetComponentByTemplate<Collision>();
							c_ground_collision != nullptr)
						{
							y_pos = c_ground_collision->GetCollisionTransform().GetTranslation().y
								+ c_ground_collision->GetCollisionTransform().GetScale().y / 2.0f
								+ temp_collision->GetConstCollisionScale().y / 2.0f;
						}
					}
					if (Physics::capture_left_obj != nullptr)
					{
						if (auto c_left_collision = Physics::capture_left_obj->GetComponentByTemplate<Collision>();
							c_left_collision != nullptr)
						{
							float x_pos = c_left_collision->GetCollisionTransform().GetTranslation().x
								+ c_left_collision->GetCollisionTransform().GetScale().x / 2.0f
								+ temp_collision->GetConstCollisionScale().x / 2.0f + 4.0f;

							obj.second->SetSpecificPosition(x_pos, true);
							obj.second->SetChangeCollisionBox(true, x_pos, true);
							obj.second->SetIsChangePosition(false);
						}
					}
					else if (Physics::capture_right_obj != nullptr)
					{
						if (auto c_right_collision = Physics::capture_right_obj->GetComponentByTemplate<Collision>();
							c_right_collision != nullptr)
						{
							float x_pos = c_right_collision->GetCollisionTransform().GetTranslation().x
								- c_right_collision->GetCollisionTransform().GetScale().x / 2.0f
								- temp_collision->GetConstCollisionScale().x / 2.0f - 4.0f;
							obj.second->SetSpecificPosition(x_pos, true);
							obj.second->SetChangeCollisionBox(true, x_pos, true);
							obj.second->SetIsChangePosition(false);
						}
					}
					else if (temp_collision->GetIsLeftTile())
					{
						float x_pos = Physics::left_tile_obj->GetTransform().GetTranslation().x
							+ Physics::left_tile_obj->GetTransform().GetScale().x / 2.0f
							+ temp_collision->GetConstCollisionScale().x / 2.0f;

						obj.second->SetSpecificPosition(x_pos, true);
						obj.second->SetChangeCollisionBox(true, x_pos, true);
						obj.second->SetIsChangePosition(false);
					}
					else if (temp_collision->GetIsRightTile())
					{
						float x_pos = Physics::right_tile_obj->GetTransform().GetTranslation().x
							- Physics::right_tile_obj->GetTransform().GetScale().x / 2.0f
							- temp_collision->GetConstCollisionScale().x / 2.0f;
						obj.second->SetSpecificPosition(x_pos, true);
						obj.second->SetChangeCollisionBox(true, x_pos, true);
						obj.second->SetIsChangePosition(false);

					}

					if(temp_collision->GetIsCapobj())
					{
						obj.second->SetSpecificPosition(y_pos);
						obj.second->SetChangeCollisionBox(true, y_pos);
						obj.second->SetIsChangePosition(false);
					}
				}
				else
				{
					obj.second->SetSpecificPosition(obj.second->GetTransform().GetTranslation().y);
					obj.second->SetChangeCollisionBox(false);
					obj.second->SetIsChangePosition(false);
					temp_collision->SetIsFollow(true, 0);

				}
				temp_collision->SetCollisionScale(temp_collision->GetConstCollisionScale());

			}
		}
	}
}

void Capture::ZoomObjectUpdate(float dt)
{
	float t = 0.0f;
	float half_scale = temp_local_bar_scale.x / 2.0f;
	float temp_scale = 0.0f;

	if (zoom > 1.0f)
	{
		t = (zoom - 1.0f) / (zoom_max_value - 1.0f);
		temp_scale = half_scale * t;
		zoombutton->SetSpecificPosition((2.5f + temp_scale) / object->GetTransform().GetScale().x, true);
	}
	else if (zoom < 1.0f)
	{
		t = (zoom - zoom_min_value) / (1.0f - zoom_min_value);
		temp_scale = half_scale * t;
		zoombutton->SetSpecificPosition((2.5f - (half_scale - temp_scale)) / object->GetTransform().GetScale().x, true);
	}
	else if (zoom == 1.0f)
	{
		zoombutton->SetSpecificPosition(2.5f / object->GetTransform().GetScale().x, true);
	}
}


void Capture::SlowMode()
{
	for (auto obj : capture_area_contian_object)
	{
		if (!obj->IsOutSide())
		{
			if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
			{
				obj->SetSlowModeCondition(true);
			}
			if (Input::IsMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
			{
				obj->SetSlowModeCondition(false);
			}
		}
	}
}