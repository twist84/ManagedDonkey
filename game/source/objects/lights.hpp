#pragma once

#include "cseries/cseries.hpp"
#include "cseries/location.hpp"

struct light_state
{
	real_point3d position;
	real_point3d target_position;
	real32 offset_origin_distance;
	real_point3d offset_origin;
	real_vector3d forward;
	real_vector3d up;
	real32 size_modifier;
	real32 fade;
	real32 maximum_scale;
	real32 current_scale;
	real_rgb_color color;
	real_vector2d gel_translation;
};
static_assert(sizeof(light_state) == 0x64);

struct light_datum :
	s_datum_header
{
	uns16 flags;
	int32 definition_index;
	int32 scenario_light_index;
	int32 first_cluster_reference_index;
	int32 creation_time;
	real_point3d bounding_center;
	real32 bounding_radius;
	real_point3d lod_sphere_center;
	real32 lod_sphere_radius;
	real_point3d origin;
	s_location location;
	int32 attached_object_index;
	int32 object_function_reference;
	int16 object_attachment_index;
	int16 object_change_color_reference;
	int32 unattached_object_index;
	int16 object_node_index;
	real_point3d node_relative_position;
	real_vector3d node_relative_forward;
	real_vector3d node_relative_up;
	light_state state;
	int32 precomputed_structure_visibility_block_index;
};
static_assert(sizeof(light_datum) == 0xE4);

struct lights_game_globals_definition
{
	bool can_use_flashlights;
	int32 can_use_flashlights_time;
	bool render_lights;
	bool pad[0x3];
	c_static_flags_no_init<400> light_active_bitvector;
};
static_assert(sizeof(lights_game_globals_definition) == 0x40);

struct s_nondeterministic_light_data
{
	byte __data[0x2580];
};
static_assert(sizeof(s_nondeterministic_light_data) == 0x2580);

extern bool __cdecl lights_enable(bool enable);
extern void __cdecl lights_prepare_for_window(int32 player_window_index);
extern void __cdecl lights_update();

