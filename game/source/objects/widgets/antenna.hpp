#pragma once

#include "cseries/cseries.hpp"

struct antenna_vertex_datum
{
	real_point3d position;
	real_vector3d velocity;
	short sprite_index;
};
static_assert(sizeof(antenna_vertex_datum) == 0x1C);

struct antenna_datum :
	s_datum_header
{
	word pad;
	bool invalid;
	bool first_update;
	long definition_index;
	long object_index;
	real_point3d last_attachment_location;
	antenna_vertex_datum vertices[2];
	long last_update_index;
	real_vector3d attachment_vector;
};
static_assert(sizeof(antenna_datum) == 0x64);

