/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Camera.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include "Component.hpp"
#include "vector2.hpp"
#include "affine2d.hpp"
#include <string>

class Camera : public Component
{
public:
	Camera() = default;

	Camera(std::string curr_level_)
		: curr_level(curr_level_)
	{
	}
	Camera(vector2 camera_center, vector2 camera_up);

	bool Initialize(Object* Ob) override;
	void Update(float dt) override;
	void Delete() override;

	Object* GetBaseObject() { return base_obj; }

	float GetZoomValue() { return zoom; }
	vector2 GetCenter() const;
	void SetCenter(vector2 camera_center);
	vector2 GetUp() const;
	vector2 GetRight() const;
	std::string GetLevelInfo() { return curr_level; }

	void SetMinMaxSize(vector2 size);

	void ResetUp(vector2 camera_up = { 0,1 });
	void MoveUp(float distance);
	void MoveRight(float distnace);
	void Rotate(float angle);
	void SetBaseObject(Object* obj) { base_obj = obj; }

	affine2d CameraToWorld() const;
	affine2d WorldToCamera() const;

private:
	std::string curr_level;
	float zoom = 1.0f
	;
	Object* base_obj = nullptr;
	vector2 center{};
	vector2 max{};
	vector2 min{};

	vector2 up{ 0,1 };
	vector2 right{ 1,0 };
};
