#pragma once

#include "ai/actors.hpp"
#include "cseries/cseries.hpp"

// $TODO: confirm the following enum

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
	char atom_type;
	char flags;
	short atom_modifier;

	union
	{
		long integer_parameter1;
		real parameter1;
		bool bool_param1;
	};

	union
	{
		long integer_parameter2;
		real parameter2;
		bool bool_param2;
	};

	real_point3d point1;
	real_point3d point2;
	long ref_index;
	long ref_index2;
};
static_assert(sizeof(cs_command) == 0x2C);

struct s_control_directmovement
{
	short facing;
	real_vector3d vector;
	real_point3d start_position;
};
static_assert(sizeof(s_control_directmovement) == 0x1C);

struct s_control_point_destination
{
	c_ai_point3d point;
	real radius;
};
static_assert(sizeof(s_control_point_destination) == 0x14);

struct s_control_move_towards
{
	long object_index;
};
static_assert(sizeof(s_control_move_towards) == 0x4);

struct s_control_jump
{
	short delay_ticks;
	real target_horizontal_vel;
	real target_vertical_vel;
};
static_assert(sizeof(s_control_jump) == 0xC);

struct s_atom_control
{
	short atom_type;
	short pause_timer;
	short status;
	short simple_control_flags;

	union
	{
		s_control_directmovement directmovement;
		s_control_point_destination point_destination;
		s_control_move_towards move_towards;
		s_control_jump jump;
	};
};
static_assert(sizeof(s_atom_control) == 0x24);

struct cs_target_specification
{
	short type;
	long index;
};
static_assert(sizeof(cs_target_specification) == 0x8);

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
	short role_index;
	short priority;
	cs_command command[3];
	s_atom_control control[3];
	bool shoot_at_target;
	bool shoot_secondary_trigger;
	bool script_initiated_posture;
	bool script_initiated_custom_animation;
	bool walk;
	bool crouch;
	real crouch_speed;
	bool look_at_target;
	cs_target_specification look_target;
	bool face_exactly;
	bool face_lower_weapon;
	bool aim_at_target;
	bool face_at_target;
	cs_target_specification aim_target;
	bool vehicle_speed_valid;
	real vehicle_speed;
	bool turn_urgency_valid;
	real turn_urgency;
	bool boosting;
	bool ignore_obstacles;
	bool failsafe_pathfinding;
	bool enable_targeting;
	bool enable_looking;
	bool enable_moving;
	bool enable_dialogue;
	bool suppress_activity_termination;
	bool suppress_global_dialogue;
	bool style_index_valid;
	short forced_combat_status;
	long style_index;
	long action_animation_impulse;
	long animation_mode;
	bool grenade_throw_depress_trigger;
	bool grenade_throw_started;
	short grenade_throw_trajectory_type;
	c_ai_point3d grenade_target;
	bool approach_pending;
	bool approach_within_range;
	long approach_object_index;
	real approach_distance_sq;
	real approach_max_distance_sq;
	real approach_follow_distance_sq;
};
static_assert(sizeof(command_script_datum) == 0x188);

struct cs_script_data;
struct cs_point_set;

extern void __cdecl cs_dispose();
extern void __cdecl cs_dispose_from_old_map();
extern cs_point_set* __cdecl cs_get_point_set(long index);
extern void __cdecl cs_initialize();
extern void __cdecl cs_initialize_for_new_map();
extern void __cdecl cs_initialize_for_new_structure_bsp(long activating_structure_bsp_mask);
extern cs_script_data* __cdecl cs_scenario_get_script_data(struct scenario* scenario);

