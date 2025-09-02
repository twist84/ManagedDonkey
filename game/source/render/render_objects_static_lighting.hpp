#pragma once

#include "cseries/cseries.hpp"
#include "effects/effects.hpp"
#include "rasterizer/rasterizer_text.hpp"

struct s_cubemap_sample
{
	s_cluster_reference cluster_reference;
	int16 cluster_cubemap_index;
	int16 cubemap_bitmap_index;
};
static_assert(sizeof(s_cubemap_sample) == 0x6);

class c_dynamic_cubemap_sample
{
public:
	s_cubemap_sample m_current;
	s_cubemap_sample m_last;
	real32 m_blend_factor;
};
static_assert(sizeof(c_dynamic_cubemap_sample) == 0x10);

struct render_lighting
{
	real_vector3d shadow_direction;
	s_geometry_sample lightprobe_sample;
	c_dynamic_cubemap_sample cubemap_state;
	int32 cinematic_sh_light_index;
};
static_assert(sizeof(render_lighting) == 0x218);

struct s_shader_extern_info
{
	int32 context;
	real32(__cdecl* context_interface)(int32, int32, int32);
	uns32 cpu_memory_pool_designator;
	uns32 render_frame_allocated;
	render_lighting lighting;
	uns32 change_colors[5];
	uns8 change_color_count;
	uns8 tron_amount;
	uns8 active_camo_amount;
	uns8 overshield_amount;
	uns8 current_body_damage;
	uns8 current_shield_damage;
	real_vector4d bounding_sphere;
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
	int32 object_index;
	int32 refresh_frame_index;
	int32 render_frame_index;
	real_point3d refresh_sample_position;
	render_lighting desired_lighting;
	s_shader_extern_info render_info;
	int8 last_level_of_detail_rendered;
	int8 last_rendered_permutation_indices[4][16];
};
static_assert(sizeof(object_render_state) == 0x4D8);

struct render_first_person_model
{
	int32 render_model_index;
	int32 object_index;
	uns32 flags;
	c_static_array<real_matrix4x3, 150> node_matrices;
};
static_assert(sizeof(render_first_person_model) == 0x1E84);

struct s_render_object_first_person_globals
{
	int32 first_person_camera_object_index;
	int32 first_person_model_count;
	c_static_array<render_first_person_model, 6> first_person_models;
};
static_assert(sizeof(s_render_object_first_person_globals) == 0xB720);

extern bool render_debug_show_air_probes;
extern s_render_object_first_person_globals& render_object_globals;

extern const render_lighting* object_get_cached_render_lighting(int32 object_index);
extern bool __cdecl render_object_should_be_visible(int32 object_index);
extern void __cdecl render_objects_initialize();
extern void __cdecl render_objects_initialize_for_new_map();
extern void __cdecl render_objects_prepare_for_window(int32 user_index);

