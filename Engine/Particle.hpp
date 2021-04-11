/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Particle.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include "Object.hpp"
#include "Sprite.hpp"

enum class Particle_Fire_Type
{
	Divergent, Integrate, OneWay
};

class Particle
{
public:
	Particle(float lifeTime_ = 5.0f, float sizeVariance_ = 3.0f, float color_duration_ = 10.0f,
				vector2 start_velocity_ = { 0,0 }, vector2 random_velocity_ = { 0,0 }, 
		vector2 particle_size_ = { 10.0f,10.0f }, vector2 emit_size_ = { 0,0 }, Particle_Fire_Type type = Particle_Fire_Type::Divergent, std::string path = {})
		: static_lifeTime(lifeTime_), static_sizeVariance(sizeVariance_),startVelocity(start_velocity_), 
	static_color_duration(color_duration_), static_random_velocity(random_velocity_),particle_size(particle_size_), static_emit_size(emit_size_), m_type(type)
	{
		lifeTime = static_lifeTime;
		sizeVariance = static_sizeVariance;
		color_duration = static_color_duration;
		random_velocity = static_random_velocity;
		emit_size = static_emit_size;

		particle_obj = std::make_shared<Object>();
		particle_obj->SetMesh(mesh::CreateBox(1, { 255,255,255 }));
		particle_obj->SetScale(particle_size);

		if (!path.empty())
		{
			particle_obj->AddComponent(new Sprite(path));
		}

	}
	~Particle();

	bool Initialize(Object* Ob);
	void Update(float dt, vector2 random_velocity_);

	bool IsRespawn() { return isrespawn; }
	void RespawnParticleObj(Object* obj);
	std::shared_ptr<Object> GetParticleObject() { return particle_obj; }

	void SetStartVelocity(vector2 velocity) { startVelocity = velocity; }
	void SetRandomVelocity(vector2 velocity) { static_random_velocity = velocity; }
	void SetSizeVariance(float sizeVariance_) { static_sizeVariance = sizeVariance_; }
	void ChangeSprite(const std::string path) { particle_obj->GetComponentByTemplate<Sprite>()->ChangeSprite(path); }
	void SetLifeTime(float lifeTime_) { static_lifeTime = lifeTime_; }
	void SetEmitSize(vector2 size);
	void SetParticle_Fire_Type(Particle_Fire_Type type);
	void AddAnimation(Component* com)
	{
		particle_obj->AddComponent(com);
	}

	float GetLifeTime() { return static_lifeTime; }
	float GetSizeVariance() { return static_sizeVariance; }
	float GetColorDuration() { return static_color_duration; }
	vector2 GetRandomVelocity() { return static_random_velocity; }

private:
	void SetRandomDirection();
	void SetRandomEmitSize();
	void UpdateDirection(float dt);
	float lifeTime;
	float sizeVariance;
	vector2 startVelocity;
	float color_duration;
	vector2 random_velocity{};
	vector2 emit_size{};
	std::shared_ptr<Object> particle_obj = nullptr;
	bool isrespawn = false;

//static
private:
	Object* base_obj = nullptr;
	float static_lifeTime;
	float static_sizeVariance;
	float static_color_duration;
	vector2 static_random_velocity{};
	vector2 static_emit_size{};
	vector2 particle_size{};
	Particle_Fire_Type m_type;
};
