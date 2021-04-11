/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : LevelJson.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#include "LevelJson.hpp"

LevelJson LevelJson_;

bool LevelJson::Initialize()
{
	LevelDocument.SetObject();

	return true;
}

void LevelJson::Update(float /*dt*/)
{

}

void LevelJson::WriteLevelLockInformation()
{
	std::string filename(m_LevelLock_Path);
	filename.append("LevelLock.json");

	FILE* fp = fopen(filename.c_str(), "w+");

	char writeBuffer[65535];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	PrettyWriter<FileWriteStream> writer(os);

	LevelDocument.Accept(writer);
	LevelDocument.SetObject();

	fclose(fp);
}

Document LevelJson::OpenLevelLockInformation() const
{
	std::string filename(m_LevelLock_Path);
	filename.append("LevelLock.json");

	FILE* fp = fopen(filename.c_str(), "r+");

	char readBuffer[65535];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document LevelLockDoc;
	LevelLockDoc.ParseStream(is);

	fclose(fp);

	return LevelLockDoc;
}

std::map<std::string, bool> LevelJson::LoadLevelLock()
{
	m_LevelLock.clear();
	LevelDocument = OpenLevelLockInformation();

	for (auto& temp : LevelDocument.GetObject())
	{
		Value& lock_array = temp.value;
		Value level, clear;

		level.SetObject();
		clear.SetObject();

		level = lock_array.FindMember("ID:")->value;
		clear = lock_array.FindMember("Clear")->value;

		std::string id = level.GetString();
		bool status = clear.GetBool();

		m_LevelLock.insert(std::pair(id, status));
	}

	return m_LevelLock;
}

void LevelJson::CreateLevelLockDocument()
{
	Value array(kArrayType);
	Value level, clear;

	array.SetObject();
	level.SetObject();
	clear.SetObject();

	for(int i = 0; i < 20; ++i)
	{
		std::string lev = "Level" + std::to_string(i);

		level.SetString(lev.c_str(), LevelDocument.GetAllocator());
		if (i == 1)
			clear.SetBool(true);
		else
			clear.SetBool(false);

		array.AddMember("ID:", level, LevelDocument.GetAllocator());
		array.AddMember("Clear", clear, LevelDocument.GetAllocator());

		LevelDocument.AddMember("Level", array, LevelDocument.GetAllocator());

		level.SetObject();
		clear.SetObject();
		array.SetObject();
	}

	LevelDocument.Parse(LevelBuffer.GetString());
	
	WriteLevelLockInformation();
}

void LevelJson::Quit()
{
	//LevelDocument.Clear();
	//LevelBuffer.Clear();
}

