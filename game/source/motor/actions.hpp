#pragma once

#include "cseries/cseries.hpp"

enum e_action
{
	_action_none = -1,
	_action_death_ping,
	_action_custom_animation,
	_action_stop_custom_animation,
	_action_sync_container,
	_action_assassinate,
	_action_get_infected,
	_action_hostile_morph,
	_action_resurrect,
	_action_hard_ping,
	_action_ping,
	_action_morph,
	_action_morph_arrival,
	_action_infect,
	_action_posture,
	_action_posture_exit,
	_action_vehicle_exit,
	_action_vehicle_exit_and_detach,
	_action_vehicle_ejection,
	_action_vehicle_board,
	_action_vehicle_enter,
	_action_open,
	_action_close,
	_action_vehicle_exit_immediate,
	_action_vehicle_flip,
	_action_climb_attach,
	_action_climb_detach,
	_action_wall_lean,
	_action_wall_lean_open,
	_action_token,
	_action_takeoff,
	_action_perch,
	_action_perch_jump,
	_action_land,
	_action_jump,
	_action_leap_anticipation,
	_action_leap,
	_action_leap_recovery,
	_action_emit_ai,
	_action_berserk,
	_action_flurry,
	_action_deploy_turret,
	_action_smash,
	_action_vault,
	_action_hoist,
	_action_ai_impulse,
	_action_melee_ai,
	_action_weapon_pickup,
	_action_weapon_trade,
	_action_primary_weapon_drop,
	_action_secondary_weapon_drop,
	_action_surge,
	_action_phase,
	_action_stow,
	_action_draw,
	_action_infection_spew,
	_action_kamikaze,
	_action_primary_weapon_switch,
	_action_secondary_weapon_switch,
	_action_grenade,
	_action_equipment,
	_action_equipment_animation,
	_action_equipment_pickup,
	_action_equipment_drop,
	_action_melee_player,
	_action_primary_weapon_primary_reload,
	_action_primary_weapon_secondary_reload,
	_action_secondary_weapon_primary_reload,
	_action_secondary_weapon_secondary_reload,
	_action_soft_ping,
	_action_primary_weapon_primary_recoil,
	_action_primary_weapon_secondary_recoil,
	_action_primary_weapon_primary_chamber,
	_action_primary_weapon_secondary_chamber,
	_action_primary_weapon_primary_charged,
	_action_primary_weapon_secondary_charged,
	_action_secondary_weapon_primary_recoil,
	_action_secondary_weapon_secondary_recoil,
	_action_secondary_weapon_primary_chamber,
	_action_secondary_weapon_secondary_chamber,
	_action_secondary_weapon_primary_charged,
	_action_secondary_weapon_secondary_charged,
	_action_primary_weapon_overheat_enter,
	_action_primary_weapon_overheat_loop,
	_action_primary_weapon_overheat_exit,
	_action_secondary_weapon_overheat_enter,
	_action_secondary_weapon_overheat_loop,
	_action_secondary_weapon_overheat_exit,

	k_action_count
};

struct action_request_death_ping
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_death_ping) == 0x44);

struct action_request_custom_animation
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_custom_animation) == 0x44);

struct action_request_stop_custom_animation
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_stop_custom_animation) == 0x44);

struct action_request_sync_container
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_sync_container) == 0x44);

struct action_request_assassinate
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_assassinate) == 0x44);

struct action_request_get_infected
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_get_infected) == 0x44);

struct action_request_hostile_morph
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_hostile_morph) == 0x44);

struct action_request_resurrect
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_resurrect) == 0x44);

struct action_request_hard_ping
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_hard_ping) == 0x44);

struct action_request_ping
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_ping) == 0x44);

struct action_request_morph
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_morph) == 0x44);

struct action_request_morph_arrival
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_morph_arrival) == 0x44);

struct action_request_infect
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_infect) == 0x44);

struct action_request_posture
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_posture) == 0x44);

struct action_request_posture_exit
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_posture_exit) == 0x44);

struct action_request_vehicle_exit
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_vehicle_exit) == 0x44);

struct action_request_vehicle_exit_and_detach
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_vehicle_exit_and_detach) == 0x44);

struct action_request_vehicle_ejection
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_vehicle_ejection) == 0x44);

struct action_request_vehicle_board
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_vehicle_board) == 0x44);

struct action_request_vehicle_enter
{
	int32 vehicle_index;
	int16 seat_index;
	bool immediate;
	bool force_entry;

	byte __data[0x44 - 0x8];
};
static_assert(sizeof(action_request_vehicle_enter) == 0x44);

struct action_request_open
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_open) == 0x44);

struct action_request_close
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_close) == 0x44);

struct action_request_vehicle_exit_immediate
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_vehicle_exit_immediate) == 0x44);

struct action_request_vehicle_flip
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_vehicle_flip) == 0x44);

struct action_request_climb_attach
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_climb_attach) == 0x44);

struct action_request_climb_detach
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_climb_detach) == 0x44);

struct action_request_wall_lean
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_wall_lean) == 0x44);

struct action_request_wall_lean_open
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_wall_lean_open) == 0x44);

struct action_request_token
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_token) == 0x44);

struct action_request_takeoff
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_takeoff) == 0x44);

struct action_request_perch
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_perch) == 0x44);

struct action_request_perch_jump
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_perch_jump) == 0x44);

struct action_request_land
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_land) == 0x44);

struct action_request_jump
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_jump) == 0x44);

struct action_request_leap_anticipation
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_leap_anticipation) == 0x44);

struct action_request_leap
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_leap) == 0x44);

struct action_request_leap_recovery
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_leap_recovery) == 0x44);

struct action_request_emit_ai
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_emit_ai) == 0x44);

struct action_request_berserk
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_berserk) == 0x44);

struct action_request_flurry
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_flurry) == 0x44);

struct action_request_deploy_turret
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_deploy_turret) == 0x44);

struct action_request_smash
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_smash) == 0x44);

struct action_request_vault
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_vault) == 0x44);

struct action_request_hoist
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_hoist) == 0x44);

struct action_request_ai_impulse
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_ai_impulse) == 0x44);

struct action_request_melee_ai
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_melee_ai) == 0x44);

struct action_request_weapon_pickup
{
	int32 weapon_index;
	int32 add_weapon_mode; // e_weapon_addition_method

	byte __data[0x44 - 0x8];
};
static_assert(sizeof(action_request_weapon_pickup) == 0x44);

struct action_request_weapon_trade
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_weapon_trade) == 0x44);

struct action_request_primary_weapon_drop
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_drop) == 0x44);

struct action_request_secondary_weapon_drop
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_drop) == 0x44);

struct action_request_surge
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_surge) == 0x44);

struct action_request_phase
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_phase) == 0x44);

struct action_request_stow
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_stow) == 0x44);

struct action_request_draw
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_draw) == 0x44);

struct action_request_infection_spew
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_infection_spew) == 0x44);

struct action_request_kamikaze
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_kamikaze) == 0x44);

struct action_request_primary_weapon_switch
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_switch) == 0x44);

struct action_request_secondary_weapon_switch
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_switch) == 0x44);

struct action_request_grenade
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_grenade) == 0x44);

struct action_request_equipment
{
	int32 spawner_object_definition_index;
	bool network_predicted;
	bool network_throw_immediate;
	bool network_death_equipment;
	real_point3d network_aiming_origin;
	real_vector3d network_aiming_vector;

	byte __data[0x44 - 0x20];
};
static_assert(sizeof(action_request_equipment) == 0x44);

struct action_request_equipment_animation
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_equipment_animation) == 0x44);

struct action_request_equipment_pickup
{
	int32 equipment_index;

	byte __data[0x44 - 0x4];
};
static_assert(sizeof(action_request_equipment_pickup) == 0x44);

struct action_request_equipment_drop
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_equipment_drop) == 0x44);

struct action_request_melee_player
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_melee_player) == 0x44);

struct action_request_primary_weapon_primary_reload
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_primary_reload) == 0x44);

struct action_request_primary_weapon_secondary_reload
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_secondary_reload) == 0x44);

struct action_request_secondary_weapon_primary_reload
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_primary_reload) == 0x44);

struct action_request_secondary_weapon_secondary_reload
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_secondary_reload) == 0x44);

struct action_request_soft_ping
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_soft_ping) == 0x44);

struct action_request_primary_weapon_primary_recoil
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_primary_recoil) == 0x44);

struct action_request_primary_weapon_secondary_recoil
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_secondary_recoil) == 0x44);

struct action_request_primary_weapon_primary_chamber
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_primary_chamber) == 0x44);

struct action_request_primary_weapon_secondary_chamber
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_secondary_chamber) == 0x44);

struct action_request_primary_weapon_primary_charged
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_primary_charged) == 0x44);

struct action_request_primary_weapon_secondary_charged
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_secondary_charged) == 0x44);

struct action_request_secondary_weapon_primary_recoil
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_primary_recoil) == 0x44);

struct action_request_secondary_weapon_secondary_recoil
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_secondary_recoil) == 0x44);

struct action_request_secondary_weapon_primary_chamber
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_primary_chamber) == 0x44);

struct action_request_secondary_weapon_secondary_chamber
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_secondary_chamber) == 0x44);

struct action_request_secondary_weapon_primary_charged
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_primary_charged) == 0x44);

struct action_request_secondary_weapon_secondary_charged
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_secondary_charged) == 0x44);

struct action_request_primary_weapon_overheat_enter
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_overheat_enter) == 0x44);

struct action_request_primary_weapon_overheat_loop
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_overheat_loop) == 0x44);

struct action_request_primary_weapon_overheat_exit
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_primary_weapon_overheat_exit) == 0x44);

struct action_request_secondary_weapon_overheat_enter
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_overheat_enter) == 0x44);

struct action_request_secondary_weapon_overheat_loop
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_overheat_loop) == 0x44);

struct action_request_secondary_weapon_overheat_exit
{
	byte __data[0x44];
};
static_assert(sizeof(action_request_secondary_weapon_overheat_exit) == 0x44);

struct action_request
{
	c_enum<e_action, int32, _action_death_ping, k_action_count> type;

	union
	{
		action_request_death_ping death_ping;
		action_request_custom_animation custom_animation;
		action_request_stop_custom_animation stop_custom_animation;
		action_request_sync_container sync_container;
		action_request_assassinate assassinate;
		action_request_get_infected get_infected;
		action_request_hostile_morph hostile_morph;
		action_request_resurrect resurrect;
		action_request_hard_ping hard_ping;
		action_request_ping ping;
		action_request_morph morph;
		action_request_morph_arrival morph_arrival;
		action_request_infect infect;
		action_request_posture posture;
		action_request_posture_exit posture_exit;
		action_request_vehicle_exit vehicle_exit;
		action_request_vehicle_exit_and_detach vehicle_exit_and_detach;
		action_request_vehicle_ejection vehicle_ejection;
		action_request_vehicle_board vehicle_board;
		action_request_vehicle_enter vehicle_enter;
		action_request_open open;
		action_request_close close;
		action_request_vehicle_exit_immediate vehicle_exit_immediate;
		action_request_vehicle_flip vehicle_flip;
		action_request_climb_attach climb_attach;
		action_request_climb_detach climb_detach;
		action_request_wall_lean wall_lean;
		action_request_wall_lean_open wall_lean_open;
		action_request_token token;
		action_request_takeoff takeoff;
		action_request_perch perch;
		action_request_perch_jump perch_jump;
		action_request_land land;
		action_request_jump jump;
		action_request_leap_anticipation leap_anticipation;
		action_request_leap leap;
		action_request_leap_recovery leap_recovery;
		action_request_emit_ai emit_ai;
		action_request_berserk berserk;
		action_request_flurry flurry;
		action_request_deploy_turret deploy_turret;
		action_request_smash smash;
		action_request_vault vault;
		action_request_hoist hoist;
		action_request_ai_impulse ai_impulse;
		action_request_melee_ai melee_ai;
		action_request_weapon_pickup weapon_pickup;
		action_request_weapon_trade weapon_trade;
		action_request_primary_weapon_drop primary_weapon_drop;
		action_request_secondary_weapon_drop secondary_weapon_drop;
		action_request_surge surge;
		action_request_phase phase;
		action_request_stow stow;
		action_request_draw draw;
		action_request_infection_spew infection_spew;
		action_request_kamikaze kamikaze;
		action_request_primary_weapon_switch primary_weapon_switch;
		action_request_secondary_weapon_switch secondary_weapon_switch;
		action_request_grenade grenade;
		action_request_equipment equipment;
		action_request_equipment_animation equipment_animation;
		action_request_equipment_pickup equipment_pickup;
		action_request_equipment_drop equipment_drop;
		action_request_melee_player melee_player;
		action_request_primary_weapon_primary_reload primary_weapon_primary_reload;
		action_request_primary_weapon_secondary_reload primary_weapon_secondary_reload;
		action_request_secondary_weapon_primary_reload secondary_weapon_primary_reload;
		action_request_secondary_weapon_secondary_reload secondary_weapon_secondary_reload;
		action_request_soft_ping soft_ping;
		action_request_primary_weapon_primary_recoil primary_weapon_primary_recoil;
		action_request_primary_weapon_secondary_recoil primary_weapon_secondary_recoil;
		action_request_primary_weapon_primary_chamber primary_weapon_primary_chamber;
		action_request_primary_weapon_secondary_chamber primary_weapon_secondary_chamber;
		action_request_primary_weapon_primary_charged primary_weapon_primary_charged;
		action_request_primary_weapon_secondary_charged primary_weapon_secondary_charged;
		action_request_secondary_weapon_primary_recoil secondary_weapon_primary_recoil;
		action_request_secondary_weapon_secondary_recoil secondary_weapon_secondary_recoil;
		action_request_secondary_weapon_primary_chamber secondary_weapon_primary_chamber;
		action_request_secondary_weapon_secondary_chamber secondary_weapon_secondary_chamber;
		action_request_secondary_weapon_primary_charged secondary_weapon_primary_charged;
		action_request_secondary_weapon_secondary_charged secondary_weapon_secondary_charged;
		action_request_primary_weapon_overheat_enter primary_weapon_overheat_enter;
		action_request_primary_weapon_overheat_loop primary_weapon_overheat_loop;
		action_request_primary_weapon_overheat_exit primary_weapon_overheat_exit;
		action_request_secondary_weapon_overheat_enter secondary_weapon_overheat_enter;
		action_request_secondary_weapon_overheat_loop secondary_weapon_overheat_loop;
		action_request_secondary_weapon_overheat_exit secondary_weapon_overheat_exit;

		byte storage[0x44];
	};
};
static_assert(sizeof(action_request) == 0x4 + 0x44);

extern bool __cdecl action_executing(int32 mover_index, e_action action_type);
extern bool __cdecl action_submit(int32 mover_index, const action_request* request);
extern bool __cdecl action_submit(int32 mover_index, e_action action_type);

