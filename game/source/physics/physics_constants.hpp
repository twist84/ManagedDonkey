#pragma once

#include "cseries/cseries.hpp"

const real32 k_physics_character_ground_adhesion_force_minimum_disable_time = 0.0f;
const real32 k_physics_character_ground_adhesion_force_maximum_disable_time = 5.0f;

struct s_physics_constants
{
	real32 gravity;
	real32 water_density;
	real32 air_density;
	real_vector3d velocity_frame;
	int32 character_ground_adhesion_forces_enable_time;
	real32 slip_surface_maximum_k;
};
COMPILE_ASSERT(sizeof(s_physics_constants) == 0x20);

extern bool __cdecl character_ground_adhesion_forces_disabled();
extern void __cdecl disable_character_ground_adhesion_forces(real32 disable_time);
extern s_physics_constants* __cdecl global_physics_constants_get();
extern real_vector3d* __cdecl global_physics_velocity_frame_get();
extern void __cdecl global_physics_velocity_frame_set(real32 i, real32 j, real32 k);
extern real32 __cdecl global_slip_surface_maximum_k_get();
extern real32 __cdecl global_standard_gravity_get();
extern void __cdecl physics_constants_dispose();
extern void __cdecl physics_constants_dispose_from_old_map();
extern void __cdecl physics_constants_initialize();
extern void __cdecl physics_constants_initialize_for_new_map();
extern void __cdecl physics_constants_reset();
extern void __cdecl physics_constants_set_gravity_absolute(real32 gravity);
extern void __cdecl physics_constants_set_gravity_halo_relative(real32 gravity);
extern real32 __cdecl global_gravity_get();
extern real32 __cdecl global_water_density_get();
extern real32 __cdecl global_air_density_get();

