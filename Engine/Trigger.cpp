/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Trigger.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : SoonWoo Jung
Secondary :
- End Header ----------------------------------------------------------------*/
#include "Trigger.hpp"
#include "ObjectDepth.hpp"
#include "State.hpp"
#include "Capture.hpp"
#include "Font.hpp"

bool Trigger::Initialize(Object * Ob)
{
    object = Ob;


    return false;
}

void Trigger::Update(float dt)
{
    ConnectObjectAction();
    switch (m_trigger_style)
    {
    case TriggerStyle::Door:
        MakeConnectedDoor();
        break;
    case TriggerStyle::Font:
        break;
    case TriggerStyle::CheckPoint:
    {
        if (!m_connected_object)
        {
            m_connected_object = new Object;
            m_connected_object->SetTranslation(object->GetTransform().GetTranslation());
            m_connected_object->SetScale({ 100.f,100.f });
            m_connected_object->SetMesh(mesh::CreateBox(1, { 255,255,0, 255 }));
            m_connected_object->SetDepth(GAME_OBJECT);
            m_connected_object->SetObjectType(ObjectType::Trigger_Obj);
            m_connected_object->AddInitComponent(new Animation("asset/images/Objects/SavePoint.png", "sPoint", 19, 0.075f, false));
            m_connected_object->GetComponentByTemplate<Animation>()->SetIsActive(false);
            //m_connected_object->AddComponent(new Collision());
            Objectmanager_.AddObject(m_connected_object);
        }}
        break;
    default:
        break;
    }
}

void Trigger::Delete()
{
	delete m_connected_object;
}

void Trigger::ConnectObjectAction()
{
    if (isTriggered)
    {
        switch (m_trigger_style)
        {
        case TriggerStyle::Door:
        {   if (m_connected_object)
            m_connected_object->SetIsDead(true);
        }
            break;
        case TriggerStyle::Font:
            Text();
            break;
        case TriggerStyle::Graphic:
	    GraphicPrint();
            break;
        case TriggerStyle::CheckPoint:
            CheckPoint();
            break;
        default:
            break;
        }
    }
}

void Trigger::MakeConnectedDoor()
{
    if (!m_connected_object)
    {
        m_connected_object = new Object;
        m_connected_object->SetTranslation(o_translation);
        m_connected_object->SetScale({ 75.f,75.f });
        m_connected_object->SetMesh(mesh::CreateBox(1, { 255,255,255, 255 }));
        m_connected_object->SetDepth(GAME_OBJECT);
        m_connected_object->SetObjectType(ObjectType::Trigger_Obj);
        m_connected_object->AddComponent(new Collision());
        Objectmanager_.AddObject(m_connected_object);
    }
}

void Trigger::CheckPoint()
{
    auto player_ = StateManager_.GetCurrentState()->GetPlayerObjectPointer();
    player_->GetComponentByTemplate<RigidBody>()->SetIsStopped(true);
    player_->GetComponentByTemplate<RigidBody>()->SetVelocity(0);
    player_->SetInvisible();
    m_connected_object->GetComponentByTemplate<Animation>()->SetIsActive(true);
    m_connected_object->SetDepth(GAME_OBJECT - 0.1f);

	if (m_connected_object->GetComponentByTemplate<Animation>()->IsDone())
		m_connected_object->GetComponentByTemplate<Animation>()->ResetAnimaition();


    if(m_connected_object->GetComponentByTemplate<Animation>()->IsDone())
    {
        isTriggered = false;
        object->GetComponentByTemplate<Trigger>()->GetConnectedObject()->SetDepth(GAME_OBJECT);
        player_->GetComponentByTemplate<RigidBody>()->SetIsStopped(false);
        for (auto player : Objectmanager_.GetObjectMap())
        {
            if (player.get()->GetObjectType() == ObjectType::Capture_Camera_main)
            {
                player.get()->GetComponentByTemplate<Capture>()->SetResetPosition(m_connected_object->GetTransform().GetTranslation());
            }
        }
        object->SetIsDead(true);
        player_->SetVisible();
    }
}

void Trigger::GraphicPrint()
{
    if (!m_connected_object)
    {
        m_connected_object = new Object;

        m_connected_object->SetTranslation(o_translation);
        m_connected_object->SetScale({ 200,75 });
		m_connected_object->SetMesh(mesh::CreateBox(1, { 255,255,255, 255 }));
		m_connected_object->SetDepth(GAME_OBJECT);
        m_connected_object->SetObjectType(ObjectType::Trigger_Obj);
        std::string path = "asset/images/UI/";
        m_connected_object->AddInitComponent(new Animation(path + text+".png", text, static_cast<int>(image_frame), update_frame , true));
       
        Objectmanager_.AddObject(m_connected_object);
    }
    object->SetIsDead(true);
}

void Trigger::Text()
{
    if (!m_connected_object)
    {
        m_connected_object = new Object;

        m_connected_object->SetTranslation(o_translation);
        m_connected_object->SetScale({ 2.5 });
        m_connected_object->SetObjectType(ObjectType::Trigger_Obj);
        std::wstring w_text = L"";
        w_text.assign(text.begin(), text.end());
        m_connected_object->AddInitComponent(new Font(L"asset/font/sansation.fnt", w_text));
        m_connected_object->GetComponentByTemplate<Font>()->SetFillColor(Colors::Black);
        Objectmanager_.AddObject(m_connected_object);
    }
    object->SetIsDead(true);
}
