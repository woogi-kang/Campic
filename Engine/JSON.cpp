/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name : JSON.cpp
Language  : C++
Platform  : Visual Studio 2017
Project   : CamPic
Primary   : TaeWook Kang
Secondary :
- End Header ----------------------------------------------------------------*/
#include "JSON.hpp"
#include "Animation.hpp"
#include <iostream>
#include "Particle.hpp"
#include "Particle_Generator.hpp"
#include "Font.hpp"
#include "Capture.hpp"
#include "UI.hpp"
#include "Trigger.hpp"
#include "Chapter.hpp"

JSON JSON_;

bool JSON::Initialize()
{
	ObjectDocument.SetObject();
	TileDocument.SetObject();

	return true;
}

void JSON::Update(float /*dt*/)
{
}

void JSON::Quit()
{
}

void JSON::ObjectsToDocument(Object* obj, const std::string& file, const std::string& path)
{
    if(obj->GetObjectType() == ObjectType::Capture_Camera ||obj->GetObjectType() == ObjectType::Player
		|| obj->GetObjectType() == ObjectType::Capture_Camera_main
        || obj->GetObjectType() == ObjectType::Capture_Obj || obj->GetObjectType() == ObjectType::Polaroid 
        || obj->GetObjectType() == ObjectType::Trigger_Obj || obj->GetObjectType() == ObjectType::Camera
        || obj->GetObjectType() == ObjectType::Bus || obj->GetObjectType() == ObjectType::Background)
        return;
	//Trees for object info
	Value objTree(kArrayType);

	//Status info
	Value objTransformTree(kArrayType);
	Value objStatusTree(kArrayType);
	Value objSpriteTree(kArrayType);
	Value objAnimationTree(kArrayType);
	Value objCameraTree(kArrayType);
	Value objRigidBodyTree;
	Value objCollisionTree(kArrayType);
	Value objParticleTree(kArrayType);
	Value objSoundTree(kArrayType);
	Value objFontTree(kArrayType);
	Value objCaptureTree(kArrayType);
	Value objTriggerTree(kArrayType);
	Value objChapterTree(kArrayType);
	Value objUItree;
	Value capture;
	Value objUi;
	
	objTree.SetObject();
	objTransformTree.SetObject();
	objStatusTree.SetObject();
	objAnimationTree.SetObject();
	objCameraTree.SetObject();
	objSpriteTree.SetObject();
	objRigidBodyTree.SetObject();
	objCollisionTree.SetObject();
	objParticleTree.SetObject();
	objSoundTree.SetObject();
	objFontTree.SetObject();
	objCaptureTree.SetObject();
	objTriggerTree.SetObject();
	capture.SetObject();
	objUItree.SetObject();
	objUi.SetObject();
	objChapterTree.SetObject();

	objTransformTree = ComponentTransform(obj);
	objStatusTree = ComponentStatus(obj);

	if(obj->GetComponentByTemplate<Animation>() != nullptr)
		objAnimationTree = ComponentAnimation(obj);

	if(obj->GetComponentByTemplate<Camera>() != nullptr)
		objCameraTree = ComponentCamera(obj);
	
	if(obj->GetComponentByTemplate<Sprite>() != nullptr)
		objSpriteTree = ComponentSprite(obj);
	
	if (obj->GetComponentByTemplate<RigidBody>() != nullptr)
		objRigidBodyTree.SetBool(true);
	else
		objRigidBodyTree.SetBool(false);

	if(obj->GetComponentByTemplate<Collision>() != nullptr)
		objCollisionTree = ComponentCollision(obj);
	
	if(obj->GetComponentByTemplate<Particle_Generator>() != nullptr)
		objParticleTree = ComponentParticle(obj);

	if (obj->GetComponentByTemplate<Sound>() != nullptr)
		objSoundTree = ComponentSound(obj);

	if (obj->GetComponentByTemplate<Font>() != nullptr)
		objFontTree = ComponentFont(obj);

	if (obj->GetComponentByTemplate<Capture>() != nullptr)
	{
		capture.AddMember("x", obj->GetComponentByTemplate<Capture>()->GetResetPosition().x, ObjectDocument.GetAllocator());
		capture.AddMember("y", obj->GetComponentByTemplate<Capture>()->GetResetPosition().y, ObjectDocument.GetAllocator());

		objCaptureTree.AddMember("pos", capture, ObjectDocument.GetAllocator());
	}

    if(obj->GetComponentByTemplate<Trigger>() != nullptr)
        objTriggerTree = ComponentTrigger(obj);

	if (obj->GetComponentByTemplate<UI>() != nullptr)
	{
		std::string check = obj->GetComponentByTemplate<UI>()->GetId().c_str();
		objUi.SetString(obj->GetComponentByTemplate<UI>()->GetId().c_str(), ObjectDocument.GetAllocator());
		objUItree.AddMember("id", objUi, ObjectDocument.GetAllocator());
	}

	if(obj->GetComponentByTemplate<Chapter>() != nullptr)
	{
		int chap = obj->GetComponentByTemplate<Chapter>()->GetChapter();
		int lev = obj->GetComponentByTemplate<Chapter>()->GetLevel();

		objChapterTree.AddMember("chapter", chap, ObjectDocument.GetAllocator());
		objChapterTree.AddMember("level", lev, ObjectDocument.GetAllocator());
	}

	objTree.AddMember("Type", objStatusTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Transform", objTransformTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Sprite", objSpriteTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Animation", objAnimationTree, ObjectDocument.GetAllocator());
	objTree.AddMember("RigidBody", objRigidBodyTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Collision", objCollisionTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Particle", objParticleTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Sound", objSoundTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Font", objFontTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Capture", objCaptureTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Trigger", objTriggerTree, ObjectDocument.GetAllocator());
	objTree.AddMember("UI", objUItree, ObjectDocument.GetAllocator());
	objTree.AddMember("Camera", objCameraTree, ObjectDocument.GetAllocator());
	objTree.AddMember("Chapter", objChapterTree, ObjectDocument.GetAllocator());

	ObjectDocument.AddMember("Object", objTree, ObjectDocument.GetAllocator());

	ObjectDocument.Parse(ObjectBuffer.GetString());

	SaveObjectsToJson(file, path);
}

Value JSON::ComponentAnimation(Object* obj)
{
	Value objAnimationTree(kArrayType);
	Value objtree(kArrayType);
	Value current_animation, previous_animation;
	std::vector<Value*> animation_string;
	std::vector<Value*> animation_map;

	//Animation_Information class
	objAnimationTree.SetObject();
	current_animation.SetObject();
	previous_animation.SetObject();
	objtree.SetObject();

	auto animation_info = obj->GetComponentByTemplate<Animation>();
	int i = 0;
	for (auto& temp : animation_info->GetAnimationMap())
	{
		Value animation_path, id, path;
		Value animation_info_;
		Value animation_is_repeats;
		Value animation_pc_coordinate;
		Value objAnimationMap;

		id.SetObject();
		path.SetObject();
		objAnimationMap.SetObject();
		animation_path.SetObject();
		animation_info_.SetObject();
		animation_is_repeats.SetObject();
		animation_pc_coordinate.SetObject();

		id.SetString(temp.first.c_str(), ObjectDocument.GetAllocator());
		path.SetString(temp.second.path.c_str(), ObjectDocument.GetAllocator());

		animation_path.AddMember("id", id, ObjectDocument.GetAllocator());
		animation_path.AddMember("path", path, ObjectDocument.GetAllocator());
		animation_is_repeats.SetBool(temp.second.is_repeats);
		animation_info_.AddMember("image_frames", temp.second.image_frames, ObjectDocument.GetAllocator());
		animation_info_.AddMember("update_frames", temp.second.update_frames, ObjectDocument.GetAllocator());
		animation_info_.AddMember("is_repeats", animation_is_repeats, ObjectDocument.GetAllocator());

		animation_string.push_back(&animation_path);
		animation_map.push_back(&animation_info_);

		objAnimationMap.AddMember("path", *animation_string.at(i), ObjectDocument.GetAllocator());
		objAnimationMap.AddMember("info", *animation_map.at(i), ObjectDocument.GetAllocator());
		
		objtree.AddMember("map", objAnimationMap, ObjectDocument.GetAllocator());
		++i;
	}

	objAnimationTree.AddMember("maps", objtree, ObjectDocument.GetAllocator());

	return objAnimationTree;
}

Value JSON::ComponentTransform(Object * obj)
{
	Value objTransformTree(kArrayType);
	Value pos, scale, rotation, depth;

	objTransformTree.SetObject();
	pos.SetObject();
	scale.SetObject();
	rotation.SetObject();
	depth.SetObject();

	pos.AddMember("x", obj->GetTransform().GetTranslation().x, ObjectDocument.GetAllocator());
	pos.AddMember("y", obj->GetTransform().GetTranslation().y, ObjectDocument.GetAllocator());

	scale.AddMember("x", obj->GetTransform().GetScale().x, ObjectDocument.GetAllocator());
	scale.AddMember("y", obj->GetTransform().GetScale().y, ObjectDocument.GetAllocator());

	rotation.SetFloat(obj->GetTransform().GetRotation());

	if (obj->GetObjectType() == ObjectType::Background)
		depth.SetFloat(0.1f);
	else if (obj->GetObjectType() == ObjectType::Button)
		depth.SetFloat(-0.1f);
	else
		depth.SetFloat(obj->GetTransform().GetDepth());

	objTransformTree.AddMember("position", pos, ObjectDocument.GetAllocator());
	objTransformTree.AddMember("scale", scale, ObjectDocument.GetAllocator());
	objTransformTree.AddMember("rotation", rotation, ObjectDocument.GetAllocator());
	objTransformTree.AddMember("depth", depth, ObjectDocument.GetAllocator());

	return objTransformTree;
}

Value JSON::ComponentStatus(Object * obj)
{
	Value objTypeTree(kArrayType);
	Value isDead, type;

	objTypeTree.SetObject();
	isDead.SetObject();
	type.SetObject();

	isDead.SetBool(obj->IsDead());
	type.SetInt(static_cast<int>(obj->GetObjectType()));

	objTypeTree.AddMember("type", type, ObjectDocument.GetAllocator());
	objTypeTree.AddMember("isDead", isDead, ObjectDocument.GetAllocator());
	return objTypeTree;
}

Value JSON::ComponentCamera(Object* obj)
{
	Value objCameraTree(kArrayType);
	Value level_name, zoom,center, up, right;

	objCameraTree.SetObject();
	level_name.SetObject();
	center.SetObject();
	up.SetObject();
	right.SetObject();

	auto obj_info = obj->GetComponentByTemplate<Camera>();

	center.AddMember("x", obj_info->GetCenter().x, ObjectDocument.GetAllocator());
	center.AddMember("y", obj_info->GetCenter().y, ObjectDocument.GetAllocator());
	up.AddMember("x", obj_info->GetUp().x, ObjectDocument.GetAllocator());
	up.AddMember("y", obj_info->GetUp().y, ObjectDocument.GetAllocator());
	right.AddMember("x", obj_info->GetRight().x, ObjectDocument.GetAllocator());
	right.AddMember("y", obj_info->GetRight().y, ObjectDocument.GetAllocator());
	level_name.SetString(obj->GetComponentByTemplate<Camera>()->GetLevelInfo().c_str(),ObjectDocument.GetAllocator());

	objCameraTree.AddMember("zoom", obj_info->GetZoomValue(), ObjectDocument.GetAllocator());
	objCameraTree.AddMember("center", center, ObjectDocument.GetAllocator());
	objCameraTree.AddMember("up", up, ObjectDocument.GetAllocator());
	objCameraTree.AddMember("right", right, ObjectDocument.GetAllocator());
	objCameraTree.AddMember("level", level_name, ObjectDocument.GetAllocator());

	return objCameraTree;
}

Value JSON::ComponentSprite(Object* obj)
{
	Value objSpriteTree(kArrayType);
	Value path, flip;

	objSpriteTree.SetObject();
	path.SetObject();

	auto obj_info = obj->GetComponentByTemplate<Sprite>();

	path.SetString(obj_info->GetPath().c_str(), ObjectDocument.GetAllocator());
	flip.SetBool(obj_info->IsFlip());

	objSpriteTree.AddMember("is_flip", flip, ObjectDocument.GetAllocator());
	objSpriteTree.AddMember("image_path", path, ObjectDocument.GetAllocator());

	return objSpriteTree;
}

Value JSON::ComponentCollision(Object * obj)
{
	Value collision_type;
	collision_type.SetObject();

	auto type = obj->GetComponentByTemplate<Collision>()->GetCollisionType();

	switch(type)
	{
		case CollisionType::box_ :
		{
			collision_type.AddMember("id", static_cast<int>(type), ObjectDocument.GetAllocator());
			collision_type.AddMember("enum", "box", ObjectDocument.GetAllocator());
			break;
		}
		case CollisionType::circle_:
		{
			collision_type.AddMember("id", static_cast<int>(type), ObjectDocument.GetAllocator());
			collision_type.AddMember("enum", "circle", ObjectDocument.GetAllocator());
			break;
		}
		case CollisionType::triangle_:
		{
			collision_type.AddMember("id", static_cast<int>(type), ObjectDocument.GetAllocator());
			collision_type.AddMember("enum", "triangle", ObjectDocument.GetAllocator());
			break;
		}
		default:
			break;
	}

	return collision_type;
}

Value JSON::ComponentParticle(Object * obj)
{
	Value particleTree(kArrayType);
	Value start_velocity, random_velocity, particle_size, emit_size, path, isActive;
	Value fire_type, m_id, m_enum;
	
	particleTree.SetObject();
	start_velocity.SetObject();
	random_velocity.SetObject();
	particle_size.SetObject();
	emit_size.SetObject();
	isActive.SetObject();
	path.SetObject();
	fire_type.SetObject();
	m_id.SetObject();
	m_enum.SetObject();

	auto particle_info = obj->GetComponentByTemplate<Particle_Generator>();

	particleTree.AddMember("emit_rate", particle_info->GetEmitRate(), ObjectDocument.GetAllocator());
	particleTree.AddMember("life_time", particle_info->GetLifeTimeControl(), ObjectDocument.GetAllocator());
	particleTree.AddMember("size_variance", particle_info->GetSizeVarianceControl(), ObjectDocument.GetAllocator());
	particleTree.AddMember("color_duration", particle_info->GetColorDuration(), ObjectDocument.GetAllocator());

	start_velocity.AddMember("x", particle_info->GetStartVelocity().x, ObjectDocument.GetAllocator());
	start_velocity.AddMember("y", particle_info->GetStartVelocity().y, ObjectDocument.GetAllocator());
	random_velocity.AddMember("x", particle_info->GetRandomVelocity().x, ObjectDocument.GetAllocator());
	random_velocity.AddMember("y", particle_info->GetRandomVelocity().y, ObjectDocument.GetAllocator());
	particle_size.AddMember("x", particle_info->GetParticleSize().x, ObjectDocument.GetAllocator());
	particle_size.AddMember("y", particle_info->GetParticleSize().y, ObjectDocument.GetAllocator());
	emit_size.AddMember("x", particle_info->GetEmitSize().x, ObjectDocument.GetAllocator());
	emit_size.AddMember("y", particle_info->GetEmitSize().y, ObjectDocument.GetAllocator());
	isActive.SetBool(particle_info->GetIsActive());
	path.SetString(particle_info->GetPath().c_str(), ObjectDocument.GetAllocator());

	auto m_type = particle_info->GetParticle_Fire_Type();

	switch(m_type)
	{
		case Particle_Fire_Type::Divergent :
		{
			m_id.SetInt(0);
			m_enum.SetString("Divergent");
			break;
		}
		case Particle_Fire_Type::Integrate :
		{
			m_id.SetInt(1);
			m_enum.SetString("Integrate");
			break;
		}
		case Particle_Fire_Type::OneWay :
		{
			m_id.SetInt(2);
			m_enum.SetString("OneWay");
			break;
		}

		default :
			break;
	}

	fire_type.AddMember("id", m_id, ObjectDocument.GetAllocator());
	fire_type.AddMember("enum", m_enum, ObjectDocument.GetAllocator());

	particleTree.AddMember("start_velocity", start_velocity, ObjectDocument.GetAllocator());
	particleTree.AddMember("random_velocity", random_velocity, ObjectDocument.GetAllocator());
	particleTree.AddMember("particle_size", particle_size, ObjectDocument.GetAllocator());
	particleTree.AddMember("emit_size", emit_size, ObjectDocument.GetAllocator());
	particleTree.AddMember("isActive", isActive, ObjectDocument.GetAllocator());
	particleTree.AddMember("path", path, ObjectDocument.GetAllocator());
	particleTree.AddMember("fire_type", fire_type, ObjectDocument.GetAllocator());

	return particleTree;
}

Value JSON::ComponentSound(Object * obj)
{
	Value container(kArrayType);
	Value map,paths;

	container.SetObject();
	map.SetObject();
	paths.SetObject();

	auto sound_info = obj->GetComponentByTemplate<Sound>();
	auto path_container = sound_info->GetSoundPaths();

	for(auto& temp : path_container)
	{
		paths.SetString(temp.c_str(), ObjectDocument.GetAllocator());
		map.AddMember("path", paths, ObjectDocument.GetAllocator());
	}

	container.AddMember("map", map, ObjectDocument.GetAllocator());

	return container;
}

Value JSON::ComponentFont(Object * obj)
{
	Value container(kArrayType);
	Value paths, text;

	container.SetObject();
	paths.SetObject();
	text.SetObject();

	auto font_info = obj->GetComponentByTemplate<Font>();

	//text.SetString(font_info->GetString.c_str(), ObjectDocument.GetAllocator());
	//paths.SetString(font_info->GetFont().c_str(), ObjectDocument.GetAllocator());

	container.AddMember("text", text, ObjectDocument.GetAllocator());
	container.AddMember("path", paths, ObjectDocument.GetAllocator());

	return container;
}

Value JSON::ComponentTrigger(Object * obj)
{
    Value container(kArrayType);
    Value translation, scale, text, type, istrigger, i_frame, u_frame;

    container.SetObject();
    translation.SetObject();
	scale.SetObject();
    type.SetObject();
    text.SetObject();
    istrigger.SetObject();
    i_frame.SetObject();
    u_frame.SetObject();

    auto info = obj->GetComponentByTemplate<Trigger>();

    translation.AddMember("x", info->GetObjectTranslation().x, ObjectDocument.GetAllocator());
    translation.AddMember("y", info->GetObjectTranslation().y, ObjectDocument.GetAllocator());
	scale.AddMember("x", info->GetScale().x, ObjectDocument.GetAllocator());
	scale.AddMember("y", info->GetScale().y, ObjectDocument.GetAllocator());
    type.SetInt(static_cast<int>(info->GetTriggerStyle()));
    i_frame.SetFloat(info->GetImageFrame());
    u_frame.SetFloat(info->GetUpdateFrame());
    text.SetString(info->GetText().c_str(), ObjectDocument.GetAllocator());
    istrigger.SetBool(info->GetIsTriggerd());

    container.AddMember("translation", translation, ObjectDocument.GetAllocator());
	container.AddMember("scale", scale, ObjectDocument.GetAllocator());
    container.AddMember("type", type, ObjectDocument.GetAllocator());
    container.AddMember("text", text, ObjectDocument.GetAllocator());
    container.AddMember("trigger", istrigger, ObjectDocument.GetAllocator());
    container.AddMember("u_frame", u_frame, ObjectDocument.GetAllocator());
    container.AddMember("i_frame", i_frame, ObjectDocument.GetAllocator());

    return container;
}

void JSON::SaveObjectsToJson(const std::string& file, const std::string& path)
{
	std::string filename(file_path);
    filename.append(file);
	filename.append(path);
	filename.append(".json");

	FILE* fp = fopen(filename.c_str(), "w+");
		
	char writeBuffer[65535];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	PrettyWriter<FileWriteStream> writer(os);
	ObjectDocument.Accept(writer);
	
	fclose(fp);
}

Document JSON::LoadObjectDocumentFromJson(const std::string& file, const std::string& path)
{
	std::string filename(file_path);
	filename.append(file);
	filename.append(path);
	filename.append(".json");

	FILE* fp = fopen(filename.c_str(), "r+");

	char readBuffer[65535];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document object_lists;
	object_lists.ParseStream(is);

	fclose(fp);

	return object_lists;


}

void JSON::TilesToDocument(int grid_, Object * obj, Tile_Type type, const std::string& path)
{
	Value tileTree(kArrayType);

	Value grid, collision, sprite, animation, scale, pos, tile_type, IsAnimated;

	tileTree.SetObject();
	grid.SetObject();
	scale.SetObject();
	pos.SetObject();
	sprite.SetObject();
	animation.SetObject();
	tile_type.SetObject();
	IsAnimated.SetObject();
	collision.SetObject();

	grid.SetInt(grid_);
	tile_type.SetInt(static_cast<int>(type));
	scale.AddMember("x", obj->GetTransform().GetScale().x, TileDocument.GetAllocator());
	scale.AddMember("y", obj->GetTransform().GetScale().y, TileDocument.GetAllocator());
	pos.AddMember("x", obj->GetTransform().GetTranslation().x, TileDocument.GetAllocator());
	pos.AddMember("y", obj->GetTransform().GetTranslation().y, TileDocument.GetAllocator());

	if (obj->GetComponentByTemplate<Sprite>() != nullptr)
	{
		sprite = ComponentSprite(obj);
		IsAnimated.SetBool(false);
	}

	if (obj->GetComponentByTemplate<Animation>() != nullptr)
	{
		animation = ComponentAnimation(obj);
		IsAnimated.SetBool(true);
	}

	if (obj->GetComponentByTemplate<Collision>() != nullptr)
		collision = ComponentCollision(obj);

	tileTree.AddMember("grid", grid, TileDocument.GetAllocator());
	tileTree.AddMember("tile_type", tile_type, TileDocument.GetAllocator());
	tileTree.AddMember("scale", scale, TileDocument.GetAllocator());
	tileTree.AddMember("pos", pos, TileDocument.GetAllocator());
	tileTree.AddMember("sprite", sprite, TileDocument.GetAllocator());
	tileTree.AddMember("animation", animation, TileDocument.GetAllocator());
	tileTree.AddMember("IsAnimated", IsAnimated, TileDocument.GetAllocator());
	tileTree.AddMember("collision", collision, TileDocument.GetAllocator());

	TileDocument.AddMember("Tile", tileTree, TileDocument.GetAllocator());

	TileDocument.Parse(TileBuffer.GetString());

	SaveTilesToJson(type,path);
}

void JSON::SaveTilesToJson(Tile_Type type, const std::string& path)
{
	std::string filename(file_path);
	filename.append(path);

	if(static_cast<int>(type) == 0)
		filename.append("Physical_Tiles.json");
	else
		filename.append("Graphics_Tiles.json");

	FILE* fp = fopen(filename.c_str(), "wb+");

	char writeBuffer[16384];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	PrettyWriter<FileWriteStream> writer(os);
	TileDocument.Accept(writer);

	fclose(fp);
}

Document JSON::LoadTilesDocumentFromJson(Tile_Type type, const std::string& file)
{
	std::string filename(file_path);
	filename.append(file);

	if (static_cast<int>(type) == 0)
		filename.append("Physical_Tiles.json");
	else
		filename.append("Graphics_Tiles.json");

	FILE* fp = fopen(filename.c_str(), "rb+");

	char readBuffer[65535];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document Tiles;
	Tiles.ParseStream(is);

	fclose(fp);

	return Tiles;
}

void JSON::SaveLevelObject(Object* obj, const std::string& file, const std::string& path)
{
	ObjectsToDocument(obj,file, path);
}

void JSON::SaveLevelTiles(int grid, Object * tiles, Tile_Type type, const std::string & file, const std::string & path)
{
	TilesToDocument(grid, tiles, type, path);
}

void JSON::LoadLevel(const std::string& file, const std::string& path)
{
	LoadObjectFromJson(file, path);
	LoadTilesFromJson(Tile_Type::Graphical, file);
	LoadTilesFromJson(Tile_Type::Physical, file);
}

void JSON::LoadTilesFromJson(Tile_Type type,const std::string& file)
{
	TileDocument = LoadTilesDocumentFromJson(type, file);

	for (auto& temp : TileDocument.GetObject())
	{
		Value& tile_array = temp.value;

		Value tile_type, grid, scale, pos, animation, sprite, collision, isAnimated;
		Object* obj = new Object();

		sprite.SetObject();
		animation.SetObject();
		collision.SetObject();
		isAnimated.SetObject();
		tile_type.SetObject();
		grid.SetObject();
		scale.SetObject();
		pos.SetObject();

		grid = tile_array.FindMember("grid")->value;
		tile_type = tile_array.FindMember("tile_type")->value;
		scale = tile_array.FindMember("scale")->value;
		pos = tile_array.FindMember("pos")->value;
		sprite = tile_array.FindMember("sprite")->value;
		animation = tile_array.FindMember("animation")->value;
		collision = tile_array.FindMember("collision")->value;
		isAnimated = tile_array.FindMember("IsAnimated")->value;

		int grid_ = grid.GetInt();

		//////////////////////////////////////// Transform
		vector2 position, scale_;

		position.x = pos.FindMember("x")->value.GetFloat();
		position.y = pos.FindMember("y")->value.GetFloat();
		scale_.x = scale.FindMember("x")->value.GetFloat();
		scale_.y = scale.FindMember("y")->value.GetFloat();

		obj->SetTranslation(position);
		obj->SetScale(scale_);

		obj->SetMesh(mesh::CreateBox(1, { 255, 255, 255, 255 }));

		////////////////////////////////////////////////// Sprite

		if (sprite.HasMember("is_flip"))
		{
			bool is_flip = false;
			std::string path;

			path = sprite.FindMember("image_path")->value.GetString();
			is_flip = sprite.FindMember("is_flip")->value.GetBool();

			obj->AddInitComponent(new Sprite(path));
			obj->GetComponentByTemplate<Sprite>()->SetFlip(is_flip);
		}

		//////////////////////////////////////////// Animation

		if (animation.HasMember("map"))
		{
			std::vector<std::string> id, ani_path;
			std::vector<int> image_frame;
			std::vector<float> update_frame;
			std::vector<bool> is_repeat;

			for (auto& temp_ : animation.FindMember("map")->value.GetObject())
			{
				Value& map_array = animation.FindMember("map")->value;

				id.push_back(map_array.FindMember("path")->value.FindMember("id")->value.GetString());
				ani_path.push_back(map_array.FindMember("path")->value.FindMember("path")->value.GetString());
				image_frame.push_back(map_array.FindMember("info")->value.FindMember("image_frames")->value.GetInt());
				update_frame.push_back(map_array.FindMember("info")->value.FindMember("update_frames")->value.GetFloat());
				is_repeat.push_back(map_array.FindMember("info")->value.FindMember("is_repeats")->value.GetBool());
			}

			obj->AddInitComponent(new Animation(ani_path.at(0), id.at(0), image_frame.at(0), update_frame.at(0), is_repeat.at(0)));

			for (size_t i = 1; i < id.size(); i++)
			{
				obj->GetComponentByTemplate<Animation>()->AddAnimaition(ani_path.at(i), id.at(i), image_frame.at(i),
					update_frame.at(i), is_repeat.at(i));
			}
		}

		///////////////////////////////////////////////// Collision
		if (collision.HasMember("id"))
		{
			int collision_type = collision.FindMember("id")->value.GetInt();

			obj->AddInitComponent(new Collision(static_cast<CollisionType>(collision_type)));
		}

        if (static_cast<int>(type) == 0)
        {
            Tile_Map_.MakeGridTure(position.x, position.y);
            Tile_Map_.InsertPhysicalTiles(grid_, obj);
            Tile_Map_.SetReset(false);
        }
        else
        {
                    Tile_Map_.MakeGridTure(position.x, position.y);
            Tile_Map_.InsertGraphicalTiles(grid_, obj);
            Tile_Map_.SetReset(false);
        }
	}
        TileDocument.SetObject();
}

void JSON::LoadObjectFromJson(const std::string& file, const std::string& path)
{
	ObjectDocument = LoadObjectDocumentFromJson(file, path);

	for (auto& temp : ObjectDocument.GetObject())
	{
		Value& obj_array = temp.value;

		Value status, transform, animation, sprite, rigid_body, collision, particle, sound, font, capture, camera;
		Value ui, trigger, chapter;
		
		Object* obj = new Object();

		status.SetObject();
		transform.SetObject();
		sprite.SetObject();
		animation.SetObject();
		rigid_body.SetObject();
		collision.SetObject();
		particle.SetObject();
		sound.SetObject();
		font.SetObject();
		capture.SetObject();
		ui.SetObject();
        trigger.SetObject();
		chapter.SetObject();
		
		status = obj_array.FindMember("Type")->value;
		transform = obj_array.FindMember("Transform")->value;
		sprite = obj_array.FindMember("Sprite")->value;
		animation = obj_array.FindMember("Animation")->value;
		rigid_body = obj_array.FindMember("RigidBody")->value;
		collision = obj_array.FindMember("Collision")->value;
		particle = obj_array.FindMember("Particle")->value;
		sound = obj_array.FindMember("Sound")->value;
		font = obj_array.FindMember("Font")->value;
		capture = obj_array.FindMember("Capture")->value;
		camera = obj_array.FindMember("Camera")->value;
        trigger = obj_array.FindMember("Trigger")->value;
		chapter = obj_array.FindMember("Chapter")->value;
		ui = obj_array.FindMember("UI")->value;

		//////////////////////////////////////////// Status
		if (status.HasMember("type"))
		{
			bool isDead = status.FindMember("isDead")->value.GetBool();
			int obj_type = status.FindMember("type")->value.GetInt();

			obj->SetIsDead(isDead);
			obj->SetObjectType(static_cast<ObjectType>(obj_type));
		}

	    //////////////////////////////////////// Transform
		vector2 pos, scale;
		float rotation, depth;

		pos.x = transform.FindMember("position")->value.FindMember("x")->value.GetFloat();
		pos.y = transform.FindMember("position")->value.FindMember("y")->value.GetFloat();
		scale.x = transform.FindMember("scale")->value.FindMember("x")->value.GetFloat();
		scale.y = transform.FindMember("scale")->value.FindMember("y")->value.GetFloat();
		rotation = transform.FindMember("rotation")->value.GetFloat();
		depth = transform.FindMember("depth")->value.GetFloat();

		obj->SetTranslation(pos);
		obj->SetScale(scale);
		obj->SetRotation(rotation);
		obj->SetDepth(depth);

		obj->SetMesh(mesh::CreateBox(1, { 255, 255, 255, 255 }));

		////////////////////////////////////////////////// Sprite
		bool is_flip = false;
		std::string path;

		if (sprite.HasMember("image_path"))
		{
			path = sprite.FindMember("image_path")->value.GetString();
			is_flip = sprite.FindMember("is_flip")->value.GetBool();

			obj->AddComponent(new Sprite(path));
			obj->GetComponentByTemplate<Sprite>()->SetFlip(is_flip);
		}

		//////////////////////////////////////////// Animation
		std::vector<std::string> id, ani_path;
		std::vector<int> image_frame;
		std::vector<float> update_frame;
		std::vector<bool> is_repeat;

		if(animation.HasMember("maps"))
		{
			for (auto& temp_ : animation.FindMember("maps")->value.GetObject())
			{
				Value& map_array = temp_.value;

				id.push_back(map_array.FindMember("path")->value.FindMember("id")->value.GetString());
				ani_path.push_back(map_array.FindMember("path")->value.FindMember("path")->value.GetString());
				image_frame.push_back(map_array.FindMember("info")->value.FindMember("image_frames")->value.GetInt());
				update_frame.push_back(map_array.FindMember("info")->value.FindMember("update_frames")->value.GetFloat());
				is_repeat.push_back(map_array.FindMember("info")->value.FindMember("is_repeats")->value.GetBool());
			}

			obj->AddComponent(new Animation(ani_path.at(0), id.at(0),image_frame.at(0), update_frame.at(0), is_repeat.at(0)));

			for(size_t i = 1; i < id.size(); i++)
			{
				obj->GetComponentByTemplate<Animation>()->AddAnimaition(ani_path.at(i), id.at(i), image_frame.at(i),
					update_frame.at(i), is_repeat.at(i));
			}
		}

		/////////////////////////////////////////////// RigidBody
		bool is_rigid = false;
		is_rigid = rigid_body.GetBool();
		
		if (is_rigid)
			obj->AddComponent(new RigidBody());

		/////////////////////////////////////////////// Collision
		if (collision.HasMember("id"))
		{
			CollisionType type = static_cast<CollisionType>(collision.FindMember("id")->value.GetInt());

			obj->AddComponent(new Collision(type));
		}
		////////////////////////////////////////////// Particle
		vector2 start, random, particle_size, emit_size;

		if (particle.HasMember("emit_rate"))
		{
			int emit_rate = particle.FindMember("emit_rate")->value.GetInt();
			auto life_time = particle.FindMember("life_time")->value.GetFloat();
			auto size_variance = particle.FindMember("size_variance")->value.GetFloat();
			auto color_duration = particle.FindMember("color_duration")->value.GetFloat();

			start.x = particle.FindMember("start_velocity")->value.FindMember("x")->value.GetFloat();
			start.y = particle.FindMember("start_velocity")->value.FindMember("y")->value.GetFloat();
			random.x = particle.FindMember("random_velocity")->value.FindMember("x")->value.GetFloat();
			random.y = particle.FindMember("random_velocity")->value.FindMember("y")->value.GetFloat();
			particle_size.x = particle.FindMember("particle_size")->value.FindMember("x")->value.GetFloat();
			particle_size.y = particle.FindMember("particle_size")->value.FindMember("y")->value.GetFloat();
			emit_size.x = particle.FindMember("emit_size")->value.FindMember("x")->value.GetFloat();
			emit_size.y = particle.FindMember("emit_size")->value.FindMember("y")->value.GetFloat();
			bool isActive = particle.FindMember("isActive")->value.GetBool();

			std::string particle_path = particle.FindMember("path")->value.GetString();
				
			obj->AddComponent(new Particle_Generator(emit_rate, life_time, size_variance,
					color_duration, start, random, particle_size, emit_size,  particle_path, isActive));
		}
        ////////////////////////////////////////////////////Trigger
        if (trigger.HasMember("translation"))
        {
            start.x = trigger.FindMember("translation")->value.FindMember("x")->value.GetFloat();
            start.y = trigger.FindMember("translation")->value.FindMember("y")->value.GetFloat();

			scale.x = trigger.FindMember("scale")->value.FindMember("x")->value.GetFloat();
			scale.y = trigger.FindMember("scale")->value.FindMember("y")->value.GetFloat();

			if(scale == vector2(0,0))
			{
				scale = vector2(200, 75);
			}
			
            auto t_style = static_cast<TriggerStyle>(trigger.FindMember("type")->value.GetInt());
            auto isTrigger = trigger.FindMember("trigger")->value.GetBool();
            auto i_frame = trigger.FindMember("i_frame")->value.GetFloat();
            auto u_frame = trigger.FindMember("u_frame")->value.GetFloat();
            path = trigger.FindMember("text")->value.GetString();

            obj->AddComponent(new Trigger(start,scale, t_style, path, false, i_frame,u_frame));
        }
		////////////////////////////////////////////////////Sound
		if(sound.HasMember("map"))
		{
			std::vector<std::string> sound_path;
			for (auto& temp_ : sound.FindMember("map")->value.GetObject())
			{
				Value& map_array = sound.FindMember("map")->value;
				sound_path.push_back(map_array.FindMember("path")->value.GetString());
			}

			obj->AddComponent(new Sound(sound_path[0]));

			for (size_t i = 1; i < sound_path.size(); i++)
			{
				obj->GetComponentByTemplate<Sound>()->AddSound(sound_path[i]);
			}
		}

		//////////////////////////////////////////Font
		if (font.HasMember("text"))
		{
			std::string font_text = font.FindMember("text")->value.GetString();
			std::string font_path;
			if (font.HasMember("path"))
			{
				font_path = font.FindMember("path")->value.GetString();
			}
			//obj->AddComponent(new Font(font_text, font_path));
		}

		//////////////////////////////////////////Capture
		if(capture.HasMember("pos"))
		{
			vector2 reset_pos;// = StateManager_.GetCurrentState()->GetPlayerObjectPointer()->GetTransform().GetTranslation();
			reset_pos.x = capture.FindMember("pos")->value.FindMember("x")->value.GetFloat();
			reset_pos.y = capture.FindMember("pos")->value.FindMember("y")->value.GetFloat();

			obj->AddComponent(new Capture(reset_pos));
		}

		////////////////////////////////////////////UI
		if(ui.HasMember("id"))
		{
			obj->AddComponent(new UI(ui.FindMember("id")->value.GetString()));
		}

		////////////////////////////////////////////Moving

	        if (obj->GetObjectType() == ObjectType::Start_Pos)
                {
                    StateManager_.GetCurrentState()->SetStartPosition(obj->GetTransform().GetTranslation());
                }
	        if (obj->GetObjectType() == ObjectType::JumpIng)
                {
                    obj->SetObjectType(ObjectType::Capture_Obj);
                    obj->GetComponentByTemplate<Collision>()->SetFilter(Filter::Jump);
                }
                        Objectmanager_.AddObject(obj);
                if (obj->GetObjectType() == ObjectType::Player)
                    Objectmanager_.SetPlayer(obj);


		////////////////////Chapter
		if (chapter.HasMember("chapter"))
		{
			int chap = chapter.FindMember("chapter")->value.GetInt();
			int lev = chapter.FindMember("level")->value.GetInt();

			StateManager_.GetCurrentState()->SetChapter(std::make_pair(chap,lev));
		}
	}
	ObjectDocument.SetObject();
}
