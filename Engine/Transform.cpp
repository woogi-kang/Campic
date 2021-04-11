/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Transform.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Transform.hpp"
#include "imgui.h"

affine2d Transform::GetModelToWorld() const
{

	// child
    affine2d temp = build_translation(translation.x, translation.y)
        * rotation_affine(rotation)
        * nonuniform_scale_affine(scale.x, scale.y);

    if (parent != nullptr)
    {
        const Transform* temp_parent_transform = this;
        while (temp_parent_transform->parent)
        {
            affine2d temp_affine2d = build_translation(temp_parent_transform->parent->translation.x,
                                                       temp_parent_transform->parent->translation.y)
                * rotation_affine(temp_parent_transform->parent->rotation)
                * nonuniform_scale_affine(temp_parent_transform->parent->scale.x,
                                          temp_parent_transform->parent->scale.y);

            temp_affine2d *= temp;
            temp = temp_affine2d;
            temp_parent_transform = temp_parent_transform->parent;
        }
    }
    return temp;
}
void Transform::Imgui_Transform()
{
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::InputFloat("Translation.x ", &translation.x, -500, 500);
		ImGui::InputFloat("Translation.y", &translation.y, -500, 500);
		ImGui::InputFloat("Scale.x", &scale.x, 0, 1280);
		ImGui::InputFloat("Scale.y", &scale.y, 0, 1280);
		ImGui::InputFloat("Rotation", &rotation, 0, 370);
		ImGui::InputFloat("Depth", &depth, -1, 1);

		ImGui::TreePop();
	}
}
affine2d Transform::GetTRS()
{
    return build_translation(translation.x,
        translation.y)
        * rotation_affine(rotation)
        *nonuniform_scale_affine(scale.x, scale.y);
}


affine2d Transform::GetWorldToModel() const
{
    affine2d trans = build_translation(translation.x, translation.y);
    affine2d scale_ = nonuniform_scale_affine(scale.x, scale.y);
    affine2d rotate_ = rotation_affine(rotation);

    affine2d temp = trans * rotate_ * scale_;

    const Transform* temp_transform = this;
    if (parent != NULL)
    {
        while (temp_transform->parent)
        {
            affine2d trans_parent =build_translation(-temp_transform->parent->translation.x, -temp_transform->parent->translation.y);
            affine2d scale_parent = nonuniform_scale_affine(1 / temp_transform->parent->scale.x, 1 / temp_transform->parent->scale.y);
            affine2d rotate_parent = rotation_affine(-temp_transform->parent->rotation);

            affine2d temp_parent = scale_parent * rotate_parent * trans_parent;
            temp = temp_parent * temp;

            temp_transform = temp_transform->parent;
        }
        return temp;
    }
    return temp;
}

float Transform::CalculateWorldDepth() const
{
    float temp_depth = depth;

    const Transform* temp_parent = parent;

    if (parent != nullptr)
    {
        while (temp_parent)
        {
            temp_depth += temp_parent->depth;
            temp_parent = temp_parent->parent;
        }
        return temp_depth;
    }
    return temp_depth;
}

float Transform::GetDepth() const
{
    return depth;
}

void Transform::SetDepth(float depth_value)
{
    depth = depth_value;
}

vector2 Transform::GetTranslation() const
{
    return translation;
}

void Transform::SetTranslation(const vector2& translation_affin2d)
{
    translation = translation_affin2d;
}

vector2 Transform::GetScale() const
{
    return scale;
}

void Transform::SetScale(const vector2& scale_value)
{
    scale = scale_value;
}

float Transform::GetRotation() const
{
    return rotation;
}

void Transform::SetRotation(float rotation_value)
{
    rotation = rotation_value;
}

void Transform::SetSpecificPosition(float position, bool is_xpos)
{
    vector2 temp = translation;
    if (is_xpos)
    {
        temp.x = position;
        translation = temp;
    }
    else
    {
        temp.y = position;
        translation = temp;
    }
}

const Transform* Transform::GetParent() const
{
	return parent;
}

void Transform::SetParent(const Transform* transform_parent)
{
	parent = transform_parent;
}
