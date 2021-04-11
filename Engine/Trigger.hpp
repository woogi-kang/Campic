/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Trigger.hpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : SoonWoo Jung
Secondary :
- End Header ----------------------------------------------------------------*/
#pragma once
#include "Component.hpp"
#include "vector2.hpp"
#include "Object.hpp"

enum class TriggerStyle
{
    CheckPoint,
    Door,
    Font,
    Graphic,
    None
};

class Trigger : public Component
{
public:
    Trigger(vector2 translation, vector2 scale, TriggerStyle trigger_style, std::string string = " ", bool istriggered = false,
        float image_frame_ = 0 , float update_frame_ = 0) : o_translation(translation), o_scale(scale),m_trigger_style(trigger_style)
    {
        isTriggered = istriggered;
        text = string;
        image_frame = image_frame_;
        update_frame = update_frame_;
    }

    bool Initialize(Object* Ob);
    void Update(float dt);
    void Delete();

public:
    //When you make function and style, input in this function.
    void ConnectObjectAction();

    void SetIsTriggerd(bool trigger) { isTriggered = trigger; }
    void SetTriggerStyle(TriggerStyle trigger) { m_trigger_style = trigger; }
    void SetObjectTranslation(vector2 obj) { o_translation = obj;}
    void SetConnectedObject(Object* obj) { m_connected_object = obj;}
    void SetText(std::string obj) { text = obj;}
    void SetImageFrame(float obj) { image_frame = obj;}
    void SetUpdateUrame(float obj) { update_frame = obj;}

	vector2 GetScale() { return o_scale; }
    float GetImageFrame() { return image_frame; }
    float GetUpdateFrame() { return update_frame; }
    std::string GetText() { return text; }
    bool GetIsTriggerd() { return isTriggered; }
    Object* GetConnectedObject() { return m_connected_object; }
    TriggerStyle GetTriggerStyle() { return m_trigger_style; }
    vector2 GetObjectTranslation() { return o_translation; }
private:

    //When you want to use trigger, you should make Triggerstyle and function that you want to apply.
    //When you make new item, item ObjectType must be Trigger_Obj.
    //If you want to make Trigger_Obj, you can use translation
    void MakeConnectedDoor();
    void CheckPoint();
    void GraphicPrint();
    void Text();
    std::string text;
    vector2 o_translation; //object_translation
	vector2 o_scale;
    TriggerStyle m_trigger_style;
    float image_frame, update_frame;
    bool isTriggered;
    Object* m_connected_object = nullptr;
};

