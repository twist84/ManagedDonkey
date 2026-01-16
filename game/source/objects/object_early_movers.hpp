#pragma once

#include "cseries/cseries.hpp"

#define MAXIMUM_OBJECT_EARLY_MOVERS_PER_MAP 32

struct s_object_early_movers_globals
{
	struct s_meta
	{
		real_vector3d current_linear_velocity;
		real_vector3d current_angular_velocity;
		real_point3d current_center_of_mass;
		real_point3d last_frame_center_of_mass;
		real_point3d frame_center_of_mass;
		real_vector3d last_frame_linear_velocity;
		real_vector3d frame_linear_velocity;
		real_vector3d last_frame_angular_velocity;
		real_vector3d frame_angular_velocity;
		real_matrix4x3 last_frame_transform;
		real_matrix4x3 frame_transform;
		real_matrix4x3 step_transform;
		bool valid_last_frame_transform;
		bool valid_frame_transform;
		bool valid_current_velocities;
		int32 bsp_reference_count;
		c_static_array<uns32, 8> bsp_references;
	};
	COMPILE_ASSERT(sizeof(s_meta) == 0x130);

	s_meta meta[MAXIMUM_OBJECT_EARLY_MOVERS_PER_MAP];
	int32 object_indexes[MAXIMUM_OBJECT_EARLY_MOVERS_PER_MAP];
	int32 object_index_count;
	bool map_initialized;
};
COMPILE_ASSERT(sizeof(s_object_early_movers_globals) == 0x2688);

struct s_object_early_mover_obb
{
	real_rectangle3d aabb;
	real_matrix4x3 transform;
};
COMPILE_ASSERT(sizeof(s_object_early_mover_obb) == 0x4C);

extern bool __cdecl object_early_mover_get_obb(int32 object_index_array_index, s_object_early_mover_obb* obb);
extern void __cdecl object_early_mover_notify_local_objects(int32 early_mover_object_index);
extern void __cdecl object_early_mover_update(int32 object_index, int32 early_mover_array_index);
extern void __cdecl object_early_movers_dispose();
extern void __cdecl object_early_movers_dispose_from_old_map();
extern void __cdecl object_early_movers_initialize();
extern void __cdecl object_early_movers_initialize_for_new_map();
extern bool __cdecl object_get_early_mover_local_space_velocity(int32 moving_object_index, real_vector3d* linear_local_space_velocity, real_vector3d* angular_local_space_velocity, bool localized_phyiscs_early_movers_only, bool current);
extern void __cdecl object_get_early_movers(const int32** object_early_movers, int32* object_early_movers_count);
extern void __cdecl object_in_early_mover_join(int32 object_index, int32 joining_object_index);
extern void __cdecl object_early_mover_render_debug();

