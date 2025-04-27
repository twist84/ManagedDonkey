#pragma once

#include "cseries/cseries.hpp"

struct cloth_vertex
{
	real_point3d position;
	real_point3d last_position;
	c_dword_unit_vector normal;
};
static_assert(sizeof(cloth_vertex) == 0x1C);

struct cloth_vertex_attachment
{
	short vertex_index;
	short marker_index;
};
static_assert(sizeof(cloth_vertex_attachment) == 0x4);

struct cloth_datum :
	s_datum_header
{
	uint16 pad;
	long definition_index;
	long object_index;
	real32 time_accumulator;
	real_point3d last_attached_object_position;
	real32 object_velocity;
	cloth_vertex dynamic_vertices[121];
	uint32 attached_bit_vector[4];
	cloth_vertex_attachment attachments[6];
	short attachment_count;
	c_static_array<short, 1210> attached_links;
	bool attachment_links_initialized;
	short attached_link_count;
	long last_update_tick;
};
static_assert(sizeof(cloth_datum) == 0x1704);

