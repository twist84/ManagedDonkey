#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"

enum e_weapon_set
{
	_weapon_set_primary = 0,
	_weapon_set_secondary,

	k_weapon_set_count
};

struct s_unit_weapon_set
{
	word set_identifier;
	c_static_array<byte, k_weapon_set_count> weapon_indices;
};

struct unit_control_data
{
	long animation_seat;
	short aiming_speed;
	s_unit_weapon_set weapon_set;
	word grenade_index;
	word zoom_level;
	dword_flags control_flags;
	vector3d throttle;
	real primary_trigger;
	real secondary_trigger;
	vector3d facing_vector;
	vector3d aiming_vector;
	vector3d looking_vector;
	real_point3d gaze_position;
	s_aim_assist_targeting_result aim_assist_data;
};
static_assert(sizeof(unit_control_data) == 0x80);

struct unit_datum;

extern void __cdecl unit_add_equipment_to_inventory(long unit_index, long slot_index, long object_index);
extern bool __cdecl unit_add_grenade_to_inventory(long unit_index, long object_index);
extern short __cdecl unit_add_grenade_type_to_inventory(long unit_index, short grenade_type, short grenade_count);
extern void __cdecl unit_add_starting_profile_equipment(long unit_index, short profile_index, bool clear_player, bool create_new);
extern bool __cdecl unit_add_weapon_to_inventory(long unit_index, long object_index, long weapon_addition_method);
extern bool __cdecl unit_has_weapon_definition_index(long unit_index, long weapon_definition_index);

extern bool __cdecl units_debug_can_select_unit(long unit_index);
extern long __cdecl units_debug_get_closest_unit(long unit_index);
extern long __cdecl units_debug_get_next_unit(long unit_index);
extern void __cdecl debug_rotate_units_callback(bool enable);
extern void __cdecl debug_rotate_all_units_callback(bool enable);

