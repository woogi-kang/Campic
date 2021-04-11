/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : UI.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once
#include <string>
#include "LevelJson.hpp"
#include "Component.hpp"

class UI : public Component
{
public:
	UI(std::string id_, bool IsLock_ = true);

	bool Initialize(Object* Ob) override;
	void Update(float dt) override;
	void Delete() override;

	void SetIsLock(bool islock) { IsLock = islock; }
	void SetId(const std::string& id_) { id = id_; }
	void TriggerLevelLock(std::string level_id, bool isclear = true);
	bool GetIsLock() { return IsLock; }
	std::string GetId() { return id; }

private:
	std::string id;
	bool IsLock = false;
};

