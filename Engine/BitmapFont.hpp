/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : BitmapFont.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Font.hpp"

class BitmapFont
{
public:
	struct Character
	{
		~Character() = default;
		wchar_t id = wchar_t(-1);
		unsigned short x = 0;
		unsigned short y = 0;
		unsigned short width = 0;
		unsigned short height = 0;
		short xoffset = 0;
		short yoffset = 0;
		short xadvance = 0;
		unsigned char page = 0;
	};

	struct Information
	{
		~Information() = default;
		short fontSize = 0;
		std::wstring fontName{};
		unsigned short lineHeight = 0;
		unsigned short imageWidth = 0;
		unsigned short imageHeight = 0;
		unsigned short pagesCount = 0;
		std::vector<std::wstring> pageNames{};
	};

public:
	BitmapFont(const std::wstring& fnt_filepath);
	~BitmapFont();
	bool LoadFromFile(const std::wstring& fnt_filepath);
	const Information& GetInformation() const;
	Character GetCharacter(wchar_t character_id) const;
	unsigned short GetLineHeight() const;
	 Sprite* GetTexture(int page_index) ;
	bool HasCharacter(wchar_t character_id) const;

private:
	bool CanParseFile(const std::wstring& fnt_filepath);

private:
	Information information{};
	std::unordered_map<wchar_t, Character>characters{};
	std::vector<Sprite*> pageTextures{};
};