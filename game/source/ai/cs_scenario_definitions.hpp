#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct cs_point
{
	c_static_string<k_tag_string_length> name;
	real_point3d position;

	// ai_reference_frame
	short manual_reference_frame; // short_block_index

	// scenario_structure_bsp_reference
	short structure_bsp; // short_block_index

	short bsp_index;
	short sector_index;

	real_euler_angles2d facing_direction;
};
static_assert(sizeof(cs_point) == 0x3C);

enum e_point_set_flags
{
	_point_set_flag_manual_reference_frame_bit = 0,
	_point_set_flag_turret_deployment_bit,
	_point_set_flag_giant_set_bit,
	_point_set_flag_invalid_sector_refs_bit,

	k_point_set_flags
};

struct cs_point_set
{
	char name[32];
	c_typed_tag_block<cs_point> point_data;

	// scenario_structure_bsp_reference
	short manual_bsp_index; // short_block_index

	// ai_reference_frame
	short manual_reference_frame; // short_block_index

	c_flags<e_point_set_flags, uint32, k_point_set_flags> flags;

	// s_scenario_editor_folder
	short editor_folder; // short_block_index

	// pad
	uint8 AJDEYNFD[2];
};
static_assert(sizeof(cs_point_set) == 0x38);

struct cs_script_data
{
	c_typed_tag_block<cs_point_set> point_sets;
	long pad1[0x1E];
};
static_assert(sizeof(cs_script_data) == 0x84);

extern short cs_point_set_index_by_name(char const* name);
extern short cs_point_index_by_name(cs_point_set* point_set, char const* name);

