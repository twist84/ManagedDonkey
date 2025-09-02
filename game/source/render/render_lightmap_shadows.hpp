#pragma once

#include "math/real_math.hpp"

struct old_light_geometry_spherical_parameters
{
	real32 diffuse_radius;
	real32 specular_radius;
};
static_assert(sizeof(old_light_geometry_spherical_parameters) == 0x8);

struct old_light_geometry_frustum_parameters
{
	real32 near_width;
	real32 height_scale;
	real32 field_of_view;
	real32 falloff_distance;
	real32 cutoff_distance;
};
static_assert(sizeof(old_light_geometry_frustum_parameters) == 0x14);

struct light_intermediate_frustum_geometry
{
	bool valid;
	real_vector3d skew;
	real32 length;
	real_vector2d projection;
	real32 inverse_aspect;
	real_vector2d size;
	real32 falloff_distance;
	real32 cutoff_distance;
	real_vector2d cutoff_size;
	real_point3d cutoff_position;
};
static_assert(sizeof(light_intermediate_frustum_geometry) == 0x44);

struct light_intermediate_spherical_geometry
{
	bool valid;
	real32 diffuse_radius;
	real32 specular_radius;
	real32 maximum_radius;
};
static_assert(sizeof(light_intermediate_spherical_geometry) == 0x10);

struct light_intermediate_geometry
{
	bool valid;
	real_vector3d basis_right;
	real32 origin_offset;
	real_point3d origin;
	union
	{
		light_intermediate_frustum_geometry frustum;
		light_intermediate_spherical_geometry spherical;
	};
};
static_assert(sizeof(light_intermediate_geometry) == 0x64);

struct light_geometry
{
	int32 geometry_type;
	union
	{
		old_light_geometry_spherical_parameters spherical;
		old_light_geometry_frustum_parameters frustum;
	};
	light_intermediate_geometry intermediate;
};
static_assert(sizeof(light_geometry) == 0x7C);

