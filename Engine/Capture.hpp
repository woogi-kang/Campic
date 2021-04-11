/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Capture.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once
#include "Component.hpp"
#include <vector>
#include "vector2.hpp"
#include "RandomFunction.hpp"
#include "Object.hpp"
#include "Collision.hpp"
#include "ObjectDepth.hpp"

class Capture : public Component
{
public:
	Capture(vector2 pos) : reset_pos(pos){}

	bool Initialize(Object* Ob) override;
	void Update(float dt) override;
	void Delete() override;

	bool IsCaputreObjectVisible() { return isvisible; }
	void SetVisibleCaptureObj();
	void SetInvisibleCaptureObj();

	void CaptureObjectMove();

    
	void SetResetPosition(vector2 size) { reset_pos = size; }
	vector2 GetResetPosition()const { return reset_pos; }
	void SetFilter(Filter filter) { m_c_filter = filter; }

	Filter GetFilter() { return m_c_filter; }

	bool IsCheese() { return cheese; }
	void SetCheese(bool condition) { cheese = condition; }
	void SetZoomMaxMin(float max, float min = 0.0f);

	static bool IsChangeCaptureCount;
private:
	class Polaroid
	{
	public:
		Polaroid(Object* obj_, vector2 size)
		{
			float angle = RandomNumberGenerator(-45.0f, 45.0f);

			obj = new Object();
			obj->SetTranslation(obj_->GetTransform().GetTranslation());
			obj->SetScale(size * 1.5f);
			obj->SetRotation(angle);
			obj->SetDepth(POLAROID);
			obj->SetObjectType(ObjectType::Polaroid);
			obj->SetMesh(mesh::CreateBox(1, { 255,255,255,200 }));
			obj->AddComponent(new Sprite("asset/images/Objects/polaroid.png"));
		}
		~Polaroid() { delete obj; }

		void Update();
		bool IsDead() { return isdead; }
		Object* GetObject() { return obj; }

	private:
		Object* obj = nullptr;
		bool isdead = false;
	};

	bool IsCaptureArea();
	void Capturing();
	void CreateCaptureObject();
	void CreatePolaroidObject();
	void CameraZoom();
	void CameraZoomInOut();
	void SetOrigianlSize();
    void ZoomObjectUpdate(float dt);

	void SlowMode();

    Filter m_c_filter = Filter::None;

	bool iscreate = false;
	bool cheese = false;
	bool is_runtime_change = false;
	bool isoutside = false;
	bool isvisible = false;

        bool isCollisionSizeBig = false;

	float temp_zoom = 1.0f;
	float zoom = 1.0f;
	float const_zoom = 0.f;
	vector2 reset_pos;
	vector2 temp_local_bar_scale;

	float zoom_max_value = 0.0f;
	float zoom_min_value = 0.0f;

	Object* zoombutton = nullptr;
	Object* zoomobject = nullptr;
	Object* player = nullptr;
	vector2 prev_mouse_pos;

	std::vector<std::pair<vector2, Object*>> original_scale;
	std::vector<Object*> capture_area_contian_object;
	std::vector<std::pair<Object*,vector2>> temporary_obj_storage;
	std::vector<Polaroid*> polaroid_object;

    Object* ground_object = nullptr;
	Object* save_ground_obj = nullptr;
};