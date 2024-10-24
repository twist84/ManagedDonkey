#pragma once

#include "cseries/cseries.hpp"
#include "items/items.hpp"
#include "objects/target_tracking.hpp"

enum e_weapon_barrel_state
{
	_weapon_barrel_state_idle = 0,
	_weapon_barrel_state_firing,
	_weapon_barrel_state_locked_recovering,
	_weapon_barrel_state_locked_recovering_empty,
	_weapon_barrel_state_recovering,

	k_weapon_barrel_states
};

enum e_weapon_barrel_flags
{
	_weapon_barrel_fire_bit = 0,
	_weapon_barrel_create_bit,
	_weapon_barrel_dest_bit,
	_weapon_barrel_blur_bit,
	_weapon_barrel_f_b4_charge_bit,
	_weapon_barrel_dmg_bit,
	_weapon_barrel_click_bit,
	_weapon_barrel_fx_bit,

	// anymore flags?

	k_weapon_barrel_flags
};

struct weapon_barrel
{
	char idle_ticks;
	c_enum<e_weapon_barrel_state, char, _weapon_barrel_state_idle, k_weapon_barrel_states> state;
	short timer;
	c_flags<e_weapon_barrel_flags, word, k_weapon_barrel_flags> flags;

	byte __data4[0x16];

	real current_error;
	real angle_change_scale;
	real bonus_round_fraction;
	real overflow;

	byte __data2C[0x8];
};
static_assert(sizeof(weapon_barrel) == 0x34);

enum e_weapon_trigger_state
{
	_weapon_trigger_state_idle = 0,
	_weapon_trigger_state_charging,
	_weapon_trigger_state_charged,
	_weapon_trigger_state_spewing,

	k_weapon_trigger_states
};

enum e_weapon_trigger_flags
{
	_weapon_trigger_rel_bit = 0,
	_weapon_trigger_dest_bit,
	_weapon_trigger_f_b4_charge_bit,
	_weapon_trigger_1aa_bit,
	_weapon_trigger_2aa_bit,
	_weapon_trigger_spew_bit,
	_weapon_trigger_p_charge_bit,
	_weapon_trigger_s_held_bit,

	// anymore flags?

	k_weapon_trigger_flags
};

struct weapon_trigger
{
	c_enum<e_weapon_trigger_state, char, _weapon_trigger_state_idle, k_weapon_trigger_states> state;

	byte __data1[0x3];

	short timer;
	c_flags<e_weapon_trigger_flags, word, k_weapon_trigger_flags> flags;

	byte __data8[0x4];
};
static_assert(sizeof(weapon_trigger) == 0xC);

enum e_weapon_magazine_state
{
	_weapon_magazine_state_idle = 0,
	_weapon_magazine_state_reload_single,
	_weapon_magazine_state_reload_continuous_starting,
	_weapon_magazine_state_reload_continue_underway,
	_weapon_magazine_state_reload_continue_ending,
	_weapon_magazine_state_unchambered,
	_weapon_magazine_state_chambering,
	_weapon_magazine_state_busy,

	k_weapon_magazine_states
};

struct weapon_magazine
{
	c_enum<e_weapon_magazine_state, char, _weapon_magazine_state_idle, k_weapon_magazine_states> state;
	short reload_cooldown;

	short __unknown4;

	short rounds_unloaded;

	short __unknown8;

	short rounds_loaded;

	short __unknownC;
	short __unknownE;
	short __unknown10;

	short firing_cooldown;

	short __unknown14;
	short __unknown16;
	short __unknown18;
};
static_assert(sizeof(weapon_magazine) == 0x1A);

struct weapon_first_person_emulation
{
	byte __data[0x1C];
};
static_assert(sizeof(weapon_first_person_emulation) == 0x1C);

enum e_weapon_flags
{
	_weapon_bit0,
	_weapon_overheated_bit,
	_weapon_overheated_exit_bit,

	k_weapon_flags
};

enum e_weapon_state
{
	_weapon_state_idle = 0,
	_weapon_state_fire_1,
	_weapon_state_fire_2,
	_weapon_state_chamber_1,
	_weapon_state_chamber_2,
	_weapon_state_reload_1,
	_weapon_state_reload_2,
	_weapon_state_charged_1,
	_weapon_state_charged_2,
	_weapon_state_ready,
	_weapon_state_put_away,

	k_weapon_states
};

struct _weapon_datum
{
	c_flags<e_weapon_flags, word, k_weapon_flags> flags;
	word_flags control_flags;
	byte primary_trigger;
	byte last_primary_trigger;
	byte last_hill_or_valley;
	char primary_trigger_direction;
	char primary_trigger_down_ticks;
	char primary_trigger_firing_ticks;
	byte delay_apply_predicted_state_timer;

	byte __unknownB;

	c_enum<e_weapon_state, short, _weapon_state_idle, k_weapon_states> state;
	short state_timer;
	short weapon_disabled_by_reload_timer;

	// weapon_compute_function_value
	// - STRING_ID(global, bomb_arming_amount)
	// - STRING_ID(global, bomb_is_unarmed)
	// - STRING_ID(global, bomb_is_arming)
	// - STRING_ID(global, bomb_is_armed)
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
static_assert(sizeof(_weapon_datum) == 0x150);

struct weapon_datum
{
	long definition_index;
	_object_datum object;
	_item_datum item;
	_weapon_datum weapon;
};
static_assert(sizeof(weapon_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_item_datum) + sizeof(_weapon_datum));

extern bool debug_weapons;
extern bool debug_weapons_triggers;
extern bool debug_weapons_barrels;
extern bool debug_weapons_magazines;
extern bool debug_weapons_primary;
extern bool debug_weapons_secondary;

extern void __cdecl weapon_barrel_fire(long weapon_index, short barrel_index, bool predicted);
extern bool __cdecl weapon_can_be_dual_wielded(long weapon_index);
extern real __cdecl weapon_get_age(long weapon_index);
extern real __cdecl weapon_get_field_of_view_change_time(long weapon_index);
extern bool __cdecl weapon_has_infinite_ammo(long weapon_index);

extern weapon_datum* weapon_get(long weapon_index);

extern void weapons_debug_render_toggle();
extern void weapons_debug_render();
extern void weapon_debug_render(long weapon_index, long weapon_slot);

