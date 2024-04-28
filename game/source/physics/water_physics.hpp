#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"

struct s_water_physics_test_vector_result
{
	s_water_physics_test_vector_result() : 
		__point0(),
		__vectorC(),
		__data18(),
		__unknown1C(),
		structure_bsp_index(),
		water_instance_index(),
		material_type(),
		direction(),
		__vector38()
	{
	}

	real_point3d __point0;
	vector3d __vectorC;

	byte __data18[0x4];

	real __unknown1C;

	long structure_bsp_index;
	long water_instance_index;
	c_global_material_type material_type;
	vector3d direction;

	vector3d __vector38;
};
static_assert(sizeof(s_water_physics_test_vector_result) == 0x44);

extern bool water_physics_debug;
extern real& k_full_scale_effect_velocity;
extern real& k_minimum_water_effect_period;
extern real& k_maximum_water_scale_velocity;
extern real& k_maximum_surface_distance;
extern real& k_initial_density_decay_time;
extern real& k_density_decay_time;

extern bool __cdecl water_physics_test_vector(real_point3d const* point, vector3d const* vector, s_water_physics_test_vector_result* result);
extern void __cdecl water_physics_render_debug(real_point3d const* point, vector3d const* vector);

