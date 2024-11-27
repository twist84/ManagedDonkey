#pragma once

#include "cseries/cseries.hpp"
#include "effects/effects.hpp"
#include "memory/data.hpp"
#include "rasterizer/rasterizer_text.hpp"

struct s_cubemap_sample
{
	s_cluster_reference cluster_reference;
	short cluster_cubemap_index;
	short cubemap_bitmap_index;
};
static_assert(sizeof(s_cubemap_sample) == 0x6);

struct c_dynamic_cubemap_sample
{
	s_cubemap_sample m_current;
	s_cubemap_sample m_last;
	real m_blend_factor;
};
static_assert(sizeof(c_dynamic_cubemap_sample) == 0x10);

struct render_lighting
{
	vector3d shadow_direction;
	s_geometry_sample lightprobe_sample;
	c_dynamic_cubemap_sample cubemap_state;
	long cinematic_sh_light_index;
};
static_assert(sizeof(render_lighting) == 0x218);

struct s_shader_extern_info
{
	long context;
	real(__cdecl* context_interface)(long, long, long);
	dword cpu_memory_pool_designator;
	dword render_frame_allocated;
	render_lighting lighting;
	dword change_colors[5];
	byte change_color_count;
	byte tron_amount;
	byte active_camo_amount;
	byte overshield_amount;
	byte current_shield_damage;
	vector4d bounding_sphere;
	c_rasterizer_texture_ref emblem_player_shoulder_texture;
	c_rasterizer_texture_ref emblem_clan_chest_texture;
};
static_assert(sizeof(s_shader_extern_info) == 0x25C);

struct object_render_state :
	s_datum_header
{
	bool valid_sample_has_been_made;
	bool valid_lighting;
	bool reach_desired_lighting;
	long object_index;
	long refresh_frame_index;
	long render_frame_index;
	real_point3d refresh_sample_position;
	render_lighting desired_lighting;
	s_shader_extern_info render_info;
	char last_level_of_detail_rendered;
	char last_rendered_permutation_indices[4][16];
};
static_assert(sizeof(object_render_state) == 0x4D8);

struct render_first_person_model
{
	long render_model_index;
	long object_index;
	dword_flags flags;
	c_static_array<real_matrix4x3, 150> node_matrices;
};
static_assert(sizeof(render_first_person_model) == 0x1E84);

struct s_render_object_first_person_globals
{
	long first_person_camera_object_index;
	long first_person_model_count;
	c_static_array<render_first_person_model, 6> first_person_models;
};
static_assert(sizeof(s_render_object_first_person_globals) == 0xB720);

extern s_render_object_first_person_globals& render_object_globals;

extern render_lighting const* object_get_cached_render_lighting(long object_index);
extern bool __cdecl render_object_should_be_visible(long object_index);
extern void __cdecl render_objects_initialize();
extern void __cdecl render_objects_initialize_for_new_map();
extern void __cdecl render_objects_prepare_for_window(long user_index);

