/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : LevelJson.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "filewritestream.h"
#include "filereadstream.h"
#include "prettywriter.h"
#include <string>
#include <map>

using namespace rapidjson;

class LevelJson
{
public:
	bool Initialize();
	void Update(float /*dt*/);
	void Quit();

public:
	void WriteLevelLockInformation();
	Document OpenLevelLockInformation() const;
	std::map<std::string, bool> LoadLevelLock();
	void CreateLevelLockDocument();

	std::map<std::string, bool> GetLevelLock() { return m_LevelLock; }

private:
	Document LevelDocument{};
	StringBuffer LevelBuffer{};

private:
	std::string m_LevelLock_Path = "asset/JsonFiles/";

	std::map<std::string, bool> m_LevelLock;
};

extern LevelJson LevelJson_;

