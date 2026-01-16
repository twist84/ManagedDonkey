#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct cs_point
{
	c_static_string<k_tag_string_length> name;
	real_point3d position;

	// ai_reference_frame
	int16 manual_reference_frame; // short_block_index

	// scenario_structure_bsp_reference
	int16 structure_bsp; // short_block_index

	int16 bsp_index;
	int16 sector_index;

	real_euler_angles2d facing_direction;
};
COMPILE_ASSERT(sizeof(cs_point) == 0x3C);

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
	int16 manual_bsp_index; // short_block_index

	// ai_reference_frame
	int16 manual_reference_frame; // short_block_index

	c_flags<e_point_set_flags, uns32, k_point_set_flags> flags;

	// s_scenario_editor_folder
	int16 editor_folder; // short_block_index

	// pad
	byte AJDEYNFD[0x2];
};
COMPILE_ASSERT(sizeof(cs_point_set) == 0x38);

struct cs_script_data
{
	c_typed_tag_block<cs_point_set> point_sets;
	int32 pad1[0x1E];
};
COMPILE_ASSERT(sizeof(cs_script_data) == 0x84);

extern int16 cs_point_set_index_by_name(const char* name);
extern int16 cs_point_index_by_name(cs_point_set* point_set, const char* name);

