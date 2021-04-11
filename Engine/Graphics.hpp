/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Graphics.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include "Shader.hpp"
#include "Transform.hpp"
#include "Object.hpp"
#include <vector>
#include "Font.hpp"
#include "Particle_Generator.hpp"

class Sprite;
class ObjectManager;

enum class GraphicsType : int
{
	solid_obj,
	sprite,
	particle,
	font,
	count
};

class Graphics
{
public:
	bool Initialize();
	void Update(float dt);
	void Quit();

	Camera* GetCurrentCamera() { return game_level_camera; }
	void BeginDraw();
	void Draw();
	void HUD_Draw();
    void Tile_Draw();
    void Button_Draw();
	void EndDraw();
	bool IsDraw(Object* obj);

	void SetNDC();
	void SetHUD_NDC();

	vector2 GetDisplaySize() { return displaysize; }
	static vector2 camera_center;
private:
	struct solidshape
	{
		vector2 position;
	};
	struct texture
	{
		vector2 position;
		vector2 textureCoordinate;
	};
	struct animaition
	{
		vector2 position;
		vector2 animationCoordinate;
	};
	struct collsionbox
	{
		vector2 position;
	};
	struct particle
	{
		vector2 position;
		vector2 particleCoordinate;
	};
	struct font
	{
		vector2 position;
		vector2 fontCoordinate;
	};


	affine2d CalculateModelToNDCTransform(const Transform& transform, bool is_hud = false) const;

	void DrawSolidShape(Object* obj , bool is_hud = false);

	void DrawSprite(Object* obj, Sprite* sprite_, bool is_hud = false);

	void DrawCollisionBox(Object* obj, Collision* collision, bool is_hud=false);

	void DrawAnimation(Object* obj, Animation* animation_, bool is_hud=false);

	void DrawParticle(Particle_Generator* particles, bool is_hud = false);

	void DrawFont(Object* obj, Font* font, bool is_hud=false);

	void DescribSolidVertexPosition();
	void DescribVertexPosition();
	void DescribParticlePosition();
	void DescribFontPosition();

private:
	bool Iscamera = false;
	Camera* game_level_camera = nullptr;

	static const int NumberOfVertexTypes = (int)GraphicsType::count;
	vector2 displaysize{};
	vector2 display_max_size{};

	affine2d projection = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	affine2d hud_projection = { 1, 0, 0, 0, 1, 0, 0,0,1 };
	unsigned int lastBoundTexture = 0;

	Shader Solidshader{};
	Shader Spriteshader{};
	Shader Particleshader{};
	Shader Fontshader{};

	unsigned vertexAttributes[NumberOfVertexTypes] = { 0 };
	unsigned vertexBuffer[NumberOfVertexTypes] = { 0 };

	std::vector<solidshape> shapes{};
	std::vector<texture> sprite{};
	std::vector<animaition> animation{};
	std::vector<collsionbox> collsionboxes{};
	std::vector<particle> particles{};
	std::vector<font> fontes{};
};

extern Graphics Graphics_;