#pragma once

#include "cseries/cseries.hpp"

struct s_collision_materials_extant_flags
{
	c_static_flags<512> flags;
};
static_assert(sizeof(s_collision_materials_extant_flags) == 0x40);

struct s_seam_mapping
{
	struct s_seam_cluster_mapping
	{
		real_point3d cluster_center;
		short cluster_indices[2];
		short cluster_mapping_indices[2];
	};
	static_assert(sizeof(s_seam_cluster_mapping) == 0x14);

	char structure_bsp_indices[2];
	short cluster_count;
	c_static_array<s_seam_cluster_mapping, 32> clusters;
};
static_assert(sizeof(s_seam_mapping) == 0x284);

struct s_structure_seam_globals
{
	c_static_array<s_collision_materials_extant_flags, 16> flags;
	c_static_array<s_seam_mapping, 128> seam_mappings;
	c_static_flags<128> active_seams_mask;
	dword_flags connected_bsps_mask;
};
static_assert(sizeof(s_structure_seam_globals) == 0x14614);

