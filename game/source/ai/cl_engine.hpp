#pragma once

#include "ai/actors.hpp"
#include "cseries/cseries.hpp"

// $TODO confirm the following enum

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
	int8 atom_type;
	int8 flags;
	int16 atom_modifier;

	union
	{
		int32 integer_parameter1;
		real32 parameter1;
		bool bool_param1;
	};

	union
	{
		int32 integer_parameter2;
		real32 parameter2;
		bool bool_param2;
	};

	real_point3d point1;
	real_point3d point2;
	int32 ref_index;
	int32 ref_index2;
};
static_assert(sizeof(cs_command) == 0x2C);

struct s_control_directmovement
{
	int16 facing;
	real_vector3d vector;
	real_point3d start_position;
};
static_assert(sizeof(s_control_directmovement) == 0x1C);

struct s_control_point_destination
{
	c_ai_point3d point;
	real32 radius;
};
static_assert(sizeof(s_control_point_destination) == 0x14);

struct s_control_move_towards
{
	int32 object_index;
};
static_assert(sizeof(s_control_move_towards) == 0x4);

struct s_control_jump
{
	int16 delay_ticks;
	real32 target_horizontal_vel;
	real32 target_vertical_vel;
};
static_assert(sizeof(s_control_jump) == 0xC);

struct s_atom_control
{
	int16 atom_type;
	int16 pause_timer;
	int16 status;
	int16 simple_control_flags;

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
	int16 type;
	int32 index;
};
static_assert(sizeof(cs_target_specification) == 0x8);

struct command_script_datum :
	s_datum_header
{
	int16 type;
	int32 actor_index;
	int32 script_index;
	int32 thread_index;
	int32 next_cs;
	bool reserved;
	bool abort;
	bool reserve_mandatory;
	bool abort_on_alert;
	bool abort_on_damage;
	bool abort_on_combat_status;
	int16 abort_on_combat_status_level;
	bool abort_on_vehicle_exit;
	int16 role_index;
	int16 priority;
	cs_command command[3];
	s_atom_control control[3];
	bool shoot_at_target;
	bool shoot_secondary_trigger;
	bool script_initiated_posture;
	bool script_initiated_custom_animation;
	bool walk;
	bool crouch;
	real32 crouch_speed;
	bool look_at_target;
	cs_target_specification look_target;
	bool face_exactly;
	bool face_lower_weapon;
	bool aim_at_target;
	bool face_at_target;
	cs_target_specification aim_target;
	bool vehicle_speed_valid;
	real32 vehicle_speed;
	bool turn_urgency_valid;
	real32 turn_urgency;
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
	int16 forced_combat_status;
	int32 style_index;
	int32 action_animation_impulse;
	int32 animation_mode;
	bool grenade_throw_depress_trigger;
	bool grenade_throw_started;
	int16 grenade_throw_trajectory_type;
	c_ai_point3d grenade_target;
	bool approach_pending;
	bool approach_within_range;
	int32 approach_object_index;
	real32 approach_distance_sq;
	real32 approach_max_distance_sq;
	real32 approach_follow_distance_sq;
};
static_assert(sizeof(command_script_datum) == 0x188);

struct cs_script_data;
struct cs_point_set;

extern bool __cdecl cs_blocked(int32 thread_index);
extern void __cdecl cs_dispose();
extern void __cdecl cs_dispose_from_old_map();
extern cs_point_set* __cdecl cs_get_point_set(int32 index);
extern void __cdecl cs_global_script_context_clear();
extern void __cdecl cs_handle_thread_delete(int32 deleted_thread_index);
extern void __cdecl cs_initialize();
extern void __cdecl cs_initialize_for_new_map();
extern void __cdecl cs_initialize_for_new_structure_bsp(int32 activating_structure_bsp_mask);
extern cs_script_data* __cdecl cs_scenario_get_script_data(struct scenario* scenario);
extern void __cdecl cs_setup_global_script_context(int32 thread_index);

