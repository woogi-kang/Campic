/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Font.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#include "BitmapFont.hpp"
#include "Font.hpp"
#include "Object.hpp"

namespace
{
	void AddCharQuadToMesh(Mesh& mesh, BitmapFont::Character character, vector2 image_dimensions,
		vector2 current_position, float line_height);
}

Font::Font(const std::wstring& fnt_filepath, std::wstring text_string)
{
	font = new BitmapFont(fnt_filepath);
	needNewMeshes = true;
	string = text_string;
	BuildNewMeshesIfNeeded();
}

Font::Font(std::wstring text_string,  BitmapFont& text_font)
	: string(std::move(text_string)), font(&text_font)
{}

bool Font::Initialize(Object* Ob)
{
	if(object == nullptr)
	{
		object = Ob;
	}

	return true;
}

void Font::Update(float dt)
{
	BuildNewMeshesIfNeeded();
}

void Font::Delete()
{
	meshes.clear();
	string.clear();
	delete font;
	delete[] sprite;
}


std::wstring Font::GetString() const
{
	return string;
}

void Font::SetString(const std::wstring& text_string)
{
	if (string != text_string)
	{
		string = text_string;
		needNewMeshes = true;
	}
}

 BitmapFont* Font::GetFont() 
{
	return font;
}

void Font::SetFont( BitmapFont& text_font)
{
	if (font != &text_font)
	{
		font = &text_font;
		needNewMeshes = true;
	}
}

Color Font::GetFillColor() const
{
	return fillColor;
}

void Font::SetFillColor(Color fill_color)
{
	fillColor = fill_color;
	for (auto& pair : meshes)
	{
		auto& mesh = pair.second;
		mesh.ClearColors();
		mesh.AddColor(fill_color);
	}
}

void Font::BuildNewMeshesIfNeeded() const
{
	if (!needNewMeshes)
		return;

	for (auto& pair : meshes)
	{
		auto& mesh = pair.second;
		mesh.Clear();
	}

	float start_x = 0;
	float start_y = 0;


	for (unsigned i = 0; i < string.size(); ++i)
	{
		wchar_t temp_char = string[i];

		if (temp_char == ' ')
		{
			start_x += font->GetCharacter(temp_char).xadvance;
		}

		if (temp_char == '\n')
		{
			start_x = 0;
			start_y += font->GetInformation().lineHeight;
		}

		if (temp_char == '\t')
		{
			start_x += font->GetCharacter(L' ').xadvance * 4;
		}

		vector2 image_dimensions = { (float)font->GetInformation().imageWidth, (float)font->GetInformation().imageHeight };
		vector2 current_position = { start_x, start_y };

		if (temp_char != ' ' && temp_char != '\n' && temp_char != '\t')
		{
			AddCharQuadToMesh(meshes[font->GetCharacter(temp_char).page], font->GetCharacter(temp_char), image_dimensions,
				current_position, font->GetInformation().lineHeight);

			start_x += (float)font->GetCharacter(temp_char).xadvance;
		}
	}

	for (auto& pair : meshes)
	{
		auto& mesh = pair.second;
		mesh.ClearColors();
		mesh.AddColor(fillColor);
		mesh.SetPointListType(PointListType::Triangles);
	}

	needNewMeshes = false;
}

namespace
{
	void AddCharQuadToMesh(Mesh& mesh, BitmapFont::Character character, vector2 image_dimensions,
		vector2 current_position, float line_height)
	{
		vector2 left_top, right_top, left_bottom, right_bottom;
		vector2 left_top_uv, right_top_uv, left_bottom_uv, right_bottom_uv;

		float left = current_position.x + character.xoffset;
		float right = left + character.width;

		float bottom = ((character.yoffset + character.height)*-1 + line_height) - current_position.y;
		float top = bottom + character.height;

		left_top = { left, top };
		left_top_uv = { character.x / image_dimensions.x, character.y / image_dimensions.y };

		right_top = { right, top };
		right_top_uv = { (character.x + character.width) / image_dimensions.x , character.y / image_dimensions.y };

		left_bottom = { left, bottom };
		left_bottom_uv = { character.x / image_dimensions.x, (character.y + character.height) / image_dimensions.y };

		right_bottom = { right, bottom };
		right_bottom_uv = { (character.x + character.width) / image_dimensions.x , (character.y + character.height) / image_dimensions.y };

		// Triangle 1
		mesh.AddPoint(left_top);
		mesh.AddTextureCoordinate(left_top_uv);
		mesh.AddPoint(left_bottom);
		mesh.AddTextureCoordinate(left_bottom_uv);
		mesh.AddPoint(right_top);
		mesh.AddTextureCoordinate(right_top_uv);

		// Triangle 2
		mesh.AddPoint(right_top);
		mesh.AddTextureCoordinate(right_top_uv);
		mesh.AddPoint(left_bottom);
		mesh.AddTextureCoordinate(left_bottom_uv);
		mesh.AddPoint(right_bottom);
		mesh.AddTextureCoordinate(right_bottom_uv);
	}
}
