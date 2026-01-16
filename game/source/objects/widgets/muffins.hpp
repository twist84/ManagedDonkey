#pragma once

#include "cseries/cseries.hpp"

struct s_muffin_datum :
	s_datum_header
{
	struct s_muffin
	{
		uns8 flags;
		uns8 type_index;
		uns8 attached_node_index;
		uns8 mesh_index;
		uns16 event_delay;
		uns8 scale;
		uns8 unused;
		int32 marker_name;
		int32 birth_time;
		real_point3d jiggle_position;
		real_vector3d jiggle_velocity;
		real_matrix4x3 fixed_node_to_muffin_matrix;
		real_matrix4x3 fixed_world_to_muffin_matrix;
		real_matrix4x3 jiggle_world_to_muffin_matrix;
	};
	COMPILE_ASSERT(sizeof(s_muffin) == 0xC4);

	uns8 unused;
	uns8 muffin_count;
	s_muffin muffins[32];
	int32 definition_index;
	int32 object_index;
	real32 time_accumulator;
};
COMPILE_ASSERT(sizeof(s_muffin_datum) == 0x1890);

