/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : BitmapFont.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#include "BitmapFont.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include "Sprite.hpp"

namespace
{
	template <typename Number>
	void SearchLineAndSetNumber(const std::wstring& line, const std::wstring& key, Number& storage);
	void SearchLineAndSetString(const std::wstring& line, std::wstring key, std::wstring& storage);
}


BitmapFont::BitmapFont(const std::wstring& fnt_filepath)
{
	LoadFromFile(fnt_filepath);
}

BitmapFont::~BitmapFont()
{
	characters.clear();
	pageTextures.clear();
}

bool BitmapFont::LoadFromFile(const std::wstring& fnt_filepath)
{
	if (!CanParseFile(fnt_filepath))
		return false;

	const auto file_dir = std::filesystem::path(fnt_filepath).parent_path();

	for (const auto& page_name : information.pageNames)
	{
		const auto image_filepath = file_dir / page_name;

		if (!std::filesystem::exists(image_filepath))
			return false;

		pageTextures.push_back(new Sprite(image_filepath.string()));
		const auto loaded = pageTextures.at(pageTextures.size()-1)->Texture_Load();

		if (!loaded)
			return false;
	}

	return true;
}

const BitmapFont::Information& BitmapFont::GetInformation() const
{
	return information;
}

BitmapFont::Character BitmapFont::GetCharacter(wchar_t character_id) const
{
	auto match = characters.find(character_id);

	if (match == characters.end())
	{
		match = characters.find((wchar_t)-1);

		if (match == characters.end())
			return BitmapFont::Character{};
	}
	return match->second;
}

unsigned short BitmapFont::GetLineHeight() const
{
	return information.lineHeight;
}

 Sprite* BitmapFont::GetTexture(int page_index) 
{
	assert(information.pagesCount > 0);
	assert(page_index >= 0 && page_index < information.pagesCount);

	return pageTextures.at(page_index);
}

bool BitmapFont::HasCharacter(wchar_t character_id) const
{
	auto match = characters.find(character_id);
	return match != characters.end();
}

bool BitmapFont::CanParseFile(const std::wstring& fnt_filepath)
{
	std::wifstream stream(fnt_filepath);
	if (!stream)
	{
		stream.close();
		return false;
	}

	std::wstring line;
	std::wstring line_type;
	while (stream)
	{
		stream >> line_type;
		std::getline(stream, line);

		if (line_type == L"info")
		{
			SearchLineAndSetString(line, L"face", information.fontName);
			SearchLineAndSetNumber(line, L"size", information.fontSize);
		}
		else if (line_type == L"common")
		{
			SearchLineAndSetNumber(line, L"lineHeight", information.lineHeight);
			SearchLineAndSetNumber(line, L"scaleW", information.imageWidth);
			SearchLineAndSetNumber(line, L"scaleH", information.imageHeight);
			SearchLineAndSetNumber(line, L"pages", information.pagesCount);

			information.pageNames.resize(information.pagesCount);
		}
		else if (line_type == L"page")
		{
			int offset = 0;
			std::wstring name{};
			SearchLineAndSetNumber(line, L"id", offset);
			SearchLineAndSetString(line, L"file", name);

			information.pageNames.at(offset) = name;
		}
		else if (line_type == L"char")
		{
			Character desc{};

			SearchLineAndSetNumber(line, L"id", desc.id);
			SearchLineAndSetNumber(line, L"x", desc.x);
			SearchLineAndSetNumber(line, L"y", desc.y);
			SearchLineAndSetNumber(line, L"width", desc.width);
			SearchLineAndSetNumber(line, L"height", desc.height);
			SearchLineAndSetNumber(line, L"xoffset", desc.xoffset);
			SearchLineAndSetNumber(line, L"yoffset", desc.yoffset);
			SearchLineAndSetNumber(line, L"xadvance", desc.xadvance);
			SearchLineAndSetNumber(line, L"page", desc.page);

			characters.insert_or_assign(desc.id, desc);
		}
		
		//line.clear();
		//line_type.clear();
	}

	
	stream.clear();
	stream.close();

	return true;
}

namespace
{
	template <typename Number>
	void SearchLineAndSetNumber(const std::wstring& line, const std::wstring& key, Number& storage)
	{
		std::wstring temp = key;
		temp += L"=";

		auto offset = line.find(temp);
		if (offset != std::wstring::npos)
		{
			offset += temp.length();
			auto end = line.find(L" ", offset);
			int mystoi = std::stoi(line.substr(offset, end - offset));
			storage = static_cast<Number>(mystoi);
		}
	}

	void SearchLineAndSetString(const std::wstring& line, std::wstring key, std::wstring& storage)
	{
		key += L"=\"";
		auto offset = line.find(key);

		if (offset != std::wstring::npos)
		{
			offset += key.length();
			auto end = line.find(L"\"", offset);
			storage = line.substr(offset, end - offset);
		}
	}
}