#pragma once

#include "cseries/cseries.hpp"
#include "items/items.hpp"
#include "objects/target_tracking.hpp"

enum e_unit_messages
{
	_unit_message_idle = 0,
	_unit_message_fire_1,
	_unit_message_fire_2,
	_unit_message_misfire_1,
	_unit_message_misfire_2,
	_unit_message_light_on,
	_unit_message_light_off,
	_unit_message_reload_empty,
	_unit_message_reload_full,
	_unit_message_reload_enter,
	_unit_message_reload_continue_empty,
	_unit_message_reload_continue_full,
	_unit_message_reload_exit,
	_unit_message_overcharged,
	_unit_message_overheating,
	_unit_message_o_h_exit,
	_unit_message_unknown16,
	_unit_message_melee,
	_unit_message_put_away_1,
	_unit_message_put_away_2,
	_unit_message_ready_1,
	_unit_message_ready_2,
	_unit_message_ready_overheated_1,
	_unit_message_ready_overheated_2,
	_unit_message_unknown24,
	_unit_message_unknown25,
	_unit_message_throw_grenade,
	_unit_message_throw_overheated,
	_unit_message_throw_equipment,
	_unit_message_melee_1sthit,
	_unit_message_melee_1sthit2idle,
	_unit_message_melee_2ndhit,
	_unit_message_melee_2ndhit2idle,
	_unit_message_melee_3rdhit,
	_unit_message_melee_strike_1,
	_unit_message_melee_strike_2,
	_unit_message_melee_strike_3,
	_unit_message_melee_strike_4,
	_unit_message_melee_dash,
	_unit_message_melee_lunge,
	_unit_message_melee_dash_airborne,
	_unit_message_melee_lunge_airborne,
	_unit_message_melee_lunge_unaimed,
	_unit_message_melee_lunge_unaimed_airborne,
	_unit_message_jump,
	_unit_message_fall,
	_unit_message_land_soft,
	_unit_message_land_hard,
	_unit_message_sprint_enter,

	k_unit_messages
};

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
	int8 idle_ticks;
	c_enum<e_weapon_barrel_state, int8, _weapon_barrel_state_idle, k_weapon_barrel_states> state;
	int16 timer;
	c_flags<e_weapon_barrel_flags, uns16, k_weapon_barrel_flags> flags;

	byte __data4[0x16];

	real32 current_error;
	real32 angle_change_scale;
	real32 bonus_round_fraction;
	real32 overflow;

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
	c_enum<e_weapon_trigger_state, int8, _weapon_trigger_state_idle, k_weapon_trigger_states> state;

	byte __data1[0x3];

	int16 timer;
	c_flags<e_weapon_trigger_flags, uns16, k_weapon_trigger_flags> flags;

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
	c_enum<e_weapon_magazine_state, int8, _weapon_magazine_state_idle, k_weapon_magazine_states> state;
	int16 reload_cooldown;

	int16 __unknown4;

	int16 rounds_unloaded;

	int16 __unknown8;

	int16 rounds_loaded;

	int16 __unknownC;
	int16 __unknownE;
	int16 __unknown10;

	int16 firing_cooldown;

	int16 __unknown14;
	int16 __unknown16;
	int16 __unknown18;
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

	// anymore flags?

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
	c_flags<e_weapon_flags, uns16, k_weapon_flags> flags;
	uns16 control_flags;
	uns8 primary_trigger;
	uns8 last_primary_trigger;
	uns8 last_hill_or_valley;
	int8 primary_trigger_direction;
	int8 primary_trigger_down_ticks;
	int8 primary_trigger_firing_ticks;
	uns8 delay_apply_predicted_state_timer;

	uns8 __unknownB;

	c_enum<e_weapon_state, int16, _weapon_state_idle, k_weapon_states> state;
	int16 state_timer;
	int16 weapon_disabled_by_reload_timer;

	// weapon_compute_function_value
	// - STRING_ID(global, bomb_arming_amount)
	// - STRING_ID(global, bomb_is_unarmed)
	// - STRING_ID(global, bomb_is_arming)
	// - STRING_ID(global, bomb_is_armed)
	// if (game_is_multiplayer() && weapon->weapon.__unknown12 != NONE && object_is_ctf_flag(object))
	int16 __unknown12;

	int16 multiplayer_weapon_identifier;
	uns8 turn_on_timer;
	uns8 ready_for_use_timer;
	real32 heat;
	real32 age;
	real32 delayed_age;
	real32 overcharged;
	real32 current_power;
	real32 desired_power;
	c_target_tracking_system tracked_target;
	
	byte __data70[0x4];

	int16 alternate_shots_loaded;

	byte __data76[0x2];

	weapon_barrel const barrels[2];
	weapon_trigger const triggers[2];
	weapon_magazine const magazines[2];
	int32 overheated_effect_index;
	int32 game_time_last_fired;
	weapon_first_person_emulation first_person_emulation;
};
static_assert(sizeof(_weapon_datum) == 0x150);

struct weapon_datum
{
	int32 definition_index;
	_object_datum object;
	_item_datum item;
	_weapon_datum weapon;
};
static_assert(sizeof(weapon_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_item_datum) + sizeof(_weapon_datum));

extern bool debug_weapons;
extern bool debug_weapons_triggers;
extern bool debug_weapons_barrels;
extern bool debug_weapons_magazines;
extern bool debug_weapons_primary;
extern bool debug_weapons_secondary;

extern void __cdecl weapon_barrel_fire(int32 weapon_index, int16 barrel_index, bool predicted);
extern bool __cdecl weapon_can_be_dual_wielded(int32 weapon_index);
extern void __cdecl weapon_debug_render(int32 weapon_index, int32 weapon_slot);
extern real32 __cdecl weapon_get_age(int32 weapon_index);
extern real32 __cdecl weapon_get_field_of_view_change_time(int32 weapon_index);
extern bool __cdecl weapon_handle_potential_inventory_item(int32 weapon_index, int32 item_index, int32 unit_index, int32 player_index, int16* rounds_picked_up);
extern bool __cdecl weapon_has_infinite_ammo(int32 weapon_index);
extern bool __cdecl weapon_is_support_weapon(int32 weapon_index);
extern void __cdecl weapons_debug_render();
extern void __cdecl weapons_debug_render_toggle();

