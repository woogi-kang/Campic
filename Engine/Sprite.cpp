/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : Sprite.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : JinHyun Choi
Secondary :
- End Header ----------------------------------------------------------------*/

#include "Sprite.hpp"
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#pragma warning(push)
#pragma warning(disable : 4505) // unreferenced local function has been removed
#pragma warning(disable : 4100) // unreferenced formal parameter
#include "stb_image.h"
#pragma warning(pop)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image_write.h"
#include "Animation.hpp"

//Sprite::Sprite(const Sprite& other)
//    : handle_to_texture(other.handle_to_texture), width(other.width),
//      height(other.height)//, path(other.path)
//{
//}

Sprite& Sprite::operator=(const Sprite& other)
{
    if (this == &other)
        return *this;

    handle_to_texture = other.handle_to_texture;
    width = other.width;
    height = other.height;

    return *this;
}

bool Sprite::Initialize(Object* Ob)
{
	if (object == nullptr)
	{
		object = Ob;
		pixel = nullptr;
		handle_to_texture = 0;
		Texture_Load();
	}
    return true;
}

void Sprite::Update(float /*dt*/)
{
}

void Sprite::Delete()
{
    UnLoadSprite();
}

void Sprite::ChangeSprite(const std::string path_)
{
	path = path_;
	Texture_Load();
}

bool Sprite::Texture_Load()
{
	int channels;

    unsigned char* temp = stbi_load(path.c_str(), &width, &height,
                                          &channels, STBI_rgb_alpha);

    if (temp == nullptr)
        return false;

    pixel = temp;

    LoadSprite();
    return true;
}


void Sprite::LoadSprite()
{
    glGenTextures(1, &handle_to_texture);

    glBindTexture(GL_TEXTURE_2D, handle_to_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    const int base_mipmap_level = 0;
    const int zero_border = 0;

    glTexImage2D(GL_TEXTURE_2D, base_mipmap_level, GL_RGBA8, width, height,
                 zero_border, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
}

void Sprite::Bind(unsigned int slot) const
{
    if (handle_to_texture == 0)
        return;

    assert(slot < 32);

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, handle_to_texture);
}

void Sprite::ScreenShot(const std::string& file_path) const
{
	if (!stbi_write_png(file_path.c_str(), width, height, STBI_rgb_alpha, 
		pixel, width*ChannelsPerColor))
		return;
}

void Sprite::UnLoadSprite()
{
    DeleteSpriteTexture();
    pixel = nullptr;
}

void Sprite::DeleteSpriteTexture()
{
    if (handle_to_texture != 0)
    {
        glDeleteTextures(1, &handle_to_texture);
    }
    handle_to_texture = 0;
}

int Sprite::GetPixelsBufferBytesSize() const
{
    return ChannelsPerColor * sizeof(pixel);
}

void Sprite::Imgui_Sprite()
{
	if (ImGui::Button("Flip"))
		flip = !flip;
}
