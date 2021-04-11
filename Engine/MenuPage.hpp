/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : MenuPage.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include "Object.hpp"

class MenuPage
{
public:
	MenuPage() = default;
	~MenuPage() { buttons.clear(); }

	std::vector<Object*>& GetButtons() { return buttons; }

	void Clear() { buttons.clear(); }
	void InsertButtons(Object* obj) { buttons.push_back(obj); }

private:
	std::vector<Object*> buttons;
};