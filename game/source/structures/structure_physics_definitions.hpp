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
static_assert(sizeof(s_structure_physics) == 0x40);

