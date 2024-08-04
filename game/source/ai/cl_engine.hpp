#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

// #TODO: confirm the following enum

enum e_ai_atom
{
	_ai_atom_pause = 0,
	_ai_atom_go_to,
	_ai_atom_go_by,
	_ai_atom_go_to_and_face,
	_ai_atom_go_to_and_posture,
	_ai_atom_move_direction,
	_ai_atom_move_towards,

	// ODST
	_ai_atom_move_towards_point,

	_ai_atom_grenade,
	_ai_atom_equipment,
	_ai_atom_vehicle,
	_ai_atom_jump,
	_ai_atom_targeted_jump,
	_ai_atom_recording,
	_ai_atom_action,
	_ai_atom_vocalize,
	_ai_atom_die,
	_ai_atom_behavior,
	_ai_atom_fly_to,
	_ai_atom_fly_by,
	_ai_atom_fly_to_and_face,
	_ai_atom_formation,
	_ai_atom_approach,
	_ai_atom_deploy_turret,
	_ai_atom_custom_animation,
	_ai_atom_play_sound,
	_ai_atom_melee,
	_ai_atom_smash,
	_ai_atom_swarm_to,
	_ai_atom_swarm_from,

	k_ai_atom_count
};

struct cs_command
{
	byte atom_type; // e_ai_atom

	byte __data[0x2B];
};
static_assert(sizeof(cs_command) == 0x2C);

struct s_atom_control
{
	short __unknown0;
	byte __data2[0x2];
	short __unknown4;
	byte __data[0x1E];
};
static_assert(sizeof(s_atom_control) == 0x24);

struct command_script_datum :
	s_datum_header
{
	short type;
	long actor_index;
	long script_index;
	long thread_index;
	long next_cs;
	bool reserved;
	bool abort;
	bool reserve_mandatory;
	bool abort_on_alert;
	bool abort_on_damage;
	bool abort_on_combat_status;
	short abort_on_combat_status_level;
	bool abort_on_vehicle_exit;

	byte __pad1D[0x1];

	short role_index;
	short priority;

	byte __pad22[0x2];

	cs_command const command[3];
	s_atom_control const control[3];
	bool shoot_at_target;
	bool shoot_secondary_trigger;
	bool script_initiated_posture;
	bool script_initiated_custom_animation;

	byte __data118[0x70];
};
static_assert(sizeof(command_script_datum) == 0x188);

struct cs_script_data;
struct cs_point_set;

extern cs_script_data* cs_scenario_get_script_data(struct scenario* scenario);
extern cs_point_set* cs_get_point_set(long index);

