#pragma once

#include "cseries/cseries.hpp"

struct antenna_vertex_datum
{
	real_point3d position;
	real_vector3d velocity;
	int16 sprite_index;
};
COMPILE_ASSERT(sizeof(antenna_vertex_datum) == 0x1C);

struct antenna_datum :
	s_datum_header
{
	uns16 pad;
	bool invalid;
	bool first_update;
	int32 definition_index;
	int32 object_index;
	real_point3d last_attachment_location;
	antenna_vertex_datum vertices[2];
	int32 last_update_index;
	real_vector3d attachment_vector;
};
COMPILE_ASSERT(sizeof(antenna_datum) == 0x64);

