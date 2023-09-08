#pragma once

#include "cseries/cseries.hpp"
#include "objects/damage_reporting.hpp"

struct s_simulation_damage_aftermath_event_data
{
	enum e_flags
	{
		_flag_unused_bit0 = 0,
		_flag_shield_depleted_bit,
		_flag_area_of_effect_bit,
		_flag_any_damage_dealt_bit,
		_flag_damaged_by_friendly_bit,
		_flag_silent_bit,
		_flag_killed_instantly_bit,
		_flag_primary_recipient_bit,
		_flag_outside_aoe_dmg_range_bit,
		_flag_unused_bit9,
		_flag_emp_bit,

		k_flags
	};

	long damage_definition_index;
	long __unknown4;
	short damage_owner_player_index;
	bool direction_valid;
	vector3d direction;
	bool epicenter_valid;
	vector3d epicenter_direction_vector;
	real epicenter_direction;
	real shake;
	real shake_scale;
	c_enum<e_flags, long, _flag_unused_bit0, k_flags> flags;
	real shield_damage;
	real body_damage;
	short body_part;
	short node_index;
	long ping_type;
	long special_death_type;
	s_damage_reporting_info damage_reporting_info;
};
static_assert(sizeof(s_simulation_damage_aftermath_event_data) == 0x50);

struct s_simulation_damage_section_response_event_data
{
	long damage_section_index;
	long response_index;
	long damage_section_response_type;
};
static_assert(sizeof(s_simulation_damage_section_response_event_data) == 0xC);

