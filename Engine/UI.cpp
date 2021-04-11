/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : UI.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#include "UI.hpp"

UI::UI(std::string id_,bool IsLock_) : id(id_), IsLock(IsLock_)
{
}


bool UI::Initialize(Object * Ob)
{
	if(object == nullptr)
	{
		object = Ob;
	}
	return true;
}

void UI::Update(float dt)
{
}

void UI::Delete()
{
}

void UI::TriggerLevelLock(std::string level_id, bool isclear)
{
	Document load = LevelJson_.OpenLevelLockInformation();

	for (auto& temp : load.GetObject())
	{
		Value& lock_array = temp.value;

		if(lock_array.FindMember("ID:")->value.GetString() == level_id)
		{
			lock_array.FindMember("Clear")->value.SetBool(isclear);
			break;
		}
	}

	std::string filename("asset/JsonFiles/LevelLock.json");

	FILE* fp = fopen(filename.c_str(), "w+");

	char writeBuffer[65535];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	PrettyWriter<FileWriteStream> writer(os);

	load.Accept(writer);

	fclose(fp);
}
