#pragma once

#include "cseries/cseries.hpp"
#include "items/items.hpp"
#include "objects/target_tracking.hpp"

struct weapon_barrel
{
	byte __data0[0x34];
};
static_assert(sizeof(weapon_barrel) == 0x34);

struct weapon_trigger
{
	byte __data0[0xC];
};
static_assert(sizeof(weapon_trigger) == 0xC);

struct weapon_magazine
{
	byte __data0[0x1A];
};
static_assert(sizeof(weapon_magazine) == 0x1A);

struct weapon_first_person_emulation
{
	byte __data[0x1C];
};
static_assert(sizeof(weapon_first_person_emulation) == 0x1C);

struct weapon_data
{
	word_flags flags;
	word_flags weapon_control_flags;
	byte primary_trigger;
	byte last_primary_trigger;
	byte last_hill_or_valley;
	char primary_trigger_direction;
	char primary_trigger_down_ticks;
	char primary_trigger_firing_ticks;
	byte delay_apply_predicted_state_timer;

	byte __unknownB;

	short state;
	short state_timer;
	short weapon_disabled_by_reload_timer;

	// weapon_compute_function_value
	// - _global_string_id_bomb_arming_amount
	// - _global_string_id_bomb_is_unarmed
	// - _global_string_id_bomb_is_arming
	// - _global_string_id_bomb_is_armed
	// if (game_is_multiplayer() && weapon->weapon.__unknown12 != NONE && object_is_ctf_flag(object))
	short __unknown12;

	short multiplayer_weapon_identifier;
	byte turn_on_timer;
	byte ready_for_use_timer;
	real heat;
	real age;
	real delayed_age;
	real overcharged;
	real current_power;
	real desired_power;
	c_target_tracking_system tracked_target;
	
	byte __data70[0x4];

	short alternate_shots_loaded;

	byte __data76[0x2];

	weapon_barrel const barrels[2];
	weapon_trigger const triggers[2];
	weapon_magazine const magazines[2];
	long overheated_effect_index;
	long game_time_last_fired;
	weapon_first_person_emulation first_person_emulation;
};
static_assert(sizeof(weapon_data) == 0x150);

struct weapon_datum
{
	object_data object;
	item_data item;
	weapon_data weapon;
};
static_assert(sizeof(weapon_datum) == sizeof(object_data) + sizeof(item_data) + sizeof(weapon_data));

extern real __cdecl weapon_get_age(long weapon_index);
extern real __cdecl weapon_get_field_of_view_change_time(long weapon_index);
extern bool __cdecl weapon_has_infinite_ammo(long weapon_index);

