#pragma once

#include "ai/actor_moving.hpp"
#include "ai/actor_moving_hints.hpp"
#include "ai/ai_actions.hpp"
#include "ai/joint_behavior.hpp"
#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct actor_meta_data
{
	c_enum<e_actor_type, int16, _actor_elite, k_number_of_actor_types> type;
	bool general_update;
	bool swarm;
	bool frozen;
	bool active;
	bool squadless;
	bool force_active;
	bool disposable;
	bool erase_next_tick;
	bool allowed_to_refresh_status;
	real32 original_vitality;
	int32 unit_index;
	int32 swarm_index;
	int32 next_actor_index;
	int16 team_index;
	bool squad_limbo;
	int32 prevehicle_squad_index;
	int16 squad_adoption_timeout;
	int16 squad_adoption_attempts;
	int16 outside_map_ticks;
	int16 equipment_use_flags;
	int32 squad_index;
	int16 cell_index;
	int16 spawn_point_index;
	int16 spawn_formation_index;
	bool pathfinding_timeslice;
	bool perception_timeslice;
	int16 pathfinding_service_timer;
	int16 perception_service_timer;
	int32 spawn_time;
	int32 first_active_time;
	int32 last_active_time;
	int32 last_in_pvs_time;
	int32 inactive_erase_time;
	int16 highest_prop_status_timer;
	int16 highest_prop_state_timer;
	int32 character_definition_index;
	int32 character_equipment_definition_index;
	int32 first_prop_ref_index;
	real32 min_trackable_weight;
	int32 clump_index;
	int32 clump_next_actor_index;
};
COMPILE_ASSERT(sizeof(actor_meta_data) == 0x6C);

struct behavior_stimulus
{
	int32 type;
	int16 ticks;
	int16 desire;
};
COMPILE_ASSERT(sizeof(behavior_stimulus) == 0x8);

struct s_actor_patrol_state
{
	int16 __unknown0;
	int16 __unknown2;
	int16 __unknown4;
	uns16 flags;
};
COMPILE_ASSERT(sizeof(s_actor_patrol_state) == 0x8);

struct actor_state_data
{
	byte behavior_state_data[512];
	int16 behavior_state_offsets[8];
	int16 leaf_layer;
	byte __data212[0x2];
	invitation_data const invitation[4];
	int32 const behavior_check_timer[14];
	int16 mode;
	int16 combat_status;
	int32 combat_status_timer;
	behavior_stimulus behavior_stimulus[5];
	int32 stimulus_flags;
	bool group_searching;
	bool disallow_retreating;
	bool arrived;
	bool blind;
	bool deaf;
	bool flying;
	bool carrying_turret;
	bool had_visible_enemy;
	bool stowed;
	bool force_weapon_up;
	bool dont_do_avoidance;
	bool searching;
	bool suppress_combat;
	bool broken;
	bool berserk;
	int8 covered;
	bool desired_berserk;
	bool desired_stowed;
	int8 not_sleeping_ticks;
	int16 berserk_ticks;
	int16 covered_ticks;
	s_actor_patrol_state patrol_state;
	uns16 engineer_state_flags;
	int16 isolated_from_squad_ticks;
};
COMPILE_ASSERT(sizeof(actor_state_data) == 0x2E4);

struct actor_position_data
{
	real_point3d head_position;
	real_point3d body_position;
	real_vector3d body_facing;
	s_location body_location;
	real_vector3d velocity;
};
COMPILE_ASSERT(sizeof(actor_position_data) == 0x34);

struct actor_input_data
{
	actor_position_data position;
	bool in_midair;
	bool underwater;
	bool climbing;
	bool driver;
	bool passenger;
	bool gunner;
	bool boarding;
	bool carrying_player;
	int32 root_vehicle_index;
	int16 vehicle_driver_type;
	int32 gunning_unit_index;
	bool movement_disallowed;
	bool pathfinding_location_valid;
	c_ai_point3d pathfinding_point;
	c_sector_ref sector_ref;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
	real_vector3d looking_left_vector;
	real_vector3d looking_up_vector;
	real32 scariness;
	real32 body_vitality;
	real32 shield_vitality;
	real32 recent_body_damage;
	real32 recent_shield_damage;
	bool player_weighting_valid;
	real32 player_weighting;
};
COMPILE_ASSERT(sizeof(actor_input_data) == 0xB8);

class c_recent_obstacle
{
public:
	int32 m_object_index;
	int16 m_ticks;
	uns16 m_flags;
};
COMPILE_ASSERT(sizeof(c_recent_obstacle) == 0x8);

struct actor_obstacle_data
{
	c_recent_obstacle recent_obstacles[3];
	uns8 interaction_target[8];
};
COMPILE_ASSERT(sizeof(actor_obstacle_data) == 0x20);

struct actor_memory_data
{
	int32 recent_dead_peer_time;
	int16 ticks_since_enemy_engaged;
	int32 last_damage_vocalization_time;
	int16 most_recent_interest;
	uns8 recent_interest_objects[4][8];
	int32 current_interest_object_index;
	real32 current_interest_object_interest;
	int32 postcombat_flags;
	c_sector_ref last_valid_valid_sector;
	c_ai_point3d last_valid_pathfinding_point;
	bool pathfinding_currently_invalid;
	int16 chorus_vocalization_index;
	int16 chorus_vocalization_delay_ticks;
	int8 chorus_pitch_range_index;
	int8 chorus_permutation_index;
	bool armored;
	int16 protecting;
};
COMPILE_ASSERT(sizeof(actor_memory_data) == 0x5C);

struct actor_morph_data
{
	int32 last_morph_time;
	int16 morph_delay_ticks;
	int16 ticks_getting_damaged_outside_range;
	int16 ticks_since_getting_damaged_outside_range;
	int16 default_form;
	bool currently_in_default_form;
	bool pending_form_is_default;
	bool pending_form_propogate;
	int16 pending_form;
	int16 pending_form_priority;
	int16 turtle_delay_ticks;
};
COMPILE_ASSERT(sizeof(actor_morph_data) == 0x18);

struct actor_situation
{
	int8 known_enemies;
	int16 highest_prop_class;
	int32 highest_prop_class_prop_index;
	bool close_enemy;
};
COMPILE_ASSERT(sizeof(actor_situation) == 0xC);

struct actor_target_data
{
	uns16 flags;
	int16 preferred_target_team_index;
	int32 preferred_ai_index;
	int32 target_prop_index;
	int32 target_marker;
	int32 retreat_target_prop_index;
	int32 target_occluding_object_index;
	int32 since_any_target_visible_timer;
	int16 target_group;
	bool target_group_include_player;
};
COMPILE_ASSERT(sizeof(actor_target_data) == 0x20);

struct actor_activity_data
{
	int16 activity_index;
	int16 variant_index;
	int16 point_set_index;
	bool starting_location_activity;
	int32 __unknown8;
};
COMPILE_ASSERT(sizeof(actor_activity_data) == 0xC);

struct actor_danger_zone
{
	int16 danger_type;
	int16 hostility;
	bool allow_dive_evasion;
	bool allow_brace_evasion;
	bool communicated;
	bool attached_to_us;
	int32 object_index;
	int32 owner_unit_index;
	int32 pref_index;
	real32 danger_radius;
	real_point3d position;
	real_vector3d velocity;
	real_point3d predict_danger_position;
	real32 current_distance_from_actor;
	real32 current_distance_from_center;
	real32 bounding_sphere_radius;
	real_point3d bounding_sphere_center;
	byte union_storage[2];
};
COMPILE_ASSERT(sizeof(actor_danger_zone) == 0x58);

struct actor_stimulus_data
{
	int16 surprise_level;
	int32 surprise_prop_index;
	bool stunned;
	real32 stun_level;
	real_vector3d stunned_vector;
	int32 cover_friend_actor_index;
	int16 cover_friend_behavior_index;
	int16 cover_friend_dialogue_type;
	int32 tracking_projectile_start_time;
	int32 tracking_projectile_last_time;
};
COMPILE_ASSERT(sizeof(actor_stimulus_data) == 0x2C);

struct actor_vehicle_data
{
	int32 attached_vehicle_index;
	int32 attached_seat_unit_index;
	real32 attached_seat_score;
	int16 flags;
	int16 attached_seat_index;
	int16 attachment_status;
	int16 attachment_pending_ticks;
	int16 attachment_locked_ticks;
	int16 attachment_failure_ticks;
	int16 attachment_recent_ticks;
	int32 recent_vehicle_index;
	int16 recent_seat_index;
	int16 recent_vehicle_exit_ticks;
	int16 recent_vehicle_disallow_ticks;
	int16 disallow_vehicle_exit_ticks;
	int32 vehicle_turtling_time;
};
COMPILE_ASSERT(sizeof(actor_vehicle_data) == 0x2C);

struct actor_player_data
{
	bool crouch_blocking_player_line_of_fire;
	bool moving_into_player_line_of_fire;
	int16 crouch_timer;
	int16 moving_into_player_line_of_fire_timer;
	bool player_looking_at_me;
	bool greeting_stare;
	int16 player_looking_ticks;
	int16 player_looking_index;
	real32 in_front_distance;
	int16 player_index_interested;
	int16 player_interested_ticks;
	int16 player_uninterested_ticks;
};
COMPILE_ASSERT(sizeof(actor_player_data) == 0x18);

struct actor_emotion_data
{
	real32 instantaneous_danger;
	real32 perceived_danger;
	bool ignorant_of_broken_surfaces;
	int32 last_vehicle_charge_time;
};
COMPILE_ASSERT(sizeof(actor_player_data) == 0x18);

struct actor_discarded_firing_position
{
	bool temporary;
	bool pad;
	firing_position_ref index;
};
COMPILE_ASSERT(sizeof(actor_discarded_firing_position) == 0x6);

struct actor_firing_position_data
{
	bool current_position_found_outside_range;
	bool search_areas_available;
	bool current_position_is_goal;
	int16 goal_status;
	int32 dynamic_firing_set_index;
	int32 dynamic_firing_set_support_object_index;
	int16 dynamic_firing_set_support_object_inaccessible_ticks;
	int16 current_discarded_firing_positions_entry;
	actor_discarded_firing_position discarded_firing_positions[4];
	firing_position_ref current_position_index;
	c_ai_point3d proxy_point;
	bool proxy_valid;
};
COMPILE_ASSERT(sizeof(actor_firing_position_data) == 0x44);

struct actor_look_orders
{
	int16 aim_type;
	c_ai_direction aim_direction;
	int16 look_type;
	c_ai_direction look_direction;
	bool look_force_weapon_down;
};
COMPILE_ASSERT(sizeof(actor_look_orders) == 0x34);

struct actor_move_orders
{
	bool dive_into_cover;
	bool move_face_exactly;
	bool override_movement_direction;
	bool override_throttle;
	int32 override_movement_type;
	real_vector3d override_movement_direction_vector;
	real32 override_throttle_value;
	bool jump;
	bool jump_leap;
	bool jump_boosted;
	bool jump_targeted;
	real_vector2d jump_alignment_vector;
	real32 jump_target_horizontal_vel;
	real32 jump_target_vertical_vel;
	int32 jump_target_pref_index;
	real32 scripted_turn_urgency;
	bool ignore_obstacles;
	bool scripted_disallow_movement;
	bool zig_zag;
	bool smart_boost;

	// odst?
	bool __unknown38;
	bool __unknown39;
	bool __unknown3A; // actor_move_calculate_nondirectional_flying_movement
	bool __unknown3B; // actor_move_calculate_nondirectional_flying_movement

	bool walk;
	bool allow_poses;
	bool allow_moving_poses;
	bool allow_path_jump;
	bool stationary_crouch;
	bool moving_crouch;
	bool disallow_combat_crouch;
	bool throttle_vehicle_speed;
	real32 throttle_vehicle_desired;
};
COMPILE_ASSERT(sizeof(actor_move_orders) == 0x48);

struct actor_combat_orders
{
	bool shoot_at_target;
	bool bombard_target;
	bool abort_burst;
	bool override_firing_restrictions;
	int32 target_type;
	union
	{
		c_ai_point3d target_point;
		int32 prop_index;
	};
	bool disregard_visibility;
	bool use_secondary_trigger;
	bool lower_weapon;
	bool raise_weapon;
};
COMPILE_ASSERT(sizeof(actor_combat_orders) == 0x1C);

struct actor_swarm_orders
{
	int8 action;
	int8 target_type;
	int32 target_index;
};

struct actor_orders
{
	actor_look_orders look;
	actor_move_orders move;
	actor_combat_orders combat;
	actor_swarm_orders swarm;
};
COMPILE_ASSERT(sizeof(actor_orders) == 0xA0);

struct actor_action_data
{
	c_ai_action ai_action;
};
COMPILE_ASSERT(sizeof(actor_action_data) == 0x30);

class c_path_destination
{
public:
	c_ai_point3d m_point;
	real_vector3d m_alignment;
	c_sector_ref m_sector_ref;
	real32 m_target_radius;
};
COMPILE_ASSERT(sizeof(c_path_destination) == 0x24);

struct path_step
{
	int16 type;
	int16 flags;
	c_sector_ref sector_ref;
	c_hint_ref hint_ref;
	c_ai_point3d point;
};
COMPILE_ASSERT(sizeof(path_step) == 0x1C);

struct path_step_source
{
	int16 type;
	int16 structure_index;
	union
	{
		int32 sector_link_index;
		int32 hint_index;
	};
};
COMPILE_ASSERT(sizeof(path_step_source) == 0x8);

struct path_error_info
{
	int16 error_stage;
	int16 num_thresholds;
	path_step_source failed_threshold[3];
};
COMPILE_ASSERT(sizeof(path_error_info) == 0x1C);

struct path_result
{
	bool valid;
	bool steps_finish_path;
	bool obstacle_on_goal;
	c_ai_point3d start_point;
	c_path_destination endpoint;
	int32 ignorable_object_index;
	uns16 ignorable_object_flags;
	int8 step_count;
	int8 step_index;
	path_step steps[4];
	path_error_info error_info;
};
COMPILE_ASSERT(sizeof(path_result) == 0xCC);

struct actor_path_control_data
{
	c_destination_orders destination_orders;
	int16 status;
	bool refreshed_this_tick;
	int16 failure_count;
	path_result path;
};
COMPILE_ASSERT(sizeof(actor_path_control_data) == 0x13C);

struct turn_info
{
	bool left;
	bool rev;
	real32 angle;
	real32 rating;
	real_vector2d expected_facing;
	real32 world_orientation;
	int16 turn_index;
	real32 current_throttle;
	real32 last_throttle;
	real32 last_yaw_angle;
	real32 last_urgency;
	real32 current_yaw_angle;
	bool valid;
};
COMPILE_ASSERT(sizeof(turn_info) == 0x34);

struct flying_turn_info
{
	bool servo_to_target;
	bool finished_turn;
	int16 movement_mode;
	real32 last_yaw_angle;
	real32 current_yaw_angle;
	real32 last_pitch_angle;
	real32 current_pitch_angle;
	real32 urgency;
	real_vector3d current_throttle;
	real_vector3d last_throttle;
};
COMPILE_ASSERT(sizeof(flying_turn_info) == 0x30);

struct hovering_turn_info
{
	real32 last_yaw_angle;
	real32 last_throttle_scale;
	real32 current_yaw_angle;
	real32 current_throttle_scale;
	int16 last_mode;
};
COMPILE_ASSERT(sizeof(hovering_turn_info) == 0x14);

struct actor_control_data
{
	uns32 flags;
	actor_path_control_data path;
	real_point3d stuck_point;
	int16 stuck_ticks;
	int16 isolation_count;
	real_vector3d moving_towards_vector;
	real_vector3d moving_forced_aim_direction;
	real_plane3d persistent_movement_plane;
	real32 persistent_movement_distance;
	int16 persistent_movement_ticks;

	// odst?
	bool __unknown17E; // actor_move_update

	int16 freeze_ticks;
	int16 suppress_shooting_ticks;
	real_vector2d jump_alignment_vector;
	real32 jump_target_horizontal_vel;
	real32 jump_target_vertical_vel;
	int32 jump_target_pref_index;
	int32 last_deceleration_to_zero_time;
	int32 deceleration_object_index;
	int16 deceleration_ticks;

	// odst?
	uns8 __unknown1A2; // actor_ground_throttle_control
	uns8 __unknown1A3; // actor_ground_throttle_control

	union
	{
		turn_info turn;
		flying_turn_info flying_turn;
		hovering_turn_info hovering_turn;
	};
	int16 glance_priority;
	int16 glance_timer;
	c_ai_direction glance_direction;
	int32 idle_major_change_time;
	int32 idle_major_timer;
	int32 idle_minor_timer;
	c_ai_direction idle_major_direction;
	c_ai_direction idle_minor_direction;
	int32 pose_timer;
	real_vector3d desired_facing_vector;
	real_vector3d desired_aiming_vector;
	real_vector3d desired_looking_vector;
	int16 zig_zag_ticks;
	int16 emotion_ticks;
	int16 weapon_flags;
	int16 fire_state;
	int16 fire_state_timer;
	int16 burst_disable_timer;
	int16 trigger_delay_timer;
	int16 blocked_timer;
	int16 special_fire_delay;
	int16 special_fire_deny_attempts;
	real32 accuracy;
	real32 burst_distance;
	int32 last_burst_start_time;
	int16 blocked_projectiles_count;
	int16 current_fire_target_type;
	union
	{
		c_ai_point3d current_fire_target_manual_point;
		int32 current_fire_target_prop_index;
	};
	int32 current_fire_target_timer;
	int16 current_fire_target_line_of_sight;
	int16 player_blocking_ticks;
	int32 last_player_blocking_time;
	c_ai_point3d current_fire_target_position;
	real32 current_fire_target_range;
	real_vector3d current_fire_target_aim_vector;
	real32 current_fire_target_distance;
	c_ai_point3d burst_initial_position;
	real_point3d burst_origin;
	real_vector3d burst_relative_position;
	real_vector3d burst_adjustment;
	real_point3d burst_target;
	real32 burst_aim_by_vector_velocity;
	real_vector3d burst_aim_vector;
	real32 burst_error;
	real32 burst_damage_modifier;
	int16 grenade_request_ticks;
	int16 grenade_flags;
	int32 grenade_last_throw_time;
	c_ai_point3d grenade_current_target;
	int32 grenade_current_prop_index;
	int32 grenade_current_ignore_object_index;
	real_vector3d grenade_current_aim_vector;
	real32 grenade_current_aim_speed;
	c_ai_point3d equipment_target;
	int32 last_hop_time;
	c_ai_point3d phase_destination;
	bool anchor_valid;
	real_point3d anchor_point;
};
COMPILE_ASSERT(sizeof(actor_control_data) == 0x36C);

struct actor_animation_orders
{
	int32 impulse;
	real_vector2d alignment_vector;
};
COMPILE_ASSERT(sizeof(actor_animation_orders) == 0xC);

struct actor_output_data
{
	int32 movement_type;
	actor_animation_orders animation;
	int16 aiming_speed;
	uns32 control_flags;
	real_vector3d throttle;
	real32 analog_primary_trigger;
	real32 analog_secondary_trigger;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
	real_point3d gaze_position;
};
COMPILE_ASSERT(sizeof(actor_output_data) == 0x5C);

struct actor_script_data
{
	int32 first_command_script_index;
	int32 active_list;
	int16 cl_layer;
};
COMPILE_ASSERT(sizeof(actor_script_data) == 0xC);

struct actor_datum :
	s_datum_header
{
	actor_meta_data meta;
	actor_state_data state;
	actor_input_data input;
	actor_obstacle_data obstacles;
	actor_memory_data memory;
	actor_morph_data morph;
	actor_situation situation;
	actor_target_data target;
	actor_danger_zone danger_zone;
	actor_stimulus_data stimuli;
	actor_activity_data activity;
	actor_vehicle_data vehicle;
	actor_player_data player;
	actor_emotion_data emotions;
	actor_firing_position_data firing_positions;
	actor_orders orders;
	actor_action_data actions;
	actor_control_data control;
	actor_output_data output;
	actor_script_data commands;
};
COMPILE_ASSERT(sizeof(actor_datum) == 0xA98);
COMPILE_ASSERT(0x004 == OFFSETOF(actor_datum, meta));
COMPILE_ASSERT(0x070 == OFFSETOF(actor_datum, state));
COMPILE_ASSERT(0x354 == OFFSETOF(actor_datum, input));
COMPILE_ASSERT(0x40C == OFFSETOF(actor_datum, obstacles));
COMPILE_ASSERT(0x42C == OFFSETOF(actor_datum, memory));
COMPILE_ASSERT(0x488 == OFFSETOF(actor_datum, morph));
COMPILE_ASSERT(0x4A0 == OFFSETOF(actor_datum, situation));
COMPILE_ASSERT(0x4AC == OFFSETOF(actor_datum, target));
COMPILE_ASSERT(0x4CC == OFFSETOF(actor_datum, danger_zone));
COMPILE_ASSERT(0x524 == OFFSETOF(actor_datum, stimuli));
COMPILE_ASSERT(0x550 == OFFSETOF(actor_datum, activity));
COMPILE_ASSERT(0x55C == OFFSETOF(actor_datum, vehicle));
COMPILE_ASSERT(0x588 == OFFSETOF(actor_datum, player));
COMPILE_ASSERT(0x5A0 == OFFSETOF(actor_datum, emotions));
COMPILE_ASSERT(0x5B0 == OFFSETOF(actor_datum, firing_positions));
COMPILE_ASSERT(0x5F4 == OFFSETOF(actor_datum, orders));
COMPILE_ASSERT(0x694 == OFFSETOF(actor_datum, actions));
COMPILE_ASSERT(0x6C4 == OFFSETOF(actor_datum, control));
COMPILE_ASSERT(0xA30 == OFFSETOF(actor_datum, output));
COMPILE_ASSERT(0xA8C == OFFSETOF(actor_datum, commands));

struct ai_reference_frame
{
	int32 object_index;
	int16 runtime_flags;
	int16 pad0;
};
COMPILE_ASSERT(sizeof(ai_reference_frame) == 0x8);

struct actor_iterator
{
	c_data_iterator<actor_datum> iterator;
	bool active_only;
	int32 index;
};
COMPILE_ASSERT(sizeof(actor_iterator) == 0x18);

extern const real_argb_color* __cdecl actor_activation_debug_color(int32 actor_index);
extern bool __cdecl actor_datum_available_to_current_thread();
extern void __cdecl actor_delete(int32 actor_index, bool died);
extern void __cdecl actor_erase(int32 actor_index, bool delete_immediately);
extern bool __cdecl actor_general_update(int32 actor_index);
extern void __cdecl actor_handle_equipment_delete(int32 actor_index);
extern bool __cdecl actor_is_active(const actor_datum* actor);
extern bool __cdecl actor_is_blind(int32 actor_index);
extern bool __cdecl actor_is_deaf(int32 actor_index);
extern void __cdecl actor_iterator_new(actor_iterator* iterator, bool active_only);
extern actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator);
extern void __cdecl actor_kill(int32 actor_index, bool silent, bool delayed);
extern bool __cdecl actor_set_active(int32 actor_index, bool active);
extern bool __cdecl actor_switch_bsp(int32 actor_index);
extern void __cdecl actors_dispose();
extern void __cdecl actors_dispose_from_old_map();
extern void __cdecl actors_freeze();
extern void __cdecl actors_initialize();
extern void __cdecl actors_initialize_for_new_map();
extern void __cdecl actors_move_randomly();
extern void __cdecl actors_update();

