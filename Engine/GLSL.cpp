/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : GLSL.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "GLSL.hpp"

namespace GLSL
{
	const std::string shapes_vertex =
		R"(
	#version 330

	layout(location = 0) in vec2 position;

	uniform mat3 transform;
	uniform float depth;

	void main()
	{
	    vec3 position = transform * vec3(position, 1.0f);
	    gl_Position = vec4(position.xy, depth, 1.0);
	}
	)";

	const std::string shapes_fragment =
		R"(
	#version 330

	uniform vec4 color;
	out vec4 output_color;

	void main()
	{
	    output_color = color;
	}
	)";

    const std::string vertex =
		R"(
#version 330

layout(location = 1) in vec2 position;
layout(location = 2) in vec2 texture_coordinate;

uniform mat3 transform;
uniform float depth;

out vec2 interpolated_texture_coordinate;

void main()
{
    vec3 position = transform * vec3(position, 1.0f);
    gl_Position = vec4(position.xy, depth, 1.0);
    interpolated_texture_coordinate = texture_coordinate;
}
)";

    const std::string fragment =
		R"(
#version 330

in vec2 interpolated_texture_coordinate;

uniform vec4 color;
uniform sampler2D texture_to_sample;

out vec4 output_color;

void main()
{
    vec4 texel = texture(texture_to_sample, interpolated_texture_coordinate);
    vec4 new_color = color * texel;

	if(new_color.a <= 0.1)
			discard;

    output_color = new_color;
}
)";

	const std::string particle_vertex =
		R"(
	#version 330 core

	layout (location = 3) in vec2 position;
	layout (location = 4) in vec2 texture_coordinate;
	
	uniform mat3 transform;
	uniform float depth;

	out vec2 _texture_coordinate;

	void main()
	{
		vec3 position = transform * vec3(position, 1.0f);
		gl_Position = vec4(position.xy, depth, 1.0);
		_texture_coordinate = texture_coordinate;
	}
	)";

	const std::string particle_fragment =
		R"(
	#version 330 core

	in vec2 _texture_coordinate;

	uniform vec4 color;
	uniform sampler2D texture_to_sample;

	out vec4 output_color;

	void main()
	{
		vec4 texel = texture(texture_to_sample, _texture_coordinate);
		vec4 result_color = color * texel;

		if(result_color.a <= 0.1)
			discard;

		output_color = result_color;
	}
	)";

	const std::string font_vertex =
		R"(
	#version 330

	layout(location = 5) in vec2 position;
	layout(location = 6) in vec2 texture_coordinate;

	uniform mat3 transform;
	uniform float depth;

	out vec2 interpolated_texture_coordinate;

	void main()
	{
	    vec3 position = transform * vec3(position, 1.0);
	    gl_Position = vec4(position.xy, depth, 1.0);
	    interpolated_texture_coordinate = texture_coordinate;
	}
	)";
	;
	const std::string font_fragment =
		R"(
	#version 330

	in vec2 interpolated_texture_coordinate;
	out vec4 output_color;

	uniform vec4 color;
	uniform sampler2D texture_to_sample;

	void main()
	{
		vec4 texel = texture(texture_to_sample, interpolated_texture_coordinate);
	    vec4 new_color = color * texel;

		if(new_color.a <= 0.1)
				discard;

		output_color = new_color;
	}
	)";
}
