/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Mesh.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once

#include "vector2.hpp"
#include <vector>
#include "Color.hpp"

enum CollisionType
{
	box_,
	circle_,
	triangle_
};

class Animation;
class Sprite;

enum class PointListType
{
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan
};

class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

	// Get Total Count about Coordinate
    size_t GetPointCount() const;
    size_t GetTexturePointsCount() const;
    size_t GetAnimationPointsCount() const;
    size_t GetCollisionPointsCount() const;

	// Get Specific index of vertex
    vector2 GetPoint(size_t index) const;
    vector2 GetTextureCoordinate(size_t index, Sprite* sprite);
	vector2 GetTextureCoordinate(size_t index);
    vector2 GetAnimationCoordinate(size_t index, Animation* animation);
    vector2 GetCollisionCoordinate(size_t index) const;
    Color GetColor(size_t index) const;

    std::vector<vector2> GetCollisionVectors();

	// Add Coordinate
    void AddColor(Color color);
    void AddPoint(vector2 point);
    void AddTextureCoordinate(vector2 texture_coordinate);
    void AddAnimationCoordinate(vector2 animation_coordinate);
    void AddCollisionCoordinate(vector2 collision_coordinate);

	// PointListType
    PointListType GetPointListType() const;
    void SetPointListType(PointListType list_type);

	// Color
	void SetAlphaZero();
	void SetAlphaFill();
    void ChangeColor(Color color);
    void Decrease_Alpha(float dec_value);
    void Increase_Alpha(float inc_value);

	// Clear
    void ClearColors();
    void ClearTextureCoordinates();
    void ClearAnimationCoordinates();
    void ClearCollisionCoordinates();
    void ClearPoints();

	// Visible
    bool IsVisible() { return visible; }
    void Invisible() { visible = false; }
    void Visible() { visible = true; }

    void Clear();

private:
    bool visible = true;
    std::vector<vector2> points{};
    std::vector<vector2> textureCoordinates{};
    std::vector<vector2> animationCoordinates{};
    std::vector<vector2> collisionCoordinates{};

    std::vector<Color> colors{};

    PointListType point_type = PointListType::Lines;
};

namespace mesh
{	
    Mesh CreateCircle(float radius = 1, Color color = {255, 255, 255}, size_t points_number = 30);
    Mesh CreateLineCircle(float radius = 1, Color color = {255, 255, 255}, size_t points_number = 30);
    Mesh CreateBox(float dimension = 1, Color color = {255, 255, 255});
    Mesh CreateConvex(float dimension = 1, Color color = { 255,255,255 }, size_t points_number = 5);
    Mesh CreateLineBox(float dimension = 1, Color color = {255, 255, 255});
    Mesh CrateLine(vector2 point_a = {0, 0}, vector2 point_b = {1, 0}, Color color = {255, 255, 255});
    Mesh CreateCollisionBox(CollisionType type = box_, float dimension = 1, Color color = { 255,0,0 });
    Mesh CreateFont(float dimension = 1, Color color = { 255,255,255 });
}