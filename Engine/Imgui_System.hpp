/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : imgui_System.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include "Application.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Objectmanager.hpp"
#include "Animation.hpp"
#include "Collision.hpp"
#include "AudioManager.hpp"
#include "ObjectDepth.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>

class Imgui_System
{
public:
	bool Initialize();
	void Update(float dt);
	void Quit();

	void Draw();
	void Editor(bool show_window);
	void ObjectCreator(bool object_creator);
	void ObjectEditor(bool object_editor);

	void TileEditor(bool tile_editor);
	GLuint ImageHelper(std::string path);

	// Helper
	void SpriteHelper();

private:
	GLFWwindow* window = nullptr;
	
	bool show_window = false;
	bool object_creator = false;
	bool object_editor = false;
	bool tile_editor = false;

	std::vector<std::string> imageList;
	std::vector<std::string> Day_tileList;
	std::vector<std::string> Night_tileList;
	std::vector<std::string> Sunset_tileList;

	std::unordered_map<std::string, ImTextureID> Day_tileList_buttons;
	std::unordered_map<std::string, ImTextureID> Night_tileList_buttons;
	std::unordered_map<std::string, ImTextureID> Sunset_tileList_buttons;

	std::string image_path;
	std::string tile_path;

	std::string image_dir = "asset/images/";
	std::string Day_tile_dir = "asset/images/Tiles/Day/";
	std::string Night_tile_dir = "asset/images/Tiles/Night/";
	std::string Sunset_tile_dir = "asset/images/Tiles/Sunset/";

	ImGuiID non_ani_tile_selected = false;

	bool is_normal_tile = true;
	bool graphics_tile = false;
    bool player_existed = false;
    bool start_existed = false;
    bool capture_existed = false;
	bool set_flip = false;

	Object* selectObj = nullptr;
	Object* player_ = nullptr;
};

extern Imgui_System IMGUI_;