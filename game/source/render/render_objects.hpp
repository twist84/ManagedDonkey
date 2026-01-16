#pragma once

#include "cseries/cseries.hpp"

enum e_render_object_mesh_part_flags
{
	_render_object_mesh_part_lit_bit = 0,
	_render_object_mesh_part_shadow_casting_bit,
	_render_object_mesh_part_transparent_bit,
	_render_object_mesh_part_instance_bit,
	_render_object_mesh_part_generates_heat_bit,
	_render_object_mesh_part_decal_bit,
	_render_object_mesh_part_is_sky_bit,
	_render_object_mesh_part_tron_bit,
	_render_object_mesh_part_first_person_squished_bit,
	_render_object_mesh_part_first_person_unsquished_bit,
	_render_object_mesh_part_cancel_shadows_for_first_person_albedo,

	k_number_of_render_object_mesh_part_flags
};

struct s_object_render_context
{
	byte __data0[0x10];
};
COMPILE_ASSERT(sizeof(s_object_render_context) == 0x10);

struct s_context_mesh_part
{
	uns32 flags;
	void* info;
	byte __data8[0x4];
	uns16 mesh_index;
	uns16 part_index;
	uns16 region_index;
	byte __data[0xA];
};
COMPILE_ASSERT(sizeof(s_context_mesh_part) == 0x1C);

struct s_render_object_globals
{
	c_static_sized_dynamic_array<s_object_render_context, 1024> object_render_contexts;
	c_static_sized_dynamic_array<s_context_mesh_part, 8192> context_mesh_part;

	byte __data3C008[0x4];
	int32 marker_index;

	int32 object_render_context_markers[6];
	int32 context_mesh_part_markers[6];
};
COMPILE_ASSERT(sizeof(s_render_object_globals) == 0x3C040);

class c_object_renderer
{
public:
	static void __cdecl pop_marker();
	static void __cdecl push_marker();
	static void __cdecl render_albedo(uns32 flags);
	static void __cdecl render_albedo_decals();
	static void __cdecl render_object_contexts(e_entry_point entry_point, int32 mesh_part_mask);
	static void __cdecl render_object_widgets(e_entry_point entry_point);
	static void __cdecl submit_and_render_sky(int32 entry_point_type, int32 player_window_index);
	static void __cdecl submit_visibility(uns32 submit_visibility_flags);
};

