#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_soft_surfaces_definition;

struct s_structure_physics
{
	s_tag_block mopp_code_block;
	byte JWF[0x4];
	real_point3d mopp_bounds_min;
	real_point3d mopp_bounds_max;
	s_tag_block breakable_surfaces_mopp_code_block;
	s_tag_block breakable_surface_key_table;
};
COMPILE_ASSERT(sizeof(s_structure_physics) == 0x40);

struct s_structure_soft_ceiling_triangle
{
	real_plane3d plane;
	real_point3d bounding_sphere_center;
	real32 bounding_sphere_radius;
	real_point3d vertex0;
	real_point3d vertex1;
	real_point3d vertex2;
};
COMPILE_ASSERT(sizeof(s_structure_soft_ceiling_triangle) == 0x44);

struct s_structure_soft_ceiling
{
	c_string_id name;
	c_enum<e_soft_ceiling_type, int16, _soft_ceiling_type_acceleration, k_soft_ceiling_type_count> type;
	byte fresh[0x2];
	c_typed_tag_block<s_structure_soft_ceiling_triangle> soft_ceiling_triangles;
};
COMPILE_ASSERT(sizeof(s_structure_soft_ceiling) == 0x14);

struct s_structure_physics_water_group
{
	c_string_id name;
};
COMPILE_ASSERT(sizeof(s_structure_physics_water_group) == sizeof(c_string_id));

struct s_structure_physics_water_instance_debug_triangle
{
	real_point3d points[3];
};
COMPILE_ASSERT(sizeof(s_structure_physics_water_instance_debug_triangle) == sizeof(real_point3d) * 3);

struct s_structure_physics_water_instance
{
	int16 group;
	byte PAD0[0x2];
	real_vector3d direction;
	real32 angle;
	s_tag_block water_planes;
	c_typed_tag_block<s_structure_physics_water_instance_debug_triangle>  water_debug_triangles;
};
COMPILE_ASSERT(sizeof(s_structure_physics_water_instance) == 0x2C);

struct s_structure_design_physics
{
	int32 importer_version;
	s_tag_block soft_ceiling_mopp_code;
	c_typed_tag_block<s_structure_soft_ceiling> soft_ceilings;
	s_tag_block water_mopp;
	c_typed_tag_block<s_structure_physics_water_group> water_groups;
	c_typed_tag_block<s_structure_physics_water_instance> water_instances;
};
COMPILE_ASSERT(sizeof(s_structure_design_physics) == 0x40);

struct s_structure_design
{
	s_structure_design_physics physics;

	real32 pda_z_plane_min; // meters
};
COMPILE_ASSERT(sizeof(s_structure_design) == 0x44);

