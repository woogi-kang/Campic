/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Font.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Component.hpp"
#include "Color.hpp"
#include <unordered_map>
#include "Mesh.hpp"

class BitmapFont;

class Font : public Component
{
public:
	Font(const std::wstring& fnt_filepath, std::wstring text_string);
	Font(std::wstring text_string,  BitmapFont& text_font);

	bool Initialize(Object* Ob) override;
	void Update(float dt) override;
	void Delete() override;

	std::wstring GetString() const;
	void SetString(const std::wstring& text_string);
	 BitmapFont* GetFont() ;
	void SetFont( BitmapFont& text_font);
	Color GetFillColor() const;
	void SetFillColor(Color fill_color);

	std::unordered_map<int, Mesh>& GetFontMesh() { return meshes;}

private:
	void BuildNewMeshesIfNeeded() const;

private:
	std::wstring string{};
	BitmapFont* font = nullptr;
	Sprite* sprite = nullptr;
	Color fillColor{ 255, 255, 255, 255 };
	mutable bool needNewMeshes = false;
	mutable std::unordered_map<int, Mesh> meshes{};
};
