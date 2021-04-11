#pragma once

#include "PhysicsComponent.h"
#include "GraphicsComponent.h"

class Components
{
public:
	Components();
	~Components();

private:
	PhysicsComponent* physics{};
	GraphicsComponent* graphics{};
};

