/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Transform.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once

#include "vector2.hpp"
#include "affine2d.hpp"

class Transform
{
public:
    Transform() = default;
    affine2d GetModelToWorld() const;
    affine2d GetWorldToModel() const;

    float CalculateWorldDepth() const;
    float GetDepth() const;
    void SetDepth(float depth_value);

    vector2 GetTranslation() const;
    void SetTranslation(const vector2& translation_affin2d);

    vector2 GetScale() const;
    void SetScale(const vector2& scale_value);

    float GetRotation() const;
    void SetRotation(float rotation_value);
    void SetSpecificPosition(float position, bool is_xpos = false);

    const Transform* GetParent() const;
    void SetParent(const Transform* transform_parent);

	void Imgui_Transform();
    affine2d GetTRS();
private:
    vector2 translation;
    vector2 scale{1.0f, 1.0f};
    float rotation = 0.0f;
    float depth = 0.0f;
    const Transform* parent = nullptr;
};
