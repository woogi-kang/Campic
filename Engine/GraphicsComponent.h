#pragma once

#include "Texture.hpp"
#include "Image.hpp"

class GraphicsComponent
{
public:
	GraphicsComponent();
	~GraphicsComponent();

private:
	Texture* texture{};
	Image* image{};
};

