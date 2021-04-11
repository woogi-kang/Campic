/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Tile_Map.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : SoonWoo Jung
Secondary :
- End Header ----------------------------------------------------------------*/
#include "Tile_Map.hpp"
#include <iostream>
#include "Physics.hpp"

Tile_Map Tile_Map_;


bool Tile_Map::Initialize()
{
    if (!graphics_tiles.empty())
    {
        for (auto& obj : graphics_tiles)
        {
            for (auto& component : obj.second->GetComponent())
            {
                component->Initialize(obj.second);
            }
        }
    }
    return true;
}

void Tile_Map::Update(float dt)
{
    if(!init)
    {
        Initialize();
        init = true;
    }

	for(auto it = graphics_tiles.begin(); it != graphics_tiles.end(); ++it)
	{
		for(auto component : it->second->GetComponent())
		{
			component->Update(dt);
		}
	}
}

void Tile_Map::RemoveTiles()
{
	graphics_tiles.clear();
	physical_tiles.clear();
}

void Tile_Map::Make_Tile(std::string image, Tile_Type type)
{
	int tile_x = static_cast<int>(Input::GetMousePos().x + TEMP_WIDTH / 2);
	int tile_y = static_cast<int>(Input::GetMousePos().y + TEMP_HEIGHT / 2);

    vector2 tile_position = {0};

    if (!tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE])
    {
        tile_position = { (float)((2 * (floor)(tile_x / TILE_SIZE) + 1)* TILE_SIZE / 2) - TEMP_WIDTH / 2
            , (float)((2 * (floor)(tile_y / TILE_SIZE) + 1)*TILE_SIZE) / 2 - TEMP_HEIGHT / 2 };
       
    	Normal_Tile(image, tile_x, tile_y, tile_position, type);
        
    	tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE] = true;
    }
}

void Tile_Map::MakeTileVertical(std::string image, Tile_Type type, int num)
{
    int tile_x = static_cast<int>(Input::GetMousePos().x + TEMP_WIDTH / 2);
    int tile_y = static_cast<int>(Input::GetMousePos().y + TEMP_HEIGHT / 2);

    vector2 tile_position = { 0 };

    tile_position = { (float)((2 * (floor)(tile_x / TILE_SIZE) + 1)* TILE_SIZE / 2) - TEMP_WIDTH / 2
        , (float)((2 * (floor)(tile_y / TILE_SIZE) + 1)*TILE_SIZE) / 2 - TEMP_HEIGHT / 2 };
    for (int i = 0; i < 10; i++)
    {
        if (!tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE])
        {
                Normal_Tile(image, tile_x, tile_y, tile_position, type);

            tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE] = true;
        }
    tile_x += num * TILE_SIZE;
    tile_position.x += num * TILE_SIZE;
    }
}
void Tile_Map::MakeTileHorizontal(std::string image, Tile_Type type, int num)
{
    int tile_x = static_cast<int>(Input::GetMousePos().x + TEMP_WIDTH / 2);
    int tile_y = static_cast<int>(Input::GetMousePos().y + TEMP_HEIGHT / 2);

    vector2 tile_position = { 0 };

    tile_position = { (float)((2 * (floor)(tile_x / TILE_SIZE) + 1)* TILE_SIZE / 2) - TEMP_WIDTH / 2
        , (float)((2 * (floor)(tile_y / TILE_SIZE) + 1)*TILE_SIZE) / 2 - TEMP_HEIGHT / 2 };
    for (int i = 0; i < 10; i++)
    {
        if (!tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE])
        {
                Normal_Tile(image, tile_x, tile_y, tile_position, type);

            tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE] = true;
        }
    tile_y -= num * TILE_SIZE;
    tile_position.y -= num * TILE_SIZE;
    }
}

void Tile_Map::Make_Ani_Tile(std::string image, Tile_Type type)
{
	int tile_x = static_cast<int>(Input::GetMousePos().x + TEMP_WIDTH / 2);
	int tile_y = static_cast<int>(Input::GetMousePos().y + TEMP_HEIGHT / 2);

	vector2 tile_position = { 0 };

	if (!tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE])
	{
		tile_position = { (float)((2 * (floor)(tile_x / TILE_SIZE) + 1)* TILE_SIZE / 2) - TEMP_WIDTH / 2
			, (float)((2 * (floor)(tile_y / TILE_SIZE) + 1)*TILE_SIZE) / 2 - TEMP_HEIGHT / 2 };
		
		Ani_Tile(image, tile_x, tile_y, tile_position, type);
		
		tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE] = true;
	}
}

void Tile_Map::Delete_Tile()
{
	int tile_x = static_cast<int>(Input::GetMousePos().x) + TEMP_WIDTH / 2;
	int tile_y = static_cast<int>(Input::GetMousePos().y) + TEMP_HEIGHT / 2;

    if (tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE])
    {
        graphics_tiles.erase((tile_x / TILE_SIZE *TEMP_WIDTH + tile_y / TILE_SIZE));
	physical_tiles.erase((tile_x / TILE_SIZE * TEMP_WIDTH + tile_y / TILE_SIZE));
        tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE] = false;
    }
}

void Tile_Map::Delete_Ani_Tile()
{
	int tile_x = static_cast<int>(Input::GetMousePos().x) + TEMP_WIDTH / 2;
	int tile_y = static_cast<int>(Input::GetMousePos().y) + TEMP_HEIGHT / 2;

	if (tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE])
	{
		graphics_tiles.erase((tile_x / TILE_SIZE * TEMP_WIDTH + tile_y / TILE_SIZE));
		physical_tiles.erase((tile_x / TILE_SIZE * TEMP_WIDTH + tile_y / TILE_SIZE));
		tile_grid[tile_x / TILE_SIZE][tile_y / TILE_SIZE] = false;
	}
}


void Tile_Map::Normal_Tile(std::string& image, int x , int y, vector2 position, Tile_Type type)
{
	Object* tile = new Object();
	tile->SetTranslation({ position });
	tile->SetScale({ static_cast<float>(TILE_SIZE) });
	tile->SetDepth(GAME_OBJECT);
	tile->SetMesh(mesh::CreateBox(1, { 255, 255, 255, 255 }));
	tile->SetObjectType(ObjectType::Tile);
	tile->AddInitComponent(new Sprite(image));

	if (type == Tile_Type::Physical)
	{
		tile->AddInitComponent(new Collision(box_));
		physical_tiles.insert(std::pair<int, Object*>((x / TILE_SIZE * TEMP_WIDTH + y / TILE_SIZE), tile));
	}
	else
		graphics_tiles.insert(std::pair<int, Object*>((x / TILE_SIZE * TEMP_WIDTH + y / TILE_SIZE), tile));
}

void Tile_Map::Ani_Tile(std::string& image, int x, int y, vector2 position, Tile_Type type)
{
	Object* tile = new Object();
	std::string tile_name = "tile";
	tile_name += std::to_string(++m_number);
	tile->SetTranslation({ position });
	tile->SetDepth(GAME_OBJECT);
	tile->SetScale({ static_cast<float>(TILE_SIZE) });
	tile->SetObjectType(ObjectType::Tile);
	tile->SetMesh(mesh::CreateBox(1, { 255, 255, 255, 255 }));
	tile->AddInitComponent(new Animation(image, tile_name, 3, 0.1f, true));

	if (type == Tile_Type::Physical)
	{
		tile->AddInitComponent(new Collision(box_));
		physical_tiles.insert(std::pair<int, Object*>((x / TILE_SIZE * TEMP_WIDTH + y / TILE_SIZE), tile));
	}
	else
		graphics_tiles.insert(std::pair<int, Object*>((x / TILE_SIZE * TEMP_WIDTH + y / TILE_SIZE), tile));
}

void Tile_Map::InsertGraphicalTiles(int grid_, Object * tiles)
{
	graphics_tiles.insert(std::pair<int, Object*>(grid_, tiles));
}

void Tile_Map::InsertPhysicalTiles(int grid_, Object * tiles)
{
	physical_tiles.insert(std::pair<int, Object*>(grid_, tiles));
}


Object * Tile_Map::GetSpecificTile(vector2 position)
{
    int tile_position_x = static_cast<int>(position.x) + TEMP_WIDTH / 2;
    int tile_position_y = static_cast<int>(position.y) + TEMP_HEIGHT / 2;
    if (!physical_tiles.empty())
    {
        if (physical_tiles.find(tile_position_x / TILE_SIZE * TEMP_WIDTH + tile_position_y / TILE_SIZE) == physical_tiles.end())
            return nullptr;
        else
            return physical_tiles.at(tile_position_x / TILE_SIZE * TEMP_WIDTH + tile_position_y / TILE_SIZE);
    }
    return nullptr;
}
