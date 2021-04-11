/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : imgui_System.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Imgui_System.hpp"
#include "stb_image.h"
#include <imgui_internal.h>
#include "Tile_Map.hpp"
#include "Physics.hpp"
#include "Capture.hpp"
#include "UI.hpp"
#include "Level.hpp"
#include "Trigger.hpp"
#include "Chapter.hpp"

Imgui_System IMGUI_;

bool Imgui_System::Initialize()
{
	//Imgui Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	const char* glsl_version = "#version 300 es";

	window = glfwGetCurrentContext();

	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	//Imgui configs
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.FontDefault = NULL;
	io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	//Tile list in project directory
	for (auto& p : std::filesystem::directory_iterator("asset/images/Tiles/Day"))
	{
		Day_tileList.push_back(Day_tile_dir + p.path().filename().string());
	}
	for (auto& p : std::filesystem::directory_iterator("asset/images/Tiles/Night"))
	{
		Night_tileList.push_back(Night_tile_dir + p.path().filename().string());
	}
	for (auto& p : std::filesystem::directory_iterator("asset/images/Tiles/Sunset"))
	{
		Sunset_tileList.push_back(Sunset_tile_dir + p.path().filename().string());
	}

	// Add texture into imgui
	for (auto& temp : Day_tileList)
	{
		auto texture = ImageHelper(temp);
		Day_tileList_buttons.insert(std::make_pair(temp, (void*)(intptr_t)texture));
	}
	for (auto& temp : Night_tileList)
	{
		auto texture = ImageHelper(temp);
		Night_tileList_buttons.insert(std::make_pair(temp, (void*)(intptr_t)texture));
	}
	for (auto& temp : Sunset_tileList)
	{
		auto texture = ImageHelper(temp);
		Sunset_tileList_buttons.insert(std::make_pair(temp, (void*)(intptr_t)texture));
	}

	return true;
}

void Imgui_System::Update(float dt)
{
	if (Input::IsKeyTriggered(GLFW_KEY_TAB))
	{
		show_window = !show_window;
		selectObj = nullptr;
	}

	if (show_window)
	{
		glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);

		if (Input::IsMouseTriggered(GLFW_MOUSE_BUTTON_LEFT))
		{
			if(!selectObj)
				selectObj = Input::ImGuiObjectClicker();
		}

		if (Input::IsMouseDoubleClicked(GLFW_MOUSE_BUTTON_LEFT))
		{
			selectObj = nullptr;
		}

		if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			if (selectObj)
				selectObj->SetTranslation(Input::GetMousePos());
		}

		if (Input::IsKeyPressed(GLFW_KEY_I))
			Tile_Map_.Delete_Tile();

		if (is_normal_tile)
		{
			if (Input::IsKeyPressed(GLFW_KEY_G))
			{
				if (graphics_tile)
					Tile_Map_.Make_Tile(tile_path, Tile_Type::Graphical);
				else
					Tile_Map_.Make_Tile(tile_path, Tile_Type::Physical);
			}
			if (Input::IsKeyPressed(GLFW_KEY_H))
			{
				if (graphics_tile)
					Tile_Map_.MakeTileVertical(tile_path, Tile_Type::Graphical, -1);
				else
					Tile_Map_.MakeTileVertical(tile_path, Tile_Type::Physical,-1);
			}
			if (Input::IsKeyPressed(GLFW_KEY_J))
			{
                            if (graphics_tile)
                                Tile_Map_.MakeTileHorizontal(tile_path, Tile_Type::Graphical, 1);
                            else
                                Tile_Map_.MakeTileHorizontal(tile_path, Tile_Type::Physical, 1);
			}
			if (Input::IsKeyPressed(GLFW_KEY_U))
			{
                            if (graphics_tile)
                                Tile_Map_.MakeTileHorizontal(tile_path, Tile_Type::Graphical, -1);
                            else
                                Tile_Map_.MakeTileHorizontal(tile_path, Tile_Type::Physical, -1);
			}
			if (Input::IsKeyPressed(GLFW_KEY_K))
			{
                                if (graphics_tile)
                                    Tile_Map_.MakeTileVertical(tile_path, Tile_Type::Graphical, 1);
                                else
                                    Tile_Map_.MakeTileVertical(tile_path, Tile_Type::Physical, 1);
			}
		}
		else
		{
			if (Input::IsKeyPressed(GLFW_KEY_G))
			{
				if (graphics_tile)
					Tile_Map_.Make_Ani_Tile(tile_path, Tile_Type::Graphical);
				else
					Tile_Map_.Make_Ani_Tile(tile_path, Tile_Type::Physical);
			}
		}
	}
}

void Imgui_System::Quit()
{
	imageList.clear();
	Day_tileList.clear();
	Night_tileList.clear();
	Sunset_tileList.clear();
	Day_tileList_buttons.clear();
	Night_tileList_buttons.clear();
	Sunset_tileList_buttons.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Imgui_System::Draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	Editor(show_window);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	window = glfwGetCurrentContext();
	glfwSwapBuffers(window);
}

void Imgui_System::Editor(bool show_window)
{
	if (!show_window)
		return;

	ImGui::SetNextWindowSize(ImVec2(400, 400));
	if (!ImGui::Begin("ImGui Editor", &show_window, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
		return;
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Frame Rate (%.1f FPS)", io.Framerate);

	if (ImGui::RadioButton("Object Creator", object_creator))
	{
		object_creator = !object_creator;
		object_editor = false;
		tile_editor = false;
	}

	ImGui::SameLine();

	if (ImGui::RadioButton("Object Editor", object_editor))
	{
		object_editor = !object_editor;
		object_creator = false;
		tile_editor = false;
	}

	ImGui::SameLine();

	if (ImGui::RadioButton("Tile Editor", tile_editor))
	{
		tile_editor = !tile_editor;
		object_creator = false;
		object_editor = false;
	}

	// Create editor window
	ObjectCreator(object_creator);
	ObjectEditor(object_editor);
	TileEditor(tile_editor);

	for (int i = 0; i < 50; i++)
	{
		if (tile_editor)
		{
			if (i == 5)
				break;
		}
		if (object_editor)
		{
			if (i == 30)
				break;
		}
		if (object_creator)
		{
			if (i == 30)
				break;
		}
		ImGui::Spacing();
	}

	if (ImGui::Button("Save Level"))
	{
		StateManager_.GetCurrentState()->SaveLevel();
	}

	ImGui::SameLine();
        if (ImGui::Button("Save Object"))
        {
            StateManager_.GetCurrentState()->SaveObject();
        }

        ImGui::SameLine();
        if (ImGui::Button("Save G-Tile"))
        {
            StateManager_.GetCurrentState()->SaveGTile();
        }

        ImGui::SameLine();
        if (ImGui::Button("Save P-Tile"))
        {
            StateManager_.GetCurrentState()->SavePTile();
        }


	// Delete ALL Object
	if (ImGui::Button("Clear All"))
	{
		StateManager_.GetCurrentState()->UnLoad();
                StateManager_.GetCurrentState()->SetPlayerObjectPointer(nullptr);
                StateManager_.GetCurrentState()->SetCaptureObjectPointer(nullptr);
                Tile_Map_.ClearGraphicTiles();
                Tile_Map_.ClearPhysicalTiles();
        player_existed = false;
        start_existed = false;
        capture_existed = false;
        Tile_Map_.MakeGridFalse();
		Physics_.ResetPreviousSize();
	}

	if (ImGui::Button("Clear Graphic Tiles"))
	{
		Tile_Map_.ClearGraphicTiles();
	}

	if (ImGui::Button("Clear Physical Tiles"))
	{
		Tile_Map_.ClearPhysicalTiles();
	}

	if(ImGui::Button("Clear Objects"))
	{
		Objectmanager_.GetObjectMap().clear();
                StateManager_.GetCurrentState()->SetPlayerObjectPointer(nullptr);
                StateManager_.GetCurrentState()->SetCaptureObjectPointer(nullptr);
                StateManager_.GetCurrentState()->SetCamera();
	}

	ImGui::End();
}

void Imgui_System::ObjectCreator(bool object_creator)
{
	if (!object_creator)
		return;

	ImGui::Text("Archetype");
	ImGui::Separator();

	ImGui::Button("Start Pos");
	if (ImGui::IsItemActive())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		for (auto obj : Objectmanager_.GetObjectMap())
		{
			if (obj.get()->GetObjectType() == ObjectType::Start_Pos)
				start_existed = true;
		}
	}
	if (ImGui::IsItemDeactivated())
	{
		if (!start_existed)
		{
			Object* s_pos = new Object();
			s_pos->SetTranslation(Input::GetMousePos());
			s_pos->SetScale({ 256, 130 });
			s_pos->SetMesh(mesh::CreateBox(1, { 0,255,255, 255 }));
			s_pos->SetDepth(GAME_OBJECT);
			s_pos->SetObjectType(ObjectType::Start_Pos);
			s_pos->AddInitComponent(new Sprite("asset/images/Objects/StartPoint.png"));

			Objectmanager_.AddObject(s_pos);
		}
	}

	ImGui::SameLine();
        ImGui::Button("Jump high Object");
        if (ImGui::IsItemActive())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        if (ImGui::IsItemDeactivated())
        {
                Object* jump_obj = new Object();
                jump_obj->SetTranslation(Input::GetMousePos());
                jump_obj->SetScale({ 75.0f, 75.0f });
                jump_obj->SetMesh(mesh::CreateBox(1, { 0,255,255, 255 }));
                jump_obj->SetDepth(GAME_OBJECT);
                jump_obj->SetObjectType(ObjectType::JumpIng);
                jump_obj->AddInitComponent(new Collision(box_));
                jump_obj->GetComponentByTemplate<Collision>()->SetFilter(Filter::Jump);
		jump_obj->AddComponent(new Animation("asset/images/Objects/JumpBoard.png", "obstacle", 15, 0.05f));

                Objectmanager_.AddObject(jump_obj);
        }

        ImGui::SameLine();
        ImGui::Button("Obstacle");
        if (ImGui::IsItemActive())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        if (ImGui::IsItemDeactivated())
        {
            Object* obstacle = new Object();
            obstacle->SetTranslation(Input::GetMousePos());
            obstacle->SetScale({ 75.0f, 75.0f });
            obstacle->SetMesh(mesh::CreateBox(1, { 0,255,255, 255 }));
            obstacle->SetDepth(GAME_OBJECT);
            obstacle->SetObjectType(ObjectType::Obstacle);
            obstacle->AddInitComponent(new Collision(box_));
			obstacle->AddComponent(new Animation("asset/images/Objects/Danger.png", "obstacle", 9, 0.05f));

            Objectmanager_.AddObject(obstacle);
        }
        ImGui::SameLine();

	ImGui::Button("Reset Pos");
	if (ImGui::IsItemActive())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
	if (ImGui::IsItemDeactivated())
	{
		if (!start_existed)
		{
			Object* r_pos = new Object();
			r_pos->SetTranslation(Input::GetMousePos());
			r_pos->SetScale({ 75.0f, 75.0f });
			r_pos->SetMesh(mesh::CreateBox(1, { 255,255,255, 255 }));
			r_pos->SetDepth(GAME_OBJECT);
			r_pos->SetObjectType(ObjectType::Reset_Pos);
			r_pos->AddComponent(new Collision(box_));
			r_pos->AddComponent(new Animation("asset/images/Cameraman.Png", "cameraman", 3, 0.01f));

			Objectmanager_.AddObject(r_pos);
		}
	}

	ImGui::NewLine();

	static int input_chapter = 0;
	ImGui::InputInt("Chapter", &input_chapter, 0, 3);
	static int input_level = 0;
	ImGui::InputInt("Level", &input_level, 0, 20);

	ImGui::Button("Chapter and Level");
	if (ImGui::IsItemActive())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	if (ImGui::IsItemDeactivated())
	{
		Object* chapter = new Object();
		chapter->SetDepth(GAME_OBJECT);
		chapter->AddComponent(new Chapter(input_chapter,input_level));
		chapter->GetMesh().Invisible();
		Objectmanager_.AddObject(chapter);
	}

	ImGui::NewLine();

	ImGui::Text("Type target Level");
	static char buffer[100] = "";
	ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer));

	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		memset(buffer, 0, 100);
	}

	ImGui::Button("Door");
	if (ImGui::IsItemActive())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	if (ImGui::IsItemDeactivated())
	{
		Object* door = new Object();
		door->SetTranslation(Input::GetMousePos());
		door->SetScale({ 256, 130 });
		door->SetDepth(GAME_OBJECT);
		door->SetMesh(mesh::CreateBox(1, { 255,255,255,255 }));
		door->SetObjectType(ObjectType::Door);
		door->AddInitComponent(new Collision(box_));
		door->AddInitComponent(new Sprite("asset/images/Objects/BusStop.png"));
		door->AddComponent(new UI(buffer));

		Objectmanager_.AddObject(door);
	}

	ImGui::SameLine();
	ImGui::Button("Photo Zone");
	if (ImGui::IsItemActive())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	if (ImGui::IsItemDeactivated())
	{
		Object* zone = new Object();
		zone->SetTranslation(Input::GetMousePos());
		zone->SetScale({ 100, 100 });
		zone->SetDepth(GAME_OBJECT);
		zone->SetMesh(mesh::CreateBox(1, { 255,255,255,150 }));
		zone->SetObjectType(ObjectType::Door);
		zone->AddInitComponent(new Animation("asset/images/Objects/SavePoint.png","zone",4, 0.05f));

		Objectmanager_.AddObject(zone);
	}

	ImGui::Separator();

    ImGui::Text("Trigger Pos");
	static vector2 pos;
	ImGui::InputFloat("x", &pos.x, -1000.f, 2000.0f);
	ImGui::InputFloat("y", &pos.y, -1000.f, 2000.0f);

	static vector2 scale;
	ImGui::InputFloat("x_", &scale.x, -1000.f, 2000.0f);
	ImGui::InputFloat("y_", &scale.y, -1000.f, 2000.0f);

    ImGui::Text("Image Frame");
    static float i_frame;
    ImGui::InputFloat("i_frame", &i_frame, -1000.f, 2000.0f);
    ImGui::Text("Update Frame");
    static float u_frame;
    ImGui::InputFloat("u_frame", &u_frame, -1000.f, 2000.0f);

    ImGui::Text("Text");
    static char buf_text[100] = "";
    ImGui::InputText(" ", buf_text, IM_ARRAYSIZE(buf_text));

    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        memset(buf_text, 0, 100);
    }

	const char* trigger_type[] = { "CheckPoint","Door", "Font", "Graphic", "None" };
	static int item_current = 0;
	ImGui::Combo("Select Trigger Type", &item_current, trigger_type, IM_ARRAYSIZE(trigger_type));
    ImGui::Button("Trigger");
	if (ImGui::IsItemActive())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
        
	if (ImGui::IsItemDeactivated())
	{
		Object* trigger = new Object();
        trigger->SetTranslation(Input::GetMousePos());
        trigger->SetScale({ 10,100 });
        trigger->SetDepth(GAME_OBJECT +0.01f);
        trigger->SetObjectType(ObjectType::Trigger);
        trigger->SetMesh(mesh::CreateBox());
        trigger->AddInitComponent(new Collision(box_));
        trigger->AddInitComponent(new Trigger(pos, scale, static_cast<TriggerStyle>(item_current), buf_text,false, i_frame, u_frame));

		Objectmanager_.AddObject(trigger);
	}
        ImGui::Button("falling_limit");
        if (ImGui::IsItemActive())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        if (ImGui::IsItemDeactivated())
        {
            Object* limit = new Object();
            limit->SetTranslation(Input::GetMousePos());
            limit->SetScale({1.f});
            limit->SetDepth(GAME_OBJECT + 0.01f);
            limit->SetObjectType(ObjectType::Falling_Limit);
            limit->SetMesh(mesh::CreateBox());
            Objectmanager_.AddObject(limit); 
        }
}

void Imgui_System::ObjectEditor(bool object_editor)
{
	if (!object_editor)
		return;

	if (selectObj == nullptr)
		return;

	selectObj->GetTransform().Imgui_Transform();

	if (selectObj->GetComponentByTemplate<Animation>() != nullptr)
		selectObj->GetComponentByTemplate<Animation>()->Imgui_Animation();

	SpriteHelper();

	if (ImGui::Button("Delete"))
	{
		for (auto object = Objectmanager_.GetObjectMap().begin(); object != Objectmanager_.GetObjectMap().end();++object)
		{
			if (object->get() == selectObj)
			{
				object = Objectmanager_.GetObjectMap().erase(object);
				selectObj = nullptr;
			}
		}
	}
}

void Imgui_System::SpriteHelper()
{
	if (ImGui::TreeNode("Sprite"))
	{
		static std::string current_item = image_path;

		if (ImGui::BeginCombo("Select texture", current_item.c_str()))
		{
			for (size_t n = 0; n < imageList.size(); n++)
			{
				bool is_selected = (current_item.c_str() == imageList[n].c_str());
				if (ImGui::Selectable(imageList[n].c_str(), is_selected))
					current_item = imageList[n].c_str();
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (selectObj->GetComponentByTemplate<Sprite>() != nullptr)
		{
			if (ImGui::Button("Change Texture"))
			{
				selectObj->GetComponentByTemplate<Sprite>()->ChangeSprite("asset/images/" + current_item);
			}
		}
		else
		{
			if (ImGui::Button("Add Texture"))
			{
				selectObj->AddComponent(new Sprite("asset/images/" + current_item));
			}
		}
		ImGui::TreePop();
	}
}


void Imgui_System::TileEditor(bool tile_editor)
{
	if (!tile_editor)
		return;

	ImGui::Separator();

	if (ImGui::RadioButton("Create Graphic Tile", graphics_tile))
	{
		graphics_tile = !graphics_tile;
	}

	ImGui::Text("If not selected, creates physical tile");

	ImGui::Separator();

	int i = 1;
	int j = 1;

	ImGui::Text("Day Tiles");
	ImGui::Separator();

	for (auto& temp : Day_tileList_buttons)
	{
		if (ImGui::ImageButton(temp.second, ImVec2(16, 16)))
		{
			tile_path = temp.first;
			is_normal_tile = true;
		}

		if (i != 8)
			ImGui::SameLine();
		else
			i = 0;
		i++;
	}

	ImGui::NewLine();

	i = 1; j = 1;

	ImGui::Text("Sunset Tiles");
	ImGui::Separator();

	for (auto& temp : Sunset_tileList_buttons)
	{
		if (ImGui::ImageButton(temp.second, ImVec2(16, 16)))
		{
			tile_path = temp.first;
			is_normal_tile = true;
		}

		if (i != 8)
			ImGui::SameLine();
		else
			i = 0;
		i++;
	}

	ImGui::NewLine();
	i = 1; j = 1;

	ImGui::Text("Night Tiles");
	ImGui::Separator();

	for (auto& temp : Night_tileList_buttons)
	{
		if (ImGui::ImageButton(temp.second, ImVec2(16, 16)))
		{
			tile_path = temp.first;
			is_normal_tile = true;
		}

		if (i != 8)
			ImGui::SameLine();
		else
			i = 0;
		i++;
	}
}

GLuint Imgui_System::ImageHelper(std::string path)
{
	int my_image_width, my_image_height;

	unsigned char* my_image_data = stbi_load(path.c_str(),
		&my_image_width, &my_image_height, nullptr, STBI_rgb_alpha);

	// Turn the RGBA pixel data into an OpenGL texture:
	GLuint my_opengl_texture;
	glGenTextures(1, &my_opengl_texture);
	glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, my_image_width, my_image_height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, my_image_data);

	stbi_image_free(my_image_data);

	return my_opengl_texture;
}