#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

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
	short vertex1;
	short vertex2;
	c_flags<e_sector_link_flags, word, k_sector_link_flags> link_flags;
	short hint_index;
	short forward_link;
	short reverse_link;
	word left_sector;
	word right_sector;
};
static_assert(sizeof(sector_link) == 0x10);

struct sector_vertex
{
	real_point3d point;
};
static_assert(sizeof(sector_vertex) == sizeof(real_point3d));

enum e_hint_type
{
	_hint_type_intersection_link = 0,
	_hint_type_jump_link,
	_hint_type_climb_link,
	_hint_type_vault_link,
	_hint_type_mount_link,
	_hint_type_hoist_link,
	_hint_type_wall_jump_link,
	_hint_type_breakable_floor,
	_hint_type_rail_link,
	_hint_type_seam_link,
	_hint_type_door_link,

	k_hint_type_count
};

struct pathfinding_hint_data
{
	c_enum<e_hint_type, short, _hint_type_intersection_link, k_hint_type_count> hint_type;
	short next_hint_index;
	long hint_data[4];
};
static_assert(sizeof(pathfinding_hint_data) == 0x14);

struct pathfinding_data
{
	s_tag_block sectors;
	c_typed_tag_block<sector_link> links;
	s_tag_block bsp2d_refs;
	s_tag_block bsp2d_nodes;
	c_typed_tag_block<sector_vertex> vertices;
	s_tag_block object_refs;
	c_typed_tag_block<pathfinding_hint_data> pathfinding_hints;
	s_tag_block instanced_geometry_refs;
	dword structure_checksum;
	s_tag_block giant_pathfinding_data;
	s_tag_block seams;
	s_tag_block jump_seams;
	s_tag_block doors;
};
static_assert(sizeof(pathfinding_data) == 0x94);

