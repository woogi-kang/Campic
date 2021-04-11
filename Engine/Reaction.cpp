/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Reaction.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : SoonWoo Jung
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Reaction.hpp"
#include "Physics.hpp"

//void Reaction(Object* object, Object* di_object,float bounce)
//{
//	if (object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::stop)
//	{
//		StopReaction(object);
//	}
//        if (di_object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::stop)
//        {
//            StopReaction(di_object);
//        }
//	else if (object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::bounce)
//	{
//		BounceReaction(object);
//	}
//	else if(object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::none)
//	{
//	}
//	if(object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::damaged)
//	{
//		if (object->GetComponentByTemplate<Collision>()->GetIsDamaged())
//		{
//			AttackedReaction(object, di_object); 
//		}
//		object->GetComponentByTemplate<Collision>()->Nohit();
//	}
//	else if (di_object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::damaged)
//	{
//		if (di_object->GetComponentByTemplate<Collision>()->GetIsDamaged())
//		{
//			AttackedReaction(di_object, object);
//		}
//		di_object->GetComponentByTemplate<Collision>()->Nohit();
//	}
//	if (object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::exit_)
//	{
//			DoorReaction(object);
//	}
//	if (di_object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::exit_)
//	{
//			DoorReaction(di_object);
//	}
//	else if (di_object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::bounce)
//	{
//		BounceReaction(di_object);
//	}
//	else if (di_object->GetComponentByTemplate<Collision>()->GetJumpingitutionType() == RestitutionType::none)
//	{
//	}
//}

void DeleteReaction(Object* object)
{
    object->GetComponentByTemplate<Collision>()->SetIsGet(true);
    Physics_.ResetPreviousSize();
}

void StaticReaction(Object* object)
{
    object->GetComponentByTemplate<RigidBody>()->SetVelocity(0);
}


void StopReaction(Object* object, Object* object_,bool ground)
{
        object->GetComponentByTemplate<RigidBody>()->SetVelocity(0);
    if (object_->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation()  
        != object->GetComponentByTemplate<Collision>()->GetCollisionTransform().GetTranslation())
    {
        object->GetComponentByTemplate<Collision>()->SetIsGround(ground);
        object->GetComponentByTemplate<RigidBody>()->SetJumping(!ground);
        object->GetTransform().SetTranslation({
                object->GetComponentByTemplate < Collision > ()->GetCollisionTransform().GetTranslation().x,
            object_->GetComponentByTemplate < Collision >()->GetCollisionTransform().GetTranslation().y 
            + object_->GetComponentByTemplate < Collision >()->GetCollisionTransform().GetScale().y / 2 
            + object->GetComponentByTemplate < Collision >()->GetCollisionTransform().GetScale().y / 2 + 1
            });
                //object->GetComponentByTemplate<RigidBody>()->GetPreviousPosition().x,
                //object->GetComponentByTemplate<RigidBody>()->GetPreviousPosition().y });
    }
}
void BounceReaction(Object *object, float bounce)
{
	object->GetComponentByTemplate<RigidBody>()->SetVelocity(-vector2
		(abs(magnitude(object->GetComponentByTemplate<RigidBody>()->GetVelocity()))
		* normalize(object->GetComponentByTemplate<RigidBody>()->GetVelocity())) * bounce);
}

void AttackedReaction(Object* object , Object* di_object, float power)
{
	object->GetComponentByTemplate<RigidBody>()->SetVelocity(-vector2(
		power * normalize(vector2(di_object->GetTransform().GetTranslation().x - object->GetTransform().GetTranslation().x
			, di_object->GetTransform().GetTranslation().y - object->GetTransform().GetTranslation().y))));
	//di_object->GetComponentByTemplate<Collision>()->ToggleIsDamaged();
}

void DisappearReaction(Object * object)
{
    for (auto obj = Objectmanager_.GetObjectMap().begin(); obj != Objectmanager_.GetObjectMap().end();)
    {
        if (obj->get() == object)
        {
           
            obj = Objectmanager_.GetObjectMap().erase(obj);
            ++obj;
        }
        else
            ++obj;
    }
}

void DoorReaction(Object * object)
{
	object->GetComponentByTemplate<Collision>()->ToggleIsDoor();
}

void CollReaction(Object* object, Object* di_object)
{
    vector2 velo1 = object->GetComponentByTemplate<RigidBody>()->GetVelocity();
    vector2 velo2 = di_object->GetComponentByTemplate<RigidBody>()->GetVelocity();
    if(magnitude(velo1) > magnitude(velo2))
    {
        object->GetComponentByTemplate<RigidBody>()->SetVelocity(velo1 - velo2);
        di_object->GetComponentByTemplate<RigidBody>()->AddVelocity(100.2f*(velo1 - velo2));
        di_object->GetComponentByTemplate<RigidBody>()->SetGravity(0);
    }
    else
    {
        di_object->GetComponentByTemplate<RigidBody>()->SetVelocity(velo2 - velo1);
        object->GetComponentByTemplate<RigidBody>()->AddVelocity(100.2f*(velo2 - velo1));
        object->GetComponentByTemplate<RigidBody>()->SetGravity(0);
    }
}

void StopReaction_dev(Object* object, Object* static_object)
{
    object->GetComponentByTemplate<RigidBody>()->SetVelocity({ 0 ,object->GetComponentByTemplate<RigidBody>()->GetVelocity().y});
        object->GetTransform().SetTranslation({
                object->GetComponentByTemplate<RigidBody>()->GetPreviousPosition().x,
                object->GetComponentByTemplate<RigidBody>()->GetPreviousPosition().y });
    //if (object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().x >
    //    object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().y)
    //{
    //    if (object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().x > 0)
    //    {
    //        object->GetTransform().SetTranslation(
    //            { static_object->GetTransform().GetTranslation().x - static_object->GetTransform().GetScale().x/2
    //                - object->GetTransform().GetScale().x/2 -1.f,
    //            object->GetTransform().GetTranslation().y });
    //    }
    //    else {
    //        object->GetTransform().SetTranslation(
    //            { object->GetTransform().GetTranslation().x ,
    //            static_object->GetTransform().GetTranslation().y + static_object->GetTransform().GetScale().y / 2
    //            + object->GetTransform().GetScale().y / 2 + 1.f });
    //        object->GetComponentByTemplate<Collision>()->SetIsGround(true);
    //    }
    //}
    //else if(object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().x <
    //    object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().y)
    //{
    //    if (object->GetComponentByTemplate<RigidBody>()->GetViewingDirection().y >= 0)
    //        object->GetTransform().SetTranslation(
    //            { object->GetTransform().GetTranslation().x ,
    //            static_object->GetTransform().GetTranslation().y - static_object->GetTransform().GetScale().y /2
    //            - object->GetTransform().GetScale().y /2 - 1.f});
    //    else
    //        object->GetTransform().SetTranslation(
    //            { static_object->GetTransform().GetTranslation().x + static_object->GetTransform().GetScale().x/2
    //                + object->GetTransform().GetScale().x/2 + 1.f,
    //            object->GetTransform().GetTranslation().y  });

    //}
       
}

void OffSetReaction(Object * object)
{
}
