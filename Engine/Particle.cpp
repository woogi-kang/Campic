/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Particle.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#include "Particle.hpp"
#include "RandomFunction.hpp"
#include <iostream>

Particle::~Particle()
{
	delete base_obj;
}

bool Particle::Initialize(Object* Ob)
{
	if (base_obj == nullptr)
		base_obj = Ob;
	
	particle_obj->SetDepth(Ob->GetTransform().GetDepth() - 0.1f);

	SetRandomEmitSize();
	SetRandomDirection();

	float temp_color_duration = RandomNumberGenerator(0.0f, static_color_duration);
	color_duration = temp_color_duration;

	float temp_life_time = RandomNumberGenerator(0.0f, static_lifeTime);
	lifeTime = temp_life_time;

	float temp_size = RandomNumberGenerator(1.0f, static_sizeVariance);
	sizeVariance = temp_size;
	particle_obj->SetScale(particle_size * sizeVariance);

	for (auto component : particle_obj->GetComponent())
	{
		component->Initialize(particle_obj.get());
	}

	return true;
}

void Particle::Update(float dt, vector2 random_velocity_)
{
	lifeTime -= dt;

	if(lifeTime > 0.0f)
	{
		UpdateDirection(dt);

	    if(!particle_obj->GetMesh().GetColor(0).isAlphaDone)
	    {
			particle_obj->GetMesh().Decrease_Alpha(color_duration * dt);
	    }
		else
		{
			particle_obj->GetMesh().Increase_Alpha(color_duration * dt);
		}
	}
	else
	{
		isrespawn = true;
	}
}

void Particle::RespawnParticleObj(Object* obj)
{
	Initialize(obj);
    particle_obj->GetMesh().ChangeColor({ 255,255,255 });
    isrespawn = false;
    lifeTime = static_lifeTime;
}


void Particle::SetEmitSize(vector2 size)
{

}

void Particle::SetParticle_Fire_Type(Particle_Fire_Type type)
{
	m_type = type;
	Initialize(base_obj);
}

void Particle::SetRandomDirection()
{
	float temp_x_dir = 0;
	float temp_y_dir = 0;

	switch (m_type)
	{
	case Particle_Fire_Type::Divergent:
		{
			if (static_random_velocity.x == 0)
			{
				if (static_random_velocity.y > 0)
				{
					temp_x_dir = 0;
					temp_y_dir = RandomNumberGenerator(-static_random_velocity.y, static_random_velocity.y);
				}
				else
				{
					static_random_velocity.y = abs(static_random_velocity.y);
					temp_x_dir = 0;
					temp_y_dir = RandomNumberGenerator(-static_random_velocity.y, static_random_velocity.y);
				}
			}
			else if (static_random_velocity.y == 0)
			{
				if (static_random_velocity.x > 0)
				{
					temp_x_dir = RandomNumberGenerator(-static_random_velocity.x, static_random_velocity.x);
					temp_y_dir = 0;
				}
				else
				{
					static_random_velocity.x = abs(static_random_velocity.x);
					temp_x_dir = RandomNumberGenerator(-static_random_velocity.x, static_random_velocity.x);
					temp_y_dir = 0;
				}
			}
			else
			{
				if (static_random_velocity.x > 0 && static_random_velocity.y > 0)
				{
					temp_x_dir = RandomNumberGenerator(-static_random_velocity.x, static_random_velocity.x);
					temp_y_dir = RandomNumberGenerator(-static_random_velocity.y, static_random_velocity.y);
				}
				else if (static_random_velocity.x < 0)
				{
					static_random_velocity.x = abs(static_random_velocity.x);
					temp_x_dir = RandomNumberGenerator(-static_random_velocity.x, static_random_velocity.x);
					temp_y_dir = RandomNumberGenerator(-static_random_velocity.y, static_random_velocity.y);
				}
				else
				{
					static_random_velocity.y = abs(static_random_velocity.y);
					temp_x_dir = RandomNumberGenerator(-static_random_velocity.x, static_random_velocity.x);
					temp_y_dir = RandomNumberGenerator(-static_random_velocity.y, static_random_velocity.y);
				}

			}
		}
		break;
	case Particle_Fire_Type::OneWay:
		{
			if (static_random_velocity.x == 0)
			{
				if (static_random_velocity.y > 0)
				{
					temp_x_dir = 0;
					temp_y_dir = RandomNumberGenerator(0.0f, static_random_velocity.y);
				}
				else
				{
					temp_x_dir = 0;
					temp_y_dir = RandomNumberGenerator(static_random_velocity.y, 0.0f);
				}
			}
			else if (static_random_velocity.y == 0)
			{
				if (static_random_velocity.x > 0)
				{
					temp_x_dir = RandomNumberGenerator(0.0f, static_random_velocity.x);
					temp_y_dir = 0;
				}
				else
				{
					temp_x_dir = RandomNumberGenerator(static_random_velocity.x, 0.0f);
					temp_y_dir = 0;
				}
			}
			else
			{
				if (static_random_velocity.x > 0 && static_random_velocity.y > 0)
				{
					temp_x_dir = RandomNumberGenerator(0.0f, static_random_velocity.x);
					temp_y_dir = RandomNumberGenerator(0.0f, static_random_velocity.y);
				}
				else if (static_random_velocity.x < 0)
				{
					temp_x_dir = RandomNumberGenerator(static_random_velocity.x, 0.0f);
					temp_y_dir = RandomNumberGenerator(0.0f, static_random_velocity.y);
				}
				else
				{
					temp_x_dir = RandomNumberGenerator(0.0f, static_random_velocity.x);
					temp_y_dir = RandomNumberGenerator(static_random_velocity.y, 0.0f);
				}

			}
		}
		break;
	}

	random_velocity.x = temp_x_dir;
	random_velocity.y = temp_y_dir;
}

void Particle::SetRandomEmitSize()
{
	float temp_x_size;
	float temp_y_size;

	if (static_emit_size.x != 0 && static_emit_size.y != 0)
	{
		if (static_emit_size.x < 0 || static_emit_size.y < 0)
		{
			std::cout << "Should Change The Emit Size To Positive Number" << std::endl;
			if (static_emit_size.x < 0)
			{
				static_emit_size.x = abs(static_emit_size.x);
			}
			else
			{
				static_emit_size.y = abs(static_emit_size.y);
			}
		}
		temp_x_size = RandomNumberGenerator(-static_emit_size.x, static_emit_size.x);
		temp_y_size = RandomNumberGenerator(-static_emit_size.y, static_emit_size.y);
	}
	else
	{
		if(static_emit_size.x ==0)
		{
			if(static_emit_size.y < 0)
			{
				std::cout << "Should Change The Emit Size To Positive Number" << std::endl;
				static_emit_size.y = abs(static_emit_size.y);
			}
			temp_x_size = 0;
			temp_y_size = RandomNumberGenerator(-static_emit_size.y, static_emit_size.y);
		}
		else if(static_emit_size.y == 0)
		{
			if (static_emit_size.x < 0)
			{
				std::cout << "Should Change The Emit Size To Positive Number" << std::endl;
				static_emit_size.x = abs(static_emit_size.x);
			}
			temp_x_size = RandomNumberGenerator(-static_emit_size.x, static_emit_size.x);
			temp_y_size = 0;
		}
		else
		{
			temp_x_size = 0;
			temp_y_size = 0;
		}
	}

	emit_size.x = temp_x_size;
	emit_size.y = temp_y_size;

	auto temp_position = base_obj->GetTransform().GetTranslation();
	particle_obj->SetTranslation({ temp_position.x + emit_size.x , temp_position.y + emit_size.y });
}

void Particle::UpdateDirection(float dt)
{
	if(m_type == Particle_Fire_Type::Integrate)
	{
		vector2 current_pos = particle_obj->GetTransform().GetTranslation();
		vector2 diff = base_obj->GetTransform().GetTranslation() - current_pos;
		vector2 target_pos = diff * dt + current_pos;
		particle_obj->GetTransform().SetTranslation(target_pos);
	}
	else
	{
		vector2 temp_position = particle_obj->GetTransform().GetTranslation();

		particle_obj->SetTranslation({ temp_position.x + random_velocity.x + startVelocity.x,
			temp_position.y + random_velocity.y + startVelocity.y });
	}
}
