/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Color.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Color.hpp"

Color::Color(ColorChannel red, ColorChannel green, ColorChannel blue, ColorChannel alpha)
    : Red(red), Green(green), Blue(blue), Alpha(alpha)
{
}

vector4 Color::RGBAfloat() const
{
    return vector4{Red / 255.0f, Green / 255.0f, Blue / 255.0f, Alpha / 255.0f};
}

namespace Colors
{
	const Color Black(0, 0, 0);
	const Color White(255, 255, 255);
	const Color Red(255, 0, 0);
	const Color Green(0, 255, 0);
	const Color Blue(0, 0, 255);
	const Color Yellow(255, 255, 0);
	const Color Magenta(255, 0, 255);
	const Color Cyan(0, 255, 255);
	const Color Transparent(0, 0, 0, 0);
}