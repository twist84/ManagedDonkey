#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct _object_definition
{
	short runtime_object_type;
	word_flags flags;
	real bounding_radius;
	real_point3d bounding_offset;
	real acceleration_scale;
	short_enum lightmap_shadow_mode;
	char_enum sweetener_size;
	char_enum water_density;
	dword_flags runtime_flags;
	real dynamic_light_sphere_radius;
	real_point3d dynamic_light_sphere_offset;
	c_string_id default_model_variant;
	s_tag_reference model;
	s_tag_reference crate_object;
	s_tag_reference collision_damage;
	s_tag_block early_mover_obb;
	s_tag_reference creation_effect;
	s_tag_reference material_effects;
	s_tag_reference armor_sounds;
	s_tag_reference melee_sound;
	s_tag_block ai_properties;
	s_tag_block functions;
	short hud_text_message_index;
	word_flags secondary_flags;
	s_tag_block attachments;
	s_tag_block widgets;
	s_tag_block change_colors;
	s_tag_block node_maps;
	s_tag_block multiplayer_object;
	s_tag_block health_packs;
	s_tag_block pathfinding_spheres;
};
static_assert(sizeof(_object_definition) == 0x120);

