#pragma once

#include "cseries/cseries.hpp"

const real k_physics_character_ground_adhesion_force_minimum_disable_time = 0.0f;
const real k_physics_character_ground_adhesion_force_maximum_disable_time = 5.0f;

struct s_physics_constants
{
	real gravity;
	real water_density;
	real air_density;
	vector3d velocity_frame;
	long character_ground_adhesion_forces_enable_time;
	real slip_surface_maximum_k;
};
static_assert(sizeof(s_physics_constants) == 0x20);

extern bool __cdecl character_ground_adhesion_forces_disabled();
extern void __cdecl disable_character_ground_adhesion_forces(real disable_time);
extern s_physics_constants* __cdecl global_physics_constants_get();
extern vector3d* __cdecl global_physics_velocity_frame_get();
extern void __cdecl global_physics_velocity_frame_set(real i, real j, real k);
extern real __cdecl global_slip_surface_maximum_k_get();
extern real __cdecl global_standard_gravity_get();
extern void __cdecl physics_constants_dispose();
extern void __cdecl physics_constants_dispose_from_old_map();
extern void __cdecl physics_constants_initialize();
extern void __cdecl physics_constants_initialize_for_new_map();
extern void __cdecl physics_constants_reset();
extern void __cdecl physics_constants_set_gravity_absolute(real gravity);
extern void __cdecl physics_constants_set_gravity_halo_relative(real gravity);
extern real __cdecl global_gravity_get();
extern real __cdecl global_water_density_get();
extern real __cdecl global_air_density_get();

