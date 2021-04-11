/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Animation.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#pragma once
#include "Component.hpp"
#include "Sprite.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include "vector2.hpp"
#include "imgui.h"
#include <algorithm>

class Object;

class Animation_Information
{
public:
    Animation_Information() = default;

    Animation_Information(const std::string path_, std::string ID, int image_frame_,
        float update_frame_, bool repeat = true)
        :  id(ID), path(path_), image_frames(image_frame_), update_frames(update_frame_), is_repeats(repeat)
    {
        sprites = new Sprite(path);

        frame_per_seconds = 1.0f / (float)image_frames;
    };

	std::string id;
    Sprite * sprites = nullptr;
	std::string path;
    int image_frames = 0;
    float update_frames = 0;
    float frame_per_seconds = 0;
    bool is_repeats = false;
    vector2 previous_current_coordinate{};
};

class Animation : public Component
{
public:
    Animation(std::string path_, std::string ID, int image_frame_, 
		float update_frame_, bool repeat = true )
    {
        current_animation = Animation_Information(path_, ID, image_frame_, update_frame_, repeat);
		current_animation.sprites->Texture_Load();
		current_animation.id = ID;

        current_animation.previous_current_coordinate.x = 0;
		current_animation.previous_current_coordinate.y = current_animation.frame_per_seconds;

        animations.insert(std::make_pair(ID, current_animation));

    };

	Animation(const Animation& copy_animation)
		: Component(copy_animation)
	{
		current_animation = Animation_Information(copy_animation.current_animation);
		current_animation.sprites = new Sprite(copy_animation.current_animation.sprites->GetPath());

		if(copy_animation.current_animation.sprites->IsFlip())
		{
			current_animation.sprites->SetFlip(true);
		}
		else
		{
			current_animation.sprites->SetFlip(false);
		}

		current_animation.sprites->Texture_Load();

		current_animation.previous_current_coordinate.x = 0;
		current_animation.previous_current_coordinate.y = current_animation.frame_per_seconds;

		animations.insert(std::make_pair(current_animation.id, current_animation));
	}

    bool Initialize(Object* Ob) override;
    void Update(float dt) override;    
	void Delete() override;

	void AddAnimaition(const std::string path, const std::string ID, 
	int image_frame_, float update_frame_, bool repeat = true);

	void ResetAnimaition();
	void ChangeAnimation(std::string ID, std::string original_ID = std::string{});
	void SetIsActive(bool condition);
	void SetIsDone(bool condition) { is_done = condition; };

	bool IsDone() { return is_done; }
	bool IsActive() { return isactive; }

	void SetFlip(bool condition) { current_animation.sprites->SetFlip(condition); }
	bool IsFiip() { return current_animation.sprites->IsFlip(); }

	void Imgui_Animation();

    Animation_Information GetCurrentAnimation() { return current_animation; }
	Animation_Information GetPreviousAnimation() { return previous_animation; }
	std::unordered_map<std::string, Animation_Information> GetAnimationMap(){ return animations; }
	float GetFrameTime() { return frame_time; }
 
private:
    Animation_Information current_animation;
	Animation_Information previous_animation;
    std::unordered_map<std::string, Animation_Information> animations;

	bool isactive = true;
	bool is_done = false;
    float frame_time = 0;
	bool flip = false;
};