/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Player.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Player.hpp"
#include "Input.hpp"
#include "RigidBody.hpp"
#include <iostream>
#include "Graphics.hpp"
#include "control_angle.hpp"

bool Player::Initialize(Object * Ob)
{
	if (object == nullptr)
	{
		object = Ob;
		object->SetScale({ 80.0f, 80.f });
		object->SetMesh(mesh::CreateBox(1, { 255,255,255, 255 }));
		object->SetDepth(GAME_OBJECT-0.1f);
		object->SetObjectType(ObjectType::Player);
		object->AddInitComponent(new Animation("asset/images/Player/PlayerDynamic.png", "player", 7, 0.05f, true));
		object->AddInitComponent(new RigidBody());
        object->AddInitComponent(new Collision(box_, player_scale));
		object->AddComponent(new Sound("asset/sounds/jump.mp3", AudioManager::CATEGORY_SFX,0.3f));
		object->AddComponent(new Sound("asset/sounds/spring.mp3", AudioManager::CATEGORY_SFX,0.3f));
		object->AddComponent(new Sound("asset/sounds/break.mp3", AudioManager::CATEGORY_SFX,0.3f));
		object->GetComponentByTemplate<Animation>()->AddAnimaition("asset/images/Player/PlayerShutter.png", "capture", 10, 0.01f, false);
		object->GetComponentByTemplate<Animation>()->AddAnimaition("asset/images/Player/PlayerJump.png", "jump", 1, 0.01f, false);
                //object->SetInvisible();
	}
	return true;
}

void Player::Update(float dt)
{
#ifdef _DEBUG
#else 
    /*if (auto camera = Graphics_.GetCurrentCamera();
        camera != nullptr)
    {
        camera->SetCenter(object->GetTransform().GetTranslation());
    }*/

	MouseTest();
#endif
        if (m_release_time < 1.f)
        {
            if (!bus)
            {
                object->GetComponentByTemplate<RigidBody>()->SetIsStopped(true);
                m_release_time += dt;
                m_twinkle = !m_twinkle;
            }
            else {
                m_release_time += dt;
                if (!bus_object)
                {
                    bus_object = new Object();
                    bus_object->SetTranslation({ object->GetTransform().GetTranslation().x - 300,object->GetTransform().GetTranslation().y });
                    bus_object->SetScale({ 400, 132 });
                    bus_object->SetMesh(mesh::CreateBox(1, { 0,255,255, 255 }));
                    bus_object->SetDepth(-0.6f);
                    bus_object->SetObjectType(ObjectType::Bus);
                    bus_object->AddInitComponent(new Sprite("asset/images/Objects/Bus.png"));
                    object->GetComponentByTemplate<Sound>()->Play("asset/sounds/break.mp3", 0.3f);
                    //AudioManager_.PlaySFX("asset/sounds/Bus.mp3",1.f);
                    Objectmanager_.AddObject(bus_object);
                }
                else {
                    object->SetInvisible();
                    if (m_release_time < 0.7f)
                    {
                        bus_object->SetTranslation({ bus_object->GetTransform().GetTranslation().x + 10 ,bus_object->GetTransform().GetTranslation().y });
                    }
                }
            }
        }
        else
        {
            bus = false;
            object->SetVisible();
        }
        if (!bus)
        {
            if (m_twinkle)
                object->SetInvisible();
            else
                object->SetVisible();
        }

        if (m_release_time >= 1.0f) {
            bus_object->SetInvisible();
            object->GetComponentByTemplate<RigidBody>()->SetIsStopped(false);
            m_twinkle = false;
        }
}

void Player::Delete()
{
}

void Player::MovePlayer()
{
    auto o_collision = object->GetComponentByTemplate<Collision>();
    auto o_rigidbody = object->GetComponentByTemplate<RigidBody>();
    auto o_animation = object->GetComponentByTemplate<Animation>();
    auto playe_c = object->GetComponentByTemplate<Player>();

    if (!o_collision->GetIsRight() && o_collision->GetIsLeft() ||
        (!o_collision->GetIsRightTile() && o_collision->GetIsLeftTile()))
    {
        if (Input::IsKeyPressed(GLFW_KEY_D))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(false);
            o_rigidbody->SetVelocity({
                o_rigidbody->GetVelocityLimit().x,
                o_rigidbody->GetVelocity().y
                });
            o_rigidbody->SetKeyDPress(true);
        }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(true);
            o_rigidbody->SetVelocity({
                0,
                o_rigidbody->GetVelocity().y
                });
            o_rigidbody->SetKeyAPress(true);
        }
        if (o_rigidbody->GetJumping() == false)
        {
            if (Input::IsKeyPressed(GLFW_KEY_W))
            {
                o_collision->SetIsGround(false);
                o_collision->SetIsCapobj(false);
                o_collision->SetIsLeftTile(true);
                o_rigidbody->SetVelocity({ o_rigidbody->GetVelocity().x,o_rigidbody->GetVelocityLimit().y });
                o_rigidbody->SetJumping(true);
                object->GetComponentByTemplate<Sound>()->Play("asset/sounds/jump.mp3", 0.4f);
                o_rigidbody->SetYLimited(false);
            }
        }
    }
    else if (o_collision->GetIsRight() && !o_collision->GetIsLeft()
        || (o_collision->GetIsRightTile() && !o_collision->GetIsLeftTile()))
    {
        if (Input::IsKeyPressed(GLFW_KEY_D))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(false);
            o_rigidbody->SetVelocity({
                0,
                o_rigidbody->GetVelocity().y
                });
            o_rigidbody->SetKeyDPress(true);
        }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(true);
            o_rigidbody->SetVelocity({
                -o_rigidbody->GetVelocityLimit().x,
                o_rigidbody->GetVelocity().y
                });
            o_rigidbody->SetKeyAPress(true);
        }
        if (o_rigidbody->GetJumping() == false)
        {
            if (Input::IsKeyPressed(GLFW_KEY_W))
            {
                o_collision->SetIsGround(false);
                o_collision->SetIsCapobj(false);
                o_collision->SetIsRightTile(true);
                o_rigidbody->SetVelocity(
                    { o_rigidbody->GetVelocity().x, o_rigidbody->GetVelocityLimit().y });
                o_rigidbody->SetJumping(true);
                object->GetComponentByTemplate<Sound>()->Play("asset/sounds/jump.mp3", 0.4f);
                o_rigidbody->SetYLimited(false);
            }
        }
    }
    else if ((!o_collision->GetIsRight() && !o_collision->GetIsLeft())
        && (!o_collision->GetIsRightTile() && !o_collision->GetIsLeftTile()))
    {
        if (Input::IsKeyPressed(GLFW_KEY_D))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(false);
            o_rigidbody->SetVelocity({
                o_rigidbody->GetVelocityLimit().x,
                o_rigidbody->GetVelocity().y
                });
            o_rigidbody->SetKeyDPress(true);
        }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(true);
            o_rigidbody->SetVelocity({
                -o_rigidbody->GetVelocityLimit().x,
                o_rigidbody->GetVelocity().y
                });
            o_rigidbody->SetKeyAPress(true);
        }
        if (o_rigidbody->GetJumping() == false)
        {
            if (Input::IsKeyPressed(GLFW_KEY_W))
            {
                o_collision->SetIsGround(false);
                o_collision->SetIsCapobj(false);
                o_rigidbody->SetVelocity(
                    { o_rigidbody->GetVelocity().x, o_rigidbody->GetVelocityLimit().y });
                o_rigidbody->SetJumping(true);
                object->GetComponentByTemplate<Sound>()->Play("asset/sounds/jump.mp3", 0.4f);
                o_rigidbody->SetYLimited(false);
            }
        }
    }
    if (Input::IsKeyReleased(GLFW_KEY_A))
    {
            o_rigidbody->SetKeyAPress(false);
    }
    if (Input::IsKeyReleased(GLFW_KEY_D))
    {
        o_rigidbody->SetKeyDPress(false);
    }
    if (!o_rigidbody->GetKeyAPress() && !o_rigidbody->GetKeyDPress())
    {
        o_rigidbody->SetVelocity(
            { o_rigidbody->GetVelocity().x * o_rigidbody->GetFriction(), o_rigidbody->GetVelocity().y });
        playe_c->SetIsMove(false);
    }

    if (o_rigidbody->GetKeyAPress() || o_rigidbody->GetKeyDPress())
    {
        playe_c->SetIsMove(true);
    }

    if (Input::IsKeyTriggered(GLFW_KEY_A))
    {
        o_animation->SetFlip(true);
    }

    if (Input::IsKeyTriggered(GLFW_KEY_D))
    {
        o_animation->SetFlip(false);
    }

    //if (Input::IsKeyTriggered(GLFW_KEY_W))
    //{
    //	o_animation->ChangeAnimation("jump");
    //}

    //if(Input::IsMouseTriggered(GLFW_MOUSE_BUTTON_LEFT))
    //{
    //	o_animation->ChangeAnimation("capture");
    //}
}

void Player::MouseTest()
{
	vector2 current_mouse_pos = Input::GetMousePos();

	if (auto camera = Graphics_.GetCurrentCamera();
		camera != nullptr)
	{
		if (prev_mouse_pos != current_mouse_pos)
		{
			offset = {};
			save_pos = camera->GetCenter();
			prev_mouse_pos = current_mouse_pos;
		}
		else
		{
			offset = camera->GetCenter() - save_pos;
		}
	}
}
