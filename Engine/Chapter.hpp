/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Chapter.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include "Component.hpp"

class Chapter : public Component
{
public:
	Chapter() = default;
	Chapter(int chapter_, int level_) :  chapter(chapter_), level(level_) {}

	bool Initialize(Object* Ob) override;
	void Update(float dt) override;
	void Delete() override;

	int GetChapter() { return chapter; }
	int GetLevel() { return level; }

private:
	int chapter = 0;
	int level = 0;
};

