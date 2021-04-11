/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Mesh.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Mesh.hpp"
#include "Animation.hpp"
#include "Sprite.hpp"

Mesh::~Mesh()
{
    Clear();
}

size_t Mesh::GetPointCount() const
{
    return points.size();
}

size_t Mesh::GetTexturePointsCount() const
{
    return textureCoordinates.size();
}

size_t Mesh::GetAnimationPointsCount() const
{
    return animationCoordinates.size();
}

size_t Mesh::GetCollisionPointsCount() const
{
    return collisionCoordinates.size();
}

vector2 Mesh::GetPoint(size_t index) const
{
    return points.at(index);
}

Color Mesh::GetColor(size_t index) const
{
    if (index >= colors.size())
    {
        if (!colors.empty())
            return colors.back();

        return {0, 0, 0};
    }
    return colors[index];
}

std::vector<vector2> Mesh::GetCollisionVectors()
{
	return collisionCoordinates;
}


vector2 Mesh::GetTextureCoordinate(size_t index, Sprite* sprite)
{
	if(!sprite->IsFlip())
	{
		switch (index)
		{
		case 0:
			textureCoordinates.at(index).x = 0;
			break;
		case 1:
			textureCoordinates.at(index).x = 1;
			break;

		case 2:
			textureCoordinates.at(index).x = 0;
			break;

		case 3:
			textureCoordinates.at(index).x = 1;
			break;

		default:
			break;
		}
	}
	else
	{
		switch (index)
		{
		case 0:
			textureCoordinates.at(index).x = 1;
			break;
		case 1:
			textureCoordinates.at(index).x = 0;
			break;

		case 2:
			textureCoordinates.at(index).x = 1;
			break;

		case 3:
			textureCoordinates.at(index).x = 0;
			break;

		default:
			break;
		}
	}
	return textureCoordinates.at(index);
}

vector2 Mesh::GetTextureCoordinate(size_t index)
{
	return textureCoordinates.at(index);
}

vector2 Mesh::GetAnimationCoordinate(size_t index, Animation* animation)
{
	if (!animation->GetCurrentAnimation().sprites->IsFlip())
	{
		switch (index)
		{
		case 0:
			animationCoordinates.at(index).x = animation->GetCurrentAnimation().previous_current_coordinate.x;
			animationCoordinates.at(index).y = 1;
			break;
		case 1:
			animationCoordinates.at(index).x = animation->GetCurrentAnimation().previous_current_coordinate.y;
			animationCoordinates.at(index).y = 1;
			break;
		case 2:
			animationCoordinates.at(index).x = animation->GetCurrentAnimation().previous_current_coordinate.x;
			animationCoordinates.at(index).y = 0;
			break;
		case 3:
			animationCoordinates.at(index).x = animation->GetCurrentAnimation().previous_current_coordinate.y;
			animationCoordinates.at(index).y = 0;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (index)
		{
		case 0:
			animationCoordinates.at(index).x = animation->GetCurrentAnimation().previous_current_coordinate.y;
			animationCoordinates.at(index).y = 1;
			break;
		case 1:
			animationCoordinates.at(index).x = animation->GetCurrentAnimation().previous_current_coordinate.x;
			animationCoordinates.at(index).y = 1;
			break;
		case 2:
			animationCoordinates.at(index).x = animation->GetCurrentAnimation().previous_current_coordinate.y;
			animationCoordinates.at(index).y = 0;
			break;
		case 3:
			animationCoordinates.at(index).x = animation->GetCurrentAnimation().previous_current_coordinate.x;
			animationCoordinates.at(index).y = 0;
			break;
		default:
			break;
		}
	}
    return animationCoordinates.at(index);
}

vector2 Mesh::GetCollisionCoordinate(size_t index) const
{
    return collisionCoordinates.at(index);
}

PointListType Mesh::GetPointListType() const
{
    return point_type;
}

void Mesh::SetPointListType(PointListType list_type)
{
    point_type = list_type;
}

void Mesh::SetAlphaZero()
{
	colors[0].Alpha = 0;
}

void Mesh::SetAlphaFill()
{
	colors[0].Alpha = 255;
}

void Mesh::AddColor(Color color)
{
    colors.push_back(color);
}

void Mesh::AddPoint(vector2 point)
{
    points.push_back(point);
}

void Mesh::AddTextureCoordinate(vector2 texture_coordinate)
{
    textureCoordinates.push_back(texture_coordinate);
}

void Mesh::AddAnimationCoordinate(vector2 animation_coordinate)
{
    animationCoordinates.push_back(animation_coordinate);
}

void Mesh::AddCollisionCoordinate(vector2 collision_coordinate)
{
    collisionCoordinates.push_back(collision_coordinate);
}

void Mesh::ChangeColor(Color color)
{
    for (auto i = colors.begin(); i != colors.end(); ++i)
    {
        i->Red = color.Red;
        i->Green = color.Green;
        i->Blue = color.Blue;
        i->Alpha = color.Alpha;
    }
}

void Mesh::Decrease_Alpha(float dec_value)
{
	if(colors[0].Alpha > 0 )
	{
		float temp = static_cast<float>(colors[0].Alpha);
		temp -= dec_value;
		colors[0].Alpha = static_cast<unsigned char>(temp);
	}
	else
	{
		colors[0].isAlphaDone = true;
	}
}

void Mesh::Increase_Alpha(float inc_value)
{
	if(colors[0].Alpha < 255)
	{
		float temp = static_cast<float>(colors[0].Alpha);
		temp += inc_value;
		colors[0].Alpha = static_cast<unsigned char>(temp);
	}
	else
	{
		colors[0].isAlphaDone = false;
	}
}

void Mesh::ClearColors()
{
    colors.clear();
}

void Mesh::ClearTextureCoordinates()
{
    textureCoordinates.clear();
}

void Mesh::ClearAnimationCoordinates()
{
    animationCoordinates.clear();
}

void Mesh::ClearCollisionCoordinates()
{
    collisionCoordinates.clear();
}

void Mesh::ClearPoints()
{
    points.clear();
}

void Mesh::Clear()
{
    ClearAnimationCoordinates();
    ClearCollisionCoordinates();
    ClearColors();
    ClearPoints();
    ClearTextureCoordinates();
}

namespace
{
    const float PI = 4.0f * std::atan(1.0f);
}

namespace mesh
{
    Mesh CreateCircle(float radius, Color color, size_t points_number)
    {
        Mesh mesh;

        mesh.AddPoint({0, 0});
        mesh.AddTextureCoordinate({0.5f, 0.5f});

        for (int i = 0; i <= (int)points_number; ++i)
        {
            float angle = i * (360.0f / points_number) * PI / 180.0f;
            mesh.AddPoint({radius * cos(angle), radius * sin(angle)});
            mesh.AddColor(color);
        }

        mesh.SetPointListType(PointListType::TriangleFan);

        return mesh;
    }

    Mesh CreateLineCircle(float radius, Color color, size_t points_number)
    {
        Mesh mesh;

        for (int i = 0; i <= (int)points_number; ++i)
        {
            float angle = i * (360.0f / points_number) * PI / 180.0f;
            mesh.AddPoint({radius * cos(angle), radius * sin(angle)});
            mesh.AddColor(color);
        }

        mesh.SetPointListType(PointListType::LineStrip);

        return mesh;
    }

    Mesh CreateBox(float dimension, Color color)
    {
        Mesh mesh;

        mesh.AddPoint({-0.5f * dimension, -0.5f * dimension});
        mesh.AddPoint({0.5f * dimension, -0.5f * dimension});
        mesh.AddPoint({-0.5f * dimension, 0.5f * dimension});
        mesh.AddPoint({0.5f * dimension, 0.5f * dimension});

        mesh.AddTextureCoordinate({0, 1});
        mesh.AddTextureCoordinate({1, 1});
        mesh.AddTextureCoordinate({0, 0});
        mesh.AddTextureCoordinate({1, 0});

        mesh.AddAnimationCoordinate({0, 1});
        mesh.AddAnimationCoordinate({1, 1});
        mesh.AddAnimationCoordinate({0, 0});
        mesh.AddAnimationCoordinate({1, 0});


        mesh.SetPointListType(PointListType::TriangleStrip);

        mesh.AddColor(color);

        return mesh;
    }

    Mesh CreateConvex(float dimension, Color color, size_t points_number)
    {
        Mesh mesh;

        mesh.AddPoint({0, 0});

        for (int i = 0; i <= (int)points_number; ++i)
        {
            float angle = i * (360.0f / points_number) * PI / 180.0f;
            mesh.AddPoint({dimension * cos(angle), dimension * sin(angle)});
            mesh.AddColor(color);
        }

        mesh.SetPointListType(PointListType::TriangleFan);
        return mesh;
    }

    Mesh CreateLineBox(float dimension, Color color)
    {
        Mesh mesh;

        mesh.AddPoint(dimension * vector2{-0.5f, -0.5f});
        mesh.AddPoint(dimension * vector2{0.5f, -0.5f});
        mesh.AddPoint(dimension * vector2{0.5f, 0.5f});
        mesh.AddPoint(dimension * vector2{-0.5f, 0.5f});
        mesh.AddPoint(dimension * vector2{-0.5f, -0.5f});

        mesh.SetPointListType(PointListType::LineStrip);

        mesh.AddColor(color);

        return mesh;
    }

    Mesh CrateLine(vector2 point_a, vector2 point_b, Color color)
    {
        Mesh mesh;

        mesh.AddPoint(point_a);
        mesh.AddPoint(point_b);

        mesh.SetPointListType(PointListType::Lines);

        mesh.AddColor(color);

        return mesh;
    }

    Mesh CreateCollisionBox(CollisionType type, float dimension, Color color)
    {
		Mesh mesh;
        switch (type)
        {
        case box_:
        {
            mesh.AddCollisionCoordinate(dimension*vector2{ -0.5f,-0.5f });
            mesh.AddCollisionCoordinate(dimension*vector2{ 0.5f,-0.5f });
            mesh.AddCollisionCoordinate(dimension*vector2{ 0.5f,0.5f });
            mesh.AddCollisionCoordinate(dimension*vector2{ -0.5f,0.5f });
            mesh.AddCollisionCoordinate(dimension*vector2{ -0.5f,-0.5f });

            mesh.SetPointListType(PointListType::LineStrip);

            mesh.AddColor(color);
        }
		break;

        case circle_:
            {   
                int points_number = 100;
                for (int i = 0; i <= points_number; ++i)
                {
                    float angle = i * (360.0f / points_number) * PI / 180.0f;
                    mesh.AddCollisionCoordinate({dimension * cos(angle), dimension * sin(angle)});
                    mesh.AddColor(color);
                }

                mesh.SetPointListType(PointListType::LineStrip);
            }
			break;
        case triangle_:
            {
                mesh.AddCollisionCoordinate(dimension * vector2{-0.5f, -0.5f});
                mesh.AddCollisionCoordinate(dimension * vector2{0.5f, -0.5f});
                mesh.AddCollisionCoordinate(dimension * vector2{0.0f, 0.5f});
                mesh.AddCollisionCoordinate(dimension * vector2{-0.5f, -0.5f});

                mesh.SetPointListType(PointListType::LineStrip);

                mesh.AddColor(color);
            }
			break;
        }
		return mesh;
    }
}
