/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Tile_Map.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : SoonWoo Jung
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include <string>
#include "Input.hpp"
#include "Graphics.hpp"
#include "ObjectDepth.hpp"
#include <vector>
#include "Application.hpp"

const int TILE_SIZE = 60;
const int TEMP_WIDTH = 12800;
const int TEMP_HEIGHT = 10240;

enum class Tile_Type
{
    Physical,
    Graphical
};


class Tile_Map
{
public:
    bool Initialize();
    void Update(float dt);
	void RemoveTiles();

        void MakeGridTure(float pos_x, float pos_y) { tile_grid[static_cast<int>(pos_x + TEMP_WIDTH / 2 ) / TILE_SIZE ]
            [static_cast<int>(pos_y + TEMP_HEIGHT / 2) / TILE_SIZE] = true; }
    void MakeGridFalse()
    {
        for(int i = 0 ; i < TEMP_WIDTH/TILE_SIZE;i++)
        {
            for(int j = 0; j < TEMP_HEIGHT/TILE_SIZE; j++)
            {
                tile_grid[i][j] = false;
            }
        }
    }

    std::unordered_map<int, Object*>& GetGraphicsTiles() { return graphics_tiles; }
    std::unordered_map<int, Object*>& GetPhysicalTiles() { return physical_tiles; }
    void Make_Tile(std::string image, Tile_Type type);
    void MakeTileVertical(std::string image, Tile_Type type, int num);
    void MakeTileHorizontal(std::string image, Tile_Type type, int num);
    void Make_Ani_Tile(std::string image, Tile_Type type);
    void Delete_Tile();
    void Delete_Ani_Tile();
    Object* GetSpecificTile(vector2 position);
    void SetReset(bool reset) { init = reset; }

    void InsertGraphicalTiles(int grid_, Object* tiles);
    void InsertPhysicalTiles(int grid_, Object* tiles);

	void ClearGraphicTiles()
	{
            for(auto iter : graphics_tiles)
            {
                tile_grid[iter.first/TEMP_WIDTH][iter.first%TEMP_WIDTH] = false;
            }
            graphics_tiles.clear();
	}
	void ClearPhysicalTiles() 
    {
            for (auto iter : physical_tiles)
            {
                tile_grid[iter.first / TEMP_WIDTH][iter.first%TEMP_WIDTH] = false;
            }
	    physical_tiles.clear(); 
	}

private:
    void Normal_Tile(std::string& image, int x, int y, vector2 position, Tile_Type type);
    void Ani_Tile(std::string& image, int x, int y, vector2 position, Tile_Type type);

    std::unordered_map<int, Object*> graphics_tiles;
    std::unordered_map<int, Object*> physical_tiles;

    int m_number = 0;
    bool init = false;

    bool tile_grid[TEMP_WIDTH / TILE_SIZE][TEMP_HEIGHT / TILE_SIZE] = {false};
};

extern Tile_Map Tile_Map_;
extern Tile_Map Background_tiles_;
extern Tile_Map Foreground_tiles_;
