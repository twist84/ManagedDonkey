#pragma once

#include "cseries/cseries.hpp"

struct s_muffin_datum :
	s_datum_header
{
	struct s_muffin
	{
		byte_flags flags;
		byte type_index;
		byte attached_node_index;
		byte mesh_index;
		word event_delay;
		byte scale;
		byte unused;
		long marker_name;
		long birth_time;
		real_point3d jiggle_position;
		real_vector3d jiggle_velocity;
		real_matrix4x3 fixed_node_to_muffin_matrix;
		real_matrix4x3 fixed_world_to_muffin_matrix;
		real_matrix4x3 jiggle_world_to_muffin_matrix;
	};
	static_assert(sizeof(s_muffin) == 0xC4);

	byte unused;
	byte muffin_count;
	s_muffin muffins[32];
	long definition_index;
	long object_index;
	real time_accumulator;
};
static_assert(sizeof(s_muffin_datum) == 0x1890);

