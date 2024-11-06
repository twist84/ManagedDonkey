#pragma once

#include "cseries/cseries.hpp"

struct s_object_render_context
{
	byte __data0[0x10];
};
static_assert(sizeof(s_object_render_context) == 0x10);

struct s_context_mesh_part
{
	dword_flags flags;
	void* info;
	byte __data8[0x4];
	word mesh_index;
	word part_index;
	word region_index;
	byte __data[0xA];
};
static_assert(sizeof(s_context_mesh_part) == 0x1C);

struct s_render_object_globals
{
	c_static_sized_dynamic_array<s_object_render_context, 1024> object_render_contexts;
	c_static_sized_dynamic_array<s_context_mesh_part, 8192> context_mesh_part;

	byte __data3C008[0x4];
	long marker_index;

	long object_render_context_markers[6];
	long context_mesh_part_markers[6];
};
static_assert(sizeof(s_render_object_globals) == 0x3C040);

struct c_object_renderer
{
	static void __cdecl render_albedo(dword flags);
	static void __cdecl submit_and_render_sky(long a1, long player_index);
};

