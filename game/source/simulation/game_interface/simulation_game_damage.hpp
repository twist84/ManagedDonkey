#pragma once

#include "cseries/cseries.hpp"
#include "objects/damage_reporting.hpp"
#include "shell/shell.hpp"

struct s_simulation_damage_aftermath_event_data
{
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
	c_enum<e_damage_aftermath_flags, long, _damage_aftermath_unknown0_bit, k_damage_aftermath_flags> flags;
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

