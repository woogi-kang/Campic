#pragma once

#include "Components.h"
#include <string>
#include <list>

class GameObject
{
public:
 
	GameObject();
	~GameObject();

private:
	Components* components;

private:
	std::string name;
};


