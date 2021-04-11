/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Shader.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include <string>
#include <map>
#include "vector2.hpp"
#include "vector3.hpp"
#include "affine2d.hpp"
#include "Color.hpp"

class Shader
{
public:
	static void UseShader(const Shader& shader);
	
public:
    Shader() = default;
    /*Shader& Use();*/

    void Delete();

    bool Compile(const std::string& vertex_scr,
                 const std::string& fragment_src);

    void SendUniformVariable(const std::string& name, float value);

    void SendUniformVariable(const std::string& name, int value);

    void SendUniformVariable(const std::string& name, vector2& vector);

    void SendUniformVariable(const std::string& name, vector3& vector);

    void SendUniformVariable(const std::string& name, const affine2d& affine_matrix);

    void SendUniformVariable(const std::string& name, Color color);

    int GetVertexAttributeLocation(const std::string& vertex_field_name) const;

    GLuint GetShaderHandler() const { return handle_to_shader; }

private:
	int GetUniformLocation(const std::string& name);

private:

private:
    /*void Compile_Check(GLuint object, std::string type);*/
    std::map<std::string, int> uniformNameToLocation{};
	unsigned handle_to_shader = 0;
};
