/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Shader.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/


#include <iostream>
#include "Color.hpp"
#include "Shader.hpp"
#include <cassert>
#include <assert.h>

namespace
{
	GLuint CompileShader(const char* shader_text, GLenum shader_type);
	GLuint LinkShader(GLuint vertex_handle, GLuint fragment_handle);
}

void Shader::UseShader(const Shader& shader)
{
	if (shader.handle_to_shader == 0)
		return;
	glUseProgram(shader.handle_to_shader);
}


void Shader::Delete()
{
    if (handle_to_shader)
    {
        glDeleteProgram(handle_to_shader);
    }

    handle_to_shader = 0;
}

bool Shader::Compile(const std::string& vertex_scr, const std::string& fragment_src)
{
	if (vertex_scr.empty() || fragment_src.empty())
		return false;

	Delete();
	uniformNameToLocation.clear();

	unsigned vertex_handle = CompileShader(vertex_scr.c_str(), GL_VERTEX_SHADER);
	if (vertex_handle == 0)
		return false;

	unsigned fragment_handle = CompileShader(fragment_src.c_str(), GL_FRAGMENT_SHADER);
	if (fragment_handle == 0)
	{
		glDeleteShader(vertex_handle);
		return false;
	}

	handle_to_shader = LinkShader(vertex_handle, fragment_handle);

	glDeleteShader(vertex_handle);
	glDeleteShader(fragment_handle);
	return handle_to_shader != 0;
}

void Shader::SendUniformVariable(const std::string& name, float value)
{
	int location = GetUniformLocation(name);

	if (location == -1)
	{
		std::cout << "ERROR: Location Has Error!" << std::endl;
		return;
	}

    glUniform1f(location, value);
}

void Shader::SendUniformVariable(const std::string& name, int value)
{
	int location = GetUniformLocation(name);

	if (location == -1)
	{
		std::cout << "ERROR: Location Has Error!" << std::endl;
		return;
	}

    glUniform1i(location, value);
}

void Shader::SendUniformVariable(const std::string& name, vector2& vector)
{
	int location = GetUniformLocation(name);

	if (location == -1)
	{
		std::cout << "ERROR: Location Has Error!" << std::endl;
		return;
	}

    glUniform2fv(location, 1, &vector.x);
}

void Shader::SendUniformVariable(const std::string& name, vector3& vector)
{
	int location = GetUniformLocation(name);

	if (location == -1)
	{
		std::cout << "ERROR: Location Has Error!" << std::endl;
		return;
	}

    glUniform3fv(location, 1, &vector.x);
}

void Shader::SendUniformVariable(const std::string& name, const affine2d& affine_matrix)
{
	int location = GetUniformLocation(name);

	if (location == -1)
	{
		std::cout << "ERROR: Location Has Error!" << std::endl;
		return;
	}

    glUniformMatrix3fv(location, 1, GL_TRUE, &affine_matrix.affine[0][0]);
}

void Shader::SendUniformVariable(const std::string& name, Color color)
{
	int location = GetUniformLocation(name);

	if (location == -1)
	{
		std::cout << "ERROR: Location Has Error!" << std::endl;
		return;
	}

    auto vec4 = color.RGBAfloat();

    glUniform4fv(location, 1, &vec4.x);
}


int Shader::GetVertexAttributeLocation(const std::string& vertex_field_name) const
{
    auto attriblocation = glGetAttribLocation(handle_to_shader, vertex_field_name.c_str());

    return attriblocation;
}

int Shader::GetUniformLocation(const std::string& name)
{
	const auto pair_of_name_and_location = uniformNameToLocation.find(name);
	if (pair_of_name_and_location != uniformNameToLocation.end())
		return pair_of_name_and_location->second;

	int location = -1;
	location = glGetUniformLocation(handle_to_shader, name.c_str());
	uniformNameToLocation.insert(std::make_pair(name, location));
	return location;
}

namespace
{
	GLuint CompileShader(const char* shader_text, GLenum shader_type)
	{
		GLuint shader = 0;
		GLint compile_result = GL_FALSE;

		shader = glCreateShader(shader_type);

		glShaderSource(shader, 1, &shader_text, nullptr);

		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

		if (compile_result == GL_FALSE)
		{
			std::string error_log = "Failed to compile";
			if (shader_type == GL_FRAGMENT_SHADER)
				error_log += " fragment shader:\n";
			else if (shader_type == GL_VERTEX_SHADER)
				error_log += " vertex shader:\n";
			GLint log_length = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &log_length);

			GLchar * log_message = new GLchar[log_length];
			glGetShaderInfoLog(shader, log_length, nullptr, log_message);

			std::cerr << error_log << log_message << "\n";
			assert(false);
			delete[] log_message;
		}
		return shader;
	}

	GLuint LinkShader(GLuint vertex_handle, GLuint fragment_handle)
	{
		GLint link_result = GL_FALSE;
		GLuint program = 0;
		program = glCreateProgram();

		glAttachShader(program, vertex_handle);
		glAttachShader(program, fragment_handle);

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &link_result);

		if (link_result == GL_FALSE)
		{
			GLint log_length = 0;
			glGetProgramiv(program, GL_LINK_STATUS, &log_length);

			GLchar* log_message = new GLchar[log_length];
			glGetProgramInfoLog(program, log_length, nullptr, log_message);

			std::cerr << "shader program failed to link\n" << log_message << "\n";
			assert(false);
			delete[] log_message;
		}
		return program;
	}
}
