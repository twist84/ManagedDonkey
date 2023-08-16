#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct cs_point
{
	c_static_string<32> name;
	real_point3d position;

	// ai_reference_frame
	short manual_reference_frame; // short_block_index

	// scenario_structure_bsp_reference
	short structure_bsp; // short_block_index

	short bsp_index;
	short sector_index;

	euler_angles2d facing_direction;
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
	c_static_string<32> name;
	s_tag_block points;

	// scenario_structure_bsp_reference
	short bsp_index; // short_block_index

	// ai_reference_frame
	short manual_reference_frame; // short_block_index

	c_flags<e_point_set_flags, dword, k_point_set_flags> flags;

	// s_scenario_editor_folder
	short editor_folder; // short_block_index

	// pad
	byte AJDEYNFD[2];
};
static_assert(sizeof(cs_point_set) == 0x38);

struct cs_script_data
{
	c_typed_tag_block<cs_point_set> point_sets;

	// pad
	byte TPHWNCUR[0x78];
};
static_assert(sizeof(cs_script_data) == 0x84);