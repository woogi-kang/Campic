/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Particle_Generator.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include "Component.hpp"
#include "vector2.hpp"
#include "Particle.hpp"
#include <vector>

class Particle_Generator : public Component
{
public:
	Particle_Generator(int rate_ = 50, float lifeTime_ = 5.0f, float sizeVariance_ = 3.0f, float color_duration_ = 10.0f
		, vector2 startVelocity_ = { 0,0 }, vector2 randomVelocity_ = { 1,1 }, 
		vector2 particle_size_ = { 10.0f, 10.0f }, vector2 emitSize_ = { 0,0 }, std::string path_ = {}, bool IsActive_ = true)
		: emitRate(rate_), lifeTime_Control(lifeTime_), sizeVariance_Control(sizeVariance_), 
			color_duration(color_duration_), startVelocity(startVelocity_), 
				randomVelocity(randomVelocity_), particle_size(particle_size_), emitSize(emitSize_), isActive(IsActive_), path(path_)
	{
		for(int i = 0; i<emitRate; ++i)
		{
		    Particle temp{ lifeTime_Control, sizeVariance_Control, color_duration, 
		    	startVelocity, randomVelocity, particle_size, emitSize, m_type, path };
		    particles.push_back(std::make_shared<Particle>(temp));
		}
	}

	bool Initialize(Object* Ob) override;
	void Update(float dt) override;
	void Delete() override;
	std::vector<std::shared_ptr<Particle>>& GetParticles() { return particles; }

	bool IsActive() const { return isActive; }
	void ToggleActive() { isActive = !isActive; }
	void SetActiveCondition(bool condition) { isActive = condition; }

	void SetEmitRate(int rate);
    void SetPath(std::string path_) { path = path_; }
	void SetStartVelocity(vector2 velocity);
	void SetRandomVelocity(vector2 velocity);
	void ChangeSprite(const std::string path);
	void SetLifeTime(float lifeTime_);
	void SetSizeVariance(float sizeVariance_);
	void SetEmitSize(vector2 size);
	void SetParticle_Fire_Type(Particle_Fire_Type type_);
	void SetIsRepeat(bool condition) { is_repeat = condition; }
	void SetDurationTime(float duration_time_) { duration_time = duration_time_; }
    void SetIsDone(bool done) { is_done = done; }

	void AddAnimation(Component* com)
	{
		for(auto particle : particles)
		{
			particle->AddAnimation(com);
		}
	}

	bool GetIsActive() { return isActive; }
	std::string GetPath() const { return path; }
	int GetEmitRate() const { return emitRate; }
    bool GetIsDone() const { return is_done; }
	float GetLifeTimeControl() const { return lifeTime_Control; }
	float GetSizeVarianceControl() const { return sizeVariance_Control; }
	float GetColorDuration() const { return color_duration; }
	vector2 GetStartVelocity() const { return startVelocity; }
	vector2 GetRandomVelocity() const { return randomVelocity; }
	vector2 GetEmitSize() const { return emitSize; }
	vector2 GetParticleSize() { return particle_size; }
	Particle_Fire_Type GetParticle_Fire_Type() const { return m_type; }
	bool GetIsRepeat() { return is_repeat; }
	float GetDurationTime() { return duration_time; }

private:
	float inter_time=0.0f;

private:
    bool is_done = false;
    int emitRate;
	float lifeTime_Control;
	float sizeVariance_Control;
	float color_duration;
	vector2 startVelocity;
	vector2 randomVelocity;
	vector2 emitSize;
	vector2 particle_size;
	bool isActive = true;
	bool is_repeat = true;
	float duration_time = 0.0f;

	Particle_Fire_Type m_type = Particle_Fire_Type::Divergent;
	std::string path;

	std::vector<std::shared_ptr<Particle>> particles;
};