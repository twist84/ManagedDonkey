#pragma once

#include "tag_files/tag_groups.hpp"

enum e_scene_object_flags
{
	_scene_object_flags_not_created_dynamically = 0,
	_scene_object_flags_comes_from_game,
	_scene_object_flags_name_is_function_call,
	_scene_object_flags_effect_object,
	_scene_object_flags_no_lightmap_shadow,

	k_scene_object_flags_count,
};

struct s_cinematic_custom_script
{
	s_tag_data script;
};
static_assert(sizeof(s_cinematic_custom_script) == sizeof(s_tag_data));

struct s_cinematic_scene
{
	int32 name;
	char anchor[32];
	int16 lighting_reset;
	int16 pad;
	s_cinematic_custom_script header;
	s_tag_block objects;
	s_tag_block shots;
	s_tag_block extra_cameras;
	s_cinematic_custom_script footer;
	int32 version;
};
static_assert(sizeof(s_cinematic_scene) == 0x78);

struct s_scene_object
{
	char name[32];
	int32 identifier;
	int32 variant_name;
	s_tag_reference animation_graph;
	s_tag_reference tag_type;
	int32 flags;
	int32 shot_active_flags;
	int32 override_creation_flags;
	s_cinematic_custom_script custom_override_creation_condition;
	s_tag_block attachments;
};
static_assert(sizeof(s_scene_object) == 0x74);

struct s_cinematic_definition; // $TODO

