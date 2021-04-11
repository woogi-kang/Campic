/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : RigidBody.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JungSoon Woo
Secondary :
- End Header ----------------------------------------------------------------*/

#include "RigidBody.hpp"
#include "Player.hpp"
#include <iostream>
#include "Input.hpp"
#include "Physics.hpp"

bool RigidBody::Initialize(Object* Ob)
{
    if (object == nullptr)
    {
        object = Ob;
        m_previous_position = object->GetTransform().GetTranslation();
        m_force_accumlator = {0, 0};
        m_velocity = {0, 0};
        m_viewing_direction = {0, 0};
        m_next_position = {0, 0};
        isJumping = false;
        if (object->GetObjectType() == ObjectType::Player)
            isPlayer = true;
    }
    m_velocity = {0, 0};
    return true;
}

void RigidBody::Update(float dt)
{
    if(Input::IsKeyTriggered(GLFW_KEY_KP_8))
    {
        m_velocity_limit.x += 50;
    }
    if (Input::IsKeyTriggered(GLFW_KEY_KP_5))
    {
        m_velocity_limit.x -= 50;
    }
    if (isPlayer)
    {
        if(!isStopped)
		MovePlayer();
        if (m_velocity.x > m_velocity_limit.x)
            m_velocity.x = m_velocity_limit.x;
        if (m_velocity.x < -m_velocity_limit.x)
            m_velocity.x = -m_velocity_limit.x;
        //if (m_velocity.y > m_velocity_limit.y)
        //    m_velocity.y = m_velocity_limit.y;
        if (m_velocity.y < -400)
            m_velocity.y = -400;
    }
    // for stop reaction
    m_previous_position = object->GetTransform().GetTranslation();

    // normalized velocity.
    m_viewing_direction = normalize(m_velocity);

    // calculate current velocity.
    m_velocity += m_inverse_mass * (m_force_accumlator * dt);

    // zero out accumulated force
    m_force_accumlator = {0, 0};

    //m_friction always activated
    //m_velocity *= m_friction;

    // integrate position
    //if (magnitude(m_velocity) < 0.001f)
    //    m_velocity = 0;
    // integrate position                   
    if (!object->GetComponentByTemplate<Collision>()->GetIsGround() && !object
                                                                        ->GetComponentByTemplate<Collision>()->
                                                                        GetIsCapobj())
    {
        m_velocity.y -= m_gravity;
        object->GetTransform().SetTranslation({
            (object->GetTransform().GetTranslation().x + (m_velocity * m_slowmode * dt).x),
            (object->GetTransform().GetTranslation().y + (m_velocity * m_slowmode * dt).y)
        });
    }
    else
    {
        object->GetTransform().SetTranslation({
            (object->GetTransform().GetTranslation().x + (m_velocity * m_slowmode * dt).x),
            (object->GetTransform().GetTranslation().y)
        });
        isJumping = false;
    }
    if (!object->GetComponentByTemplate<Collision>()->GetIsGround())
        m_next_position = {
            (object->GetTransform().GetTranslation().x + (m_velocity * m_slowmode * dt).x),
            (object->GetTransform().GetTranslation().y + (m_velocity * m_slowmode * dt).y)
        };
    else
        m_next_position = {
            (object->GetTransform().GetTranslation().x + (m_velocity * m_slowmode * dt).x),
            (object->GetTransform().GetTranslation().y)
        };
    if (isYLimited)
        m_velocity_limit.y = 800;
    else
        m_velocity_limit.y = 400;
    if (isXLimited)
        m_velocity_limit.x = 500;
    else
        m_velocity_limit.x = 200;
}

void RigidBody::Delete()
{
}

void RigidBody::MovePlayer()
{
    auto o_collision = object->GetComponentByTemplate<Collision>();
    auto o_rigidbody = object->GetComponentByTemplate<RigidBody>();
	auto o_animation = object->GetComponentByTemplate<Animation>();
	auto playe_c = object->GetComponentByTemplate<Player>();

    if (!o_collision->GetIsRight() && o_collision->GetIsLeft() || 
        (!o_collision->GetIsRightTile() &&  o_collision->GetIsLeftTile()))
    {
        if (Input::IsKeyPressed(GLFW_KEY_D))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(false);
            o_rigidbody->SetVelocity({
                m_velocity_limit.x,
                o_rigidbody->GetVelocity().y
            });
            key_press_d = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(true);
            o_rigidbody->SetVelocity({
                0,
                o_rigidbody->GetVelocity().y
            });
            key_press_a = true;
        }
        if (o_rigidbody->GetJumping() == false)
        {
            if (isYLimited)
            {
                if (Input::IsKeyPressed(GLFW_KEY_W))
                {
                    object->GetComponentByTemplate<Sound>()->Play("asset/sounds/spring.mp3", 0.4f);
                    o_collision->SetIsGround(false);
                    o_collision->SetIsCapobj(false);
                    Physics::capture_ground_obj = nullptr;

                    o_collision->SetIsLeftTile(true);
                    o_rigidbody->SetVelocity({ o_rigidbody->GetVelocity().x,m_velocity_limit.y });
                    o_rigidbody->SetJumping(true);
                    isYLimited = false;
                }
            }
            else{

                if (Input::IsKeyTriggered(GLFW_KEY_W))
                {
                    o_collision->SetIsGround(false);
                    o_collision->SetIsCapobj(false);
                    Physics::capture_ground_obj = nullptr;

                    o_rigidbody->SetVelocity(
                        { o_rigidbody->GetVelocity().x, m_velocity_limit.y });
                    o_rigidbody->SetJumping(true);
                    object->GetComponentByTemplate<Sound>()->Play("asset/sounds/jump.mp3", 0.4f);
                }
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
            key_press_d = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(true);
            o_rigidbody->SetVelocity({
                -m_velocity_limit.x,
                o_rigidbody->GetVelocity().y
            });
            key_press_a = true;
        }
        if (o_rigidbody->GetJumping() == false)
        {
            if (isYLimited)
            {
            if (Input::IsKeyPressed(GLFW_KEY_W))
            {
                object->GetComponentByTemplate<Sound>()->Play("asset/sounds/spring.mp3", 0.4f);
                o_collision->SetIsGround(false);
                o_collision->SetIsCapobj(false);
                Physics::capture_ground_obj = nullptr;

                o_collision->SetIsRightTile(true);
                o_rigidbody->SetVelocity(
                    { o_rigidbody->GetVelocity().x, m_velocity_limit .y});
                o_rigidbody->SetJumping(true);
                isYLimited = false;
            }
            }
            else{
                if (Input::IsKeyTriggered(GLFW_KEY_W))
                {
                    o_collision->SetIsGround(false);
                    o_collision->SetIsCapobj(false);
                    Physics::capture_ground_obj = nullptr;

                    o_rigidbody->SetVelocity(
                        { o_rigidbody->GetVelocity().x, m_velocity_limit.y });
                    o_rigidbody->SetJumping(true);
                    object->GetComponentByTemplate<Sound>()->Play("asset/sounds/jump.mp3", 0.4f);
                }
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
                m_velocity_limit.x,
                o_rigidbody->GetVelocity().y
            });
            key_press_d = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            object->GetComponentByTemplate<Animation>()->SetFlip(true);
            o_rigidbody->SetVelocity({
                -m_velocity_limit.x,
                o_rigidbody->GetVelocity().y
            });
            key_press_a = true;
        }
        if (o_rigidbody->GetJumping() == false)
        {
            if (isYLimited)
            {
            if (Input::IsKeyPressed(GLFW_KEY_W))
            {
                    object->GetComponentByTemplate<Sound>()->Play("asset/sounds/spring.mp3", 0.4f);
                o_collision->SetIsGround(false);
                o_collision->SetIsCapobj(false);
                Physics::capture_ground_obj = nullptr;

                o_rigidbody->SetVelocity(
                    { o_rigidbody->GetVelocity().x, m_velocity_limit .y});
                o_rigidbody->SetJumping(true);

                isYLimited = false;
            }
            }
            else
            {
                if (Input::IsKeyTriggered(GLFW_KEY_W))
                {
                    o_collision->SetIsGround(false);
                    o_collision->SetIsCapobj(false);
                    Physics::capture_ground_obj = nullptr;

                    o_rigidbody->SetVelocity(
                        { o_rigidbody->GetVelocity().x, m_velocity_limit.y });
                    o_rigidbody->SetJumping(true);
                    object->GetComponentByTemplate<Sound>()->Play("asset/sounds/jump.mp3", 0.4f);
                }
            }
        }
    }
	if (Input::IsKeyReleased(GLFW_KEY_A))
	{
		key_press_a = false;
	}
	if (Input::IsKeyReleased(GLFW_KEY_D))
	{
		key_press_d = false;
	}
    if (!key_press_a && !key_press_d)
    {
        o_rigidbody->SetVelocity(
            { o_rigidbody->GetVelocity().x * m_friction, o_rigidbody->GetVelocity().y });
		playe_c->SetIsMove(false);
    }

	if(key_press_a || key_press_d)
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
