/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : DepthValue.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once

namespace depth
{
	constexpr float NearDepth = -0.9f;
	constexpr float MiddleDepth = 0;
	constexpr float FarDepth = 0.9f;
	constexpr float ToNearPlane = -1.0f;
}