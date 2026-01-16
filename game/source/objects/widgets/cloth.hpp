#pragma once

#include "cseries/cseries.hpp"

struct cloth_vertex
{
	real_point3d position;
	real_point3d last_position;
	c_dword_unit_vector normal;
};
COMPILE_ASSERT(sizeof(cloth_vertex) == 0x1C);

struct cloth_vertex_attachment
{
	int16 vertex_index;
	int16 marker_index;
};
COMPILE_ASSERT(sizeof(cloth_vertex_attachment) == 0x4);

struct cloth_datum :
	s_datum_header
{
	uns16 pad;
	int32 definition_index;
	int32 object_index;
	real32 time_accumulator;
	real_point3d last_attached_object_position;
	real32 object_velocity;
	cloth_vertex dynamic_vertices[121];
	uns32 attached_bit_vector[4];
	cloth_vertex_attachment attachments[6];
	int16 attachment_count;
	c_static_array<int16, 1210> attached_links;
	bool attachment_links_initialized;
	int16 attached_link_count;
	int32 last_update_tick;
};
COMPILE_ASSERT(sizeof(cloth_datum) == 0x1704);

