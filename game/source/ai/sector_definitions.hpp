#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct pathfinding_hint_data;

enum e_sector_flags
{
	_sector_walkable_bit = 0,
	_sector_breakable_bit,
	_sector_mobile_bit,
	_sector_bsp_source_bit,
	_sector_floor_bit,
	_sector_ceiling_bit,
	_sector_wall_north_bit,
	_sector_wall_south_bit,
	_sector_wall_east_bit,
	_sector_wall_west_bit,
	_sector_crouchable_bit,
	_sector_aligned_bit,
	_sector_step_bit,
	_sector_interior_bit,
	_sector_rail_bit,
	_sector_user_bit,

	k_sector_flags
};

struct sector
{
	int16 flags; // e_sector_flags
	int16 hint_data;
	int32 first_link_index; // do not set manually
};
static_assert(sizeof(sector) == 0x8);

enum e_sector_link_flags
{
	_sector_link_from_collision_edge_bit = 0,
	_sector_link_section_link_bit,
	_sector_link_bsp2d_creation_error_bit,
	_sector_link_topology_error_bit,
	_sector_link_chain_error_bit,
	_sector_link_threshold_bit,
	_sector_link_crouchable_bit,
	_sector_link_wall_base_bit,
	_sector_link_ledge_bit,
	_sector_link_leanable_bit,
	_sector_link_start_corner_bit,
	_sector_link_end_corner_bit,
	_sector_link_sea_bit,

	k_sector_link_flags
};

struct sector_link
{
	uns16 index;
	uns16 index2;
	int16 flags; // e_sector_link_flags
	int16 hint_index;
	uns16 links[2]; // forward, reverse
	uns16 sectors[2]; // left, right
};
static_assert(sizeof(sector_link) == 0x10);

struct sector_vertex
{
	real_point3d point;
};
static_assert(sizeof(sector_vertex) == sizeof(real_point3d));

struct pathfinding_data
{
	c_typed_tag_block<sector> sectors;
	c_typed_tag_block<sector_link> sector_links;
	s_tag_block bsp2d_refs;
	s_tag_block bsp2d_nodes;
	c_typed_tag_block<sector_vertex> sector_vertices;
	s_tag_block object_refs;
	c_typed_tag_block<pathfinding_hint_data> hints;
	s_tag_block instanced_geometry_refs;
	uns32 structure_checksum;
	s_tag_block giant_pathfinding;
	s_tag_block seams;
	s_tag_block jump_seams;
	s_tag_block doors;
};
static_assert(sizeof(pathfinding_data) == 0x94);

