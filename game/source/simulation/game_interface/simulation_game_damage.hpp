#pragma once

#include "cseries/cseries.hpp"
#include "objects/damage_reporting.hpp"

struct s_simulation_damage_aftermath_event_data
{
	int32 damage_definition_index;
	int32 __unknown4;
	int16 damage_owner_player_index;
	bool direction_valid;
	real_vector3d direction;
	bool epicenter_valid;
	real_vector3d epicenter_direction_vector;
	real32 epicenter_direction;
	real32 shake;
	real32 shake_scale;
	c_enum<e_damage_aftermath_flags, int32, _damage_aftermath_body_depleted_bit, k_damage_aftermath_flags> flags;
	real32 shield_damage;
	real32 body_damage;
	int16 body_part;
	int16 node_index;
	int32 ping_type;
	int32 special_death_type;
	s_damage_reporting_info damage_reporting_info;
};
static_assert(sizeof(s_simulation_damage_aftermath_event_data) == 0x50);

struct s_simulation_damage_section_response_event_data
{
	int32 damage_section_index;
	int32 response_index;
	int32 damage_section_response_type;
};
static_assert(sizeof(s_simulation_damage_section_response_event_data) == 0xC);

