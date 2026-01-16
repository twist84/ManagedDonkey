#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"

struct s_water_physics_effect_parameters
{
	s_water_physics_effect_parameters() :
		water_velocity(),
		water_interaction_velocity()
	{
	}

	real_vector3d water_velocity;
	real_vector3d water_interaction_velocity;
};

struct s_water_physics_test_vector_result
{
	s_water_physics_test_vector_result() : 
		position(),
		normal(),
		__data18(),
		t(),
		structure_bsp_index(),
		water_instance_index(),
		material_type(),
		effect_parameters()
	{
	}

	real_point3d position;
	real_vector3d normal;
	byte __data18[0x4];
	real32 t;
	int32 structure_bsp_index;
	int32 water_instance_index;
	c_global_material_type material_type;
	s_water_physics_effect_parameters effect_parameters;
};
COMPILE_ASSERT(sizeof(s_water_physics_test_vector_result) == 0x44);

extern bool water_physics_debug;
extern real32& k_full_scale_effect_velocity;
extern real32& k_minimum_water_effect_period;
extern real32& k_maximum_water_scale_velocity;
extern real32& k_maximum_surface_distance;
extern real32& k_initial_density_decay_time;
extern real32& k_density_decay_time;
extern real32& water_physics_velocity_minimum;
extern real32& water_physics_velocity_maximum;

extern bool __cdecl water_physics_test_vector(const real_point3d* point, const real_vector3d* vector, s_water_physics_test_vector_result* result);
extern void __cdecl water_physics_render_debug(const real_point3d* point, const real_vector3d* vector);

