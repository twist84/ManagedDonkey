#pragma once

#include "ai/actor_moving.hpp"
#include "ai/actor_moving_hints.hpp"
#include "ai/ai_actions.hpp"
#include "ai/joint_behavior.hpp"
#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "shell/shell.hpp"

struct actor_meta_data
{
	c_enum<e_actor_type, short, _actor_elite, k_number_of_actor_types> type;
	bool general_update;
	bool swarm;
	bool frozen;
	bool active;
	bool squadless;
	bool force_active;
	bool disposable;
	bool erase_next_tick;
	bool allowed_to_refresh_status;
	real original_vitality;
	long unit_index;
	long swarm_index;
	long next_actor_index;
	short team_index;
	bool squad_limbo;
	long prevehicle_squad_index;
	short squad_adoption_timeout;
	short squad_adoption_attempts;
	short outside_map_ticks;
	short equipment_use_flags;
	long squad_index;
	short cell_index;
	short spawn_point_index;
	short spawn_formation_index;
	bool pathfinding_timeslice;
	bool perception_timeslice;
	short pathfinding_service_timer;
	short perception_service_timer;
	long spawn_time;
	long first_active_time;
	long last_active_time;
	long last_in_pvs_time;
	long inactive_erase_time;
	short highest_prop_status_timer;
	short highest_prop_state_timer;
	long character_definition_index;
	long character_equipment_definition_index;
	long first_prop_ref_index;
	real min_trackable_weight;
	long clump_index;
	long clump_next_actor_index;
};
static_assert(sizeof(actor_meta_data) == 0x6C);

struct behavior_stimulus
{
	long type;
	short ticks;
	short desire;
};
static_assert(sizeof(behavior_stimulus) == 0x8);

struct s_actor_patrol_state
{
	short __unknown0;
	short __unknown2;
	short __unknown4;
	word_flags flags;
};
static_assert(sizeof(s_actor_patrol_state) == 0x8);

struct actor_state_data
{
	byte behavior_state_data[512];
	short behavior_state_offsets[8];
	short leaf_layer;
	byte __data212[0x2];
	invitation_data const invitation[4];
	long const behavior_check_timer[14];
	short mode;
	short combat_status;
	long combat_status_timer;
	behavior_stimulus behavior_stimulus[5];
	long stimulus_flags;
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
	char covered;
	bool desired_berserk;
	bool desired_stowed;
	char not_sleeping_ticks;
	short berserk_ticks;
	short covered_ticks;
	s_actor_patrol_state patrol_state;
	word_flags engineer_state_flags;
	short isolated_from_squad_ticks;
};
static_assert(sizeof(actor_state_data) == 0x2E4);

struct actor_position_data
{
	real_point3d head_position;
	real_point3d body_position;
	real_vector3d body_facing;
	s_location body_location;
	real_vector3d velocity;
};
static_assert(sizeof(actor_position_data) == 0x34);

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
	long root_vehicle_index;
	short vehicle_driver_type;
	long gunning_unit_index;
	bool movement_disallowed;
	bool pathfinding_location_valid;
	c_ai_point3d pathfinding_point;
	c_sector_ref sector_ref;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
	real_vector3d looking_left_vector;
	real_vector3d looking_up_vector;
	real scariness;
	real body_vitality;
	real shield_vitality;
	real recent_body_damage;
	real recent_shield_damage;
	bool player_weighting_valid;
	real player_weighting;
};
static_assert(sizeof(actor_input_data) == 0xB8);

struct c_recent_obstacle
{
	long m_object_index;
	short m_ticks;
	word_flags m_flags;
};
static_assert(sizeof(c_recent_obstacle) == 0x8);

struct actor_obstacle_data
{
	c_recent_obstacle recent_obstacles[3];
	byte interaction_target[8];
};
static_assert(sizeof(actor_obstacle_data) == 0x20);

struct actor_memory_data
{
	long recent_dead_peer_time;
	short ticks_since_enemy_engaged;
	long last_damage_vocalization_time;
	short most_recent_interest;
	byte recent_interest_objects[4][8];
	long current_interest_object_index;
	real current_interest_object_interest;
	long postcombat_flags;
	c_sector_ref last_valid_valid_sector;
	c_ai_point3d last_valid_pathfinding_point;
	bool pathfinding_currently_invalid;
	short chorus_vocalization_index;
	short chorus_vocalization_delay_ticks;
	char chorus_pitch_range_index;
	char chorus_permutation_index;
	bool armored;
	short protecting;
};
static_assert(sizeof(actor_memory_data) == 0x5C);

struct actor_morph_data
{
	long last_morph_time;
	short morph_delay_ticks;
	short ticks_getting_damaged_outside_range;
	short ticks_since_getting_damaged_outside_range;
	short default_form;
	bool currently_in_default_form;
	bool pending_form_is_default;
	bool pending_form_propogate;
	short pending_form;
	short pending_form_priority;
	short turtle_delay_ticks;
};
static_assert(sizeof(actor_morph_data) == 0x18);

struct actor_situation
{
	char known_enemies;
	short highest_prop_class;
	long highest_prop_class_prop_index;
	bool close_enemy;
};
static_assert(sizeof(actor_situation) == 0xC);

struct actor_target_data
{
	word_flags flags;
	short preferred_target_team_index;
	long preferred_ai_index;
	long target_prop_index;
	long target_marker;
	long retreat_target_prop_index;
	long target_occluding_object_index;
	long since_any_target_visible_timer;
	short target_group;
	bool target_group_include_player;
};
static_assert(sizeof(actor_target_data) == 0x20);

struct actor_activity_data
{
	short activity_index;
	short variant_index;
	short point_set_index;
	bool starting_location_activity;
	long __unknown8;
};
static_assert(sizeof(actor_activity_data) == 0xC);

struct actor_danger_zone
{
	short danger_type;
	short hostility;
	bool allow_dive_evasion;
	bool allow_brace_evasion;
	bool communicated;
	bool attached_to_us;
	long object_index;
	long owner_unit_index;
	long pref_index;
	real danger_radius;
	real_point3d position;
	real_vector3d velocity;
	real_point3d predict_danger_position;
	real current_distance_from_actor;
	real current_distance_from_center;
	real bounding_sphere_radius;
	real_point3d bounding_sphere_center;
	byte union_storage[2];
};
static_assert(sizeof(actor_danger_zone) == 0x58);

struct actor_stimulus_data
{
	short surprise_level;
	long surprise_prop_index;
	bool stunned;
	real stun_level;
	real_vector3d stunned_vector;
	long cover_friend_actor_index;
	short cover_friend_behavior_index;
	short cover_friend_dialogue_type;
	long tracking_projectile_start_time;
	long tracking_projectile_last_time;
};
static_assert(sizeof(actor_stimulus_data) == 0x2C);

struct actor_vehicle_data
{
	long attached_vehicle_index;
	long attached_seat_unit_index;
	real attached_seat_score;
	short flags;
	short attached_seat_index;
	short attachment_status;
	short attachment_pending_ticks;
	short attachment_locked_ticks;
	short attachment_failure_ticks;
	short attachment_recent_ticks;
	long recent_vehicle_index;
	short recent_seat_index;
	short recent_vehicle_exit_ticks;
	short recent_vehicle_disallow_ticks;
	short disallow_vehicle_exit_ticks;
	long vehicle_turtling_time;
};
static_assert(sizeof(actor_vehicle_data) == 0x2C);

struct actor_player_data
{
	bool crouch_blocking_player_line_of_fire;
	bool moving_into_player_line_of_fire;
	short crouch_timer;
	short moving_into_player_line_of_fire_timer;
	bool player_looking_at_me;
	bool greeting_stare;
	short player_looking_ticks;
	short player_looking_index;
	real in_front_distance;
	short player_index_interested;
	short player_interested_ticks;
	short player_uninterested_ticks;
};
static_assert(sizeof(actor_player_data) == 0x18);

struct actor_emotion_data
{
	real instantaneous_danger;
	real perceived_danger;
	bool ignorant_of_broken_surfaces;
	long last_vehicle_charge_time;
};
static_assert(sizeof(actor_player_data) == 0x18);

struct actor_discarded_firing_position
{
	bool temporary;
	bool pad;
	firing_position_ref index;
};
static_assert(sizeof(actor_discarded_firing_position) == 0x6);

struct actor_firing_position_data
{
	bool current_position_found_outside_range;
	bool search_areas_available;
	bool current_position_is_goal;
	short goal_status;
	long dynamic_firing_set_index;
	long dynamic_firing_set_support_object_index;
	short dynamic_firing_set_support_object_inaccessible_ticks;
	short current_discarded_firing_positions_entry;
	actor_discarded_firing_position discarded_firing_positions[4];
	firing_position_ref current_position_index;
	c_ai_point3d proxy_point;
	bool proxy_valid;
};
static_assert(sizeof(actor_firing_position_data) == 0x44);

struct actor_look_orders
{
	short aim_type;
	c_ai_direction aim_direction;
	short look_type;
	c_ai_direction look_direction;
	bool look_force_weapon_down;
};
static_assert(sizeof(actor_look_orders) == 0x34);

struct actor_move_orders
{
	bool dive_into_cover;
	bool move_face_exactly;
	bool override_movement_direction;
	bool override_throttle;
	long override_movement_type;
	real_vector3d override_movement_direction_vector;
	real override_throttle_value;
	bool jump;
	bool jump_leap;
	bool jump_boosted;
	bool jump_targeted;
	real_vector2d jump_alignment_vector;
	real jump_target_horizontal_vel;
	real jump_target_vertical_vel;
	long jump_target_pref_index;
	real scripted_turn_urgency;
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
	real throttle_vehicle_desired;
};
static_assert(sizeof(actor_move_orders) == 0x48);

struct actor_combat_orders
{
	bool shoot_at_target;
	bool bombard_target;
	bool abort_burst;
	bool override_firing_restrictions;
	long target_type;
	union
	{
		c_ai_point3d target_point;
		long prop_index;
	};
	bool disregard_visibility;
	bool use_secondary_trigger;
	bool lower_weapon;
	bool raise_weapon;
};
static_assert(sizeof(actor_combat_orders) == 0x1C);

struct actor_swarm_orders
{
	char action;
	char target_type;
	long target_index;
};

struct actor_orders
{
	actor_look_orders look;
	actor_move_orders move;
	actor_combat_orders combat;
	actor_swarm_orders swarm;
};
static_assert(sizeof(actor_orders) == 0xA0);

struct actor_action_data
{
	c_ai_action ai_action;
};
static_assert(sizeof(actor_action_data) == 0x30);

struct c_path_destination
{
	c_ai_point3d m_point;
	real_vector3d m_alignment;
	c_sector_ref m_sector_ref;
	real m_target_radius;
};
static_assert(sizeof(c_path_destination) == 0x24);

struct path_step
{
	short type;
	short flags;
	c_sector_ref sector_ref;
	c_hint_ref hint_ref;
	c_ai_point3d point;
};
static_assert(sizeof(path_step) == 0x1C);

struct path_step_source
{
	short type;
	short structure_index;
	union
	{
		long sector_link_index;
		long hint_index;
	};
};
static_assert(sizeof(path_step_source) == 0x8);

struct path_error_info
{
	short error_stage;
	short num_thresholds;
	path_step_source failed_threshold[3];
};
static_assert(sizeof(path_error_info) == 0x1C);

struct path_result
{
	bool valid;
	bool steps_finish_path;
	bool obstacle_on_goal;
	c_ai_point3d start_point;
	c_path_destination endpoint;
	long ignorable_object_index;
	word_flags ignorable_object_flags;
	char step_count;
	char step_index;
	path_step steps[4];
	path_error_info error_info;
};
static_assert(sizeof(path_result) == 0xCC);

struct actor_path_control_data
{
	c_destination_orders destination_orders;
	short status;
	bool refreshed_this_tick;
	short failure_count;
	path_result path;
};
static_assert(sizeof(actor_path_control_data) == 0x13C);

struct turn_info
{
	bool left;
	bool rev;
	real angle;
	real rating;
	real_vector2d expected_facing;
	real world_orientation;
	short turn_index;
	real current_throttle;
	real last_throttle;
	real last_yaw_angle;
	real last_urgency;
	real current_yaw_angle;
	bool valid;
};
static_assert(sizeof(turn_info) == 0x34);

struct flying_turn_info
{
	bool servo_to_target;
	bool finished_turn;
	short movement_mode;
	real last_yaw_angle;
	real current_yaw_angle;
	real last_pitch_angle;
	real current_pitch_angle;
	real urgency;
	real_vector3d current_throttle;
	real_vector3d last_throttle;
};
static_assert(sizeof(flying_turn_info) == 0x30);

struct hovering_turn_info
{
	real last_yaw_angle;
	real last_throttle_scale;
	real current_yaw_angle;
	real current_throttle_scale;
	short last_mode;
};
static_assert(sizeof(hovering_turn_info) == 0x14);

struct actor_control_data
{
	dword_flags flags;
	actor_path_control_data path;
	real_point3d stuck_point;
	short stuck_ticks;
	short isolation_count;
	real_vector3d moving_towards_vector;
	real_vector3d moving_forced_aim_direction;
	real_plane3d persistent_movement_plane;
	real persistent_movement_distance;
	short persistent_movement_ticks;

	// odst?
	bool __unknown17E; // actor_move_update

	short freeze_ticks;
	short suppress_shooting_ticks;
	real_vector2d jump_alignment_vector;
	real jump_target_horizontal_vel;
	real jump_target_vertical_vel;
	long jump_target_pref_index;
	long last_deceleration_to_zero_time;
	long deceleration_object_index;
	short deceleration_ticks;

	// odst?
	byte __unknown1A2; // actor_ground_throttle_control
	byte __unknown1A3; // actor_ground_throttle_control

	union
	{
		turn_info turn;
		flying_turn_info flying_turn;
		hovering_turn_info hovering_turn;
	};
	short glance_priority;
	short glance_timer;
	c_ai_direction glance_direction;
	long idle_major_change_time;
	long idle_major_timer;
	long idle_minor_timer;
	c_ai_direction idle_major_direction;
	c_ai_direction idle_minor_direction;
	long pose_timer;
	real_vector3d desired_facing_vector;
	real_vector3d desired_aiming_vector;
	real_vector3d desired_looking_vector;
	short zig_zag_ticks;
	short emotion_ticks;
	short weapon_flags;
	short fire_state;
	short fire_state_timer;
	short burst_disable_timer;
	short trigger_delay_timer;
	short blocked_timer;
	short special_fire_delay;
	short special_fire_deny_attempts;
	real accuracy;
	real burst_distance;
	long last_burst_start_time;
	short blocked_projectiles_count;
	short current_fire_target_type;
	union
	{
		c_ai_point3d current_fire_target_manual_point;
		long current_fire_target_prop_index;
	};
	long current_fire_target_timer;
	short current_fire_target_line_of_sight;
	short player_blocking_ticks;
	long last_player_blocking_time;
	c_ai_point3d current_fire_target_position;
	real current_fire_target_range;
	real_vector3d current_fire_target_aim_vector;
	real current_fire_target_distance;
	c_ai_point3d burst_initial_position;
	real_point3d burst_origin;
	real_vector3d burst_relative_position;
	real_vector3d burst_adjustment;
	real_point3d burst_target;
	real burst_aim_by_vector_velocity;
	real_vector3d burst_aim_vector;
	real burst_error;
	real burst_damage_modifier;
	short grenade_request_ticks;
	short grenade_flags;
	long grenade_last_throw_time;
	c_ai_point3d grenade_current_target;
	long grenade_current_prop_index;
	long grenade_current_ignore_object_index;
	real_vector3d grenade_current_aim_vector;
	real grenade_current_aim_speed;
	c_ai_point3d equipment_target;
	long last_hop_time;
	c_ai_point3d phase_destination;
	bool anchor_valid;
	real_point3d anchor_point;
};
static_assert(sizeof(actor_control_data) == 0x36C);

struct actor_animation_orders
{
	long impulse;
	real_vector2d alignment_vector;
};
static_assert(sizeof(actor_animation_orders) == 0xC);

struct actor_output_data
{
	long movement_type;
	actor_animation_orders animation;
	short aiming_speed;
	dword control_flags;
	real_vector3d throttle;
	real analog_primary_trigger;
	real analog_secondary_trigger;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
	real_point3d gaze_position;
};
static_assert(sizeof(actor_output_data) == 0x5C);

struct actor_script_data
{
	long first_command_script_index;
	long active_list;
	short cl_layer;
};
static_assert(sizeof(actor_script_data) == 0xC);

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
static_assert(sizeof(actor_datum) == 0xA98);
static_assert(0x004 == OFFSETOF(actor_datum, meta));
static_assert(0x070 == OFFSETOF(actor_datum, state));
static_assert(0x354 == OFFSETOF(actor_datum, input));
static_assert(0x40C == OFFSETOF(actor_datum, obstacles));
static_assert(0x42C == OFFSETOF(actor_datum, memory));
static_assert(0x488 == OFFSETOF(actor_datum, morph));
static_assert(0x4A0 == OFFSETOF(actor_datum, situation));
static_assert(0x4AC == OFFSETOF(actor_datum, target));
static_assert(0x4CC == OFFSETOF(actor_datum, danger_zone));
static_assert(0x524 == OFFSETOF(actor_datum, stimuli));
static_assert(0x550 == OFFSETOF(actor_datum, activity));
static_assert(0x55C == OFFSETOF(actor_datum, vehicle));
static_assert(0x588 == OFFSETOF(actor_datum, player));
static_assert(0x5A0 == OFFSETOF(actor_datum, emotions));
static_assert(0x5B0 == OFFSETOF(actor_datum, firing_positions));
static_assert(0x5F4 == OFFSETOF(actor_datum, orders));
static_assert(0x694 == OFFSETOF(actor_datum, actions));
static_assert(0x6C4 == OFFSETOF(actor_datum, control));
static_assert(0xA30 == OFFSETOF(actor_datum, output));
static_assert(0xA8C == OFFSETOF(actor_datum, commands));

struct ai_reference_frame
{
	long object_index;
	short runtime_flags;
	short pad0;
};
static_assert(sizeof(ai_reference_frame) == 0x8);

struct actor_iterator
{
	c_data_iterator<actor_datum> iterator;
	bool active_only;
	long index;
};
static_assert(sizeof(actor_iterator) == 0x18);

extern bool __cdecl actor_datum_available_to_current_thread();
extern void __cdecl actor_delete(long actor_index, bool a2);
extern void __cdecl actor_erase(long actor_index, bool delete_immediately);
extern actor_datum* __cdecl actor_get(long actor_index);
extern bool __cdecl actor_is_active(actor_datum const* actor);
extern bool __cdecl actor_is_blind(long actor_index);
extern bool __cdecl actor_is_deaf(long actor_index);
extern void __cdecl actor_iterator_new(actor_iterator* iterator, bool a2);
extern actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator);
extern void __cdecl actors_dispose();
extern void __cdecl actors_dispose_from_old_map();
extern void __cdecl actors_freeze();
extern void __cdecl actors_initialize();
extern void __cdecl actors_initialize_for_new_map();
extern void __cdecl actors_move_randomly();
extern void __cdecl actors_update();

