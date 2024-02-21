#pragma once

#include "cseries/cseries.hpp"
#include "effects/effects.hpp"
#include "memory/data.hpp"

struct c_dynamic_cubemap_sample
{
	struct
	{
		s_cluster_reference cluster_reference;
		short cluster_cubemap_index;
		short cubemap_bitmap_index;
	} __unknown0[2];

	real m_cubemap_sample;
};
static_assert(sizeof(c_dynamic_cubemap_sample) == 0x10);

struct render_lighting
{
	vector3d up;
	s_geometry_sample lightprobe_sample;
	c_dynamic_cubemap_sample cubemap_sample;
	long __unknown214;
};
static_assert(sizeof(render_lighting) == 0x218);

struct s_shader_extern_info
{
	long context;
	real(__cdecl* context_interface)(long, long, long);
	dword cpu_memory_pool_designator;
	dword __unknownC;
	render_lighting lighting;
	dword change_colors[5];
	byte change_color_count;
	byte __data23D[0x17];
	long __unknown254;
	long __unknown258;
};
static_assert(sizeof(s_shader_extern_info) == 0x25C);

struct cached_object_render_state_datum :
	s_datum_header
{
	byte __unknown2;
	byte __unknown3;
	bool __unknown4;
	long object_index;
	dword frame_indexC;
	dword frame_index10;
	real_point3d last_sample_position;
	render_lighting last_lighting;
	s_shader_extern_info last_render_info;
	char last_level_of_detail;
	char last_permutations[4][16];
};
static_assert(sizeof(cached_object_render_state_datum) == 0x4D8);

struct render_first_person_model
{
	long render_model_definition_index;
	long object_index;
	dword_flags flags;
	c_static_array<real_matrix4x3, 150> render_model_node_matrices;
};
static_assert(sizeof(render_first_person_model) == 0x1E84);

struct s_render_object_first_person_globals
{
	long camera_object_index;
	long model_count;
	c_static_array<render_first_person_model, 6> models;
};
static_assert(sizeof(s_render_object_first_person_globals) == 0xB720);

extern s_render_object_first_person_globals& render_object_globals;

extern render_lighting const* object_get_cached_render_lighting(long object_index);

