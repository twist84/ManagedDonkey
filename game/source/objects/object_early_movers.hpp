#pragma once

#include "cseries/cseries.hpp"

#define MAXIMUM_OBJECT_EARLY_MOVERS_PER_MAP 32

struct s_object_early_mover
{
	// object_early_mover_update
	real_vector3d update_linear_velocity;
	real_vector3d update_angular_velocity;
	real_point3d update_center_of_mass;

	// object_early_mover_move
	real_point3d center_of_mass_copy;
	real_point3d center_of_mass;
	real_vector3d linear_velocity_copy;
	real_vector3d linear_velocity;
	real_vector3d angular_velocity_copy;
	real_vector3d angular_velocity;
	real_matrix4x3 transform_copy;
	real_matrix4x3 transform;
	real_matrix4x3 inverse_transform;
	bool __unknown107;
	bool __unknown108;

	// object_early_mover_update
	bool __unknown109;

	// object_early_mover_update
	long collision_model_bsp_info_count;
	union packed_index_t
	{
		struct
		{
			uint8 node_index;
			uint8 bsp_index;
			uint8 permutation_index;
			uint8 region_index;
		};

		uint32 value;
	};
	c_static_array<packed_index_t, 8> collision_model_bsp_info;
};
static_assert(sizeof(s_object_early_mover) == 0x130);

struct s_object_early_movers_globals
{
	s_object_early_mover meta[MAXIMUM_OBJECT_EARLY_MOVERS_PER_MAP];
	long object_indices[MAXIMUM_OBJECT_EARLY_MOVERS_PER_MAP];
	long object_index_count;
	bool map_initialized;
};
static_assert(sizeof(s_object_early_movers_globals) == 0x2688);

struct s_object_early_mover_obb
{
	real_rectangle3d aabb;
	real_matrix4x3 transform;
};
static_assert(sizeof(s_object_early_mover_obb) == 0x4C);

extern bool __cdecl object_early_mover_get_obb(long object_index_array_index, s_object_early_mover_obb* obb);
extern void __cdecl object_early_mover_notify_local_objects(long early_mover_object_index);
extern void __cdecl object_early_mover_update(long object_index, long early_mover_array_index);
extern void __cdecl object_early_movers_dispose();
extern void __cdecl object_early_movers_dispose_from_old_map();
extern void __cdecl object_early_movers_initialize();
extern void __cdecl object_early_movers_initialize_for_new_map();
extern bool __cdecl object_get_early_mover_local_space_velocity(long object_index, real_vector3d* linear_local_space_velocity, real_vector3d* angular_local_space_velocity, bool a4, bool a5);
extern void __cdecl object_get_early_movers(long const** object_early_movers, long* object_early_movers_count);
extern void __cdecl object_early_mover_render_debug();

