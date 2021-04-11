#pragma once
#include "System.hpp"

class PhysicsManager : public System
{
	bool Initialize();
	void Update(float dt);
	void Quit();

	void collision_check();
};
