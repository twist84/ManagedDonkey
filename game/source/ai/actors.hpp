#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct actor_meta_data
{
	short type;

	bool __unknown2;

	bool swarm;

	byte ___data4[0x1];

	bool active;
	bool squadless;

	byte ___data7[0x1];
	bool __unknown8;
	byte ___data9[0x3];

	real original_vitality;

	long unit_index;
	long swarm_index;

	byte __data18[0x4];

	short team;

	byte ___data1E[0x2];

	long prevehicle_squad_index;
	short squad_adoption_timeout;
	short squad_adoption_attempts;
	short outside_map_ticks;
	short equipment_use_flags;
	long squad_index;
	short cell_index;
	short spawn_point_index;
	short spawn_formation_index;
	bool pathfinding_timeslice; // not a bool?

	byte ___data37[0x5];

	long spawn_time;
	long first_active_time;
	long last_active_time;
	long last_in_pvs_time;
	long inactive_erase_time;
	short highest_prop_status_timer;
	short highest_prop_state_timer;
	long character_definition_index;
	long equipment_use_index;
	long first_prop_ref_index;
	real min_trackable_weight;
	long clump_index;

	byte __data68[0x4];
};
static_assert(sizeof(actor_meta_data) == 0x6C);

struct invitation_data
{
	byte __data[0x10];
};
static_assert(sizeof(invitation_data) == 0x10);

struct s_actor_patrol_state
{
	byte __data0[0x8];
};
static_assert(sizeof(s_actor_patrol_state) == 0x8);

struct actor_state_data
{
	byte behavior_state_data[512];
	short behavior_state_offsets[8];
	short leaf_layer;

	byte __data212[2];

	invitation_data const invitation[4];
	long const behavior_check_timer[14];
	short mode;
	short combat_status;
	long combat_status_timer;

	byte __data294[0x3C];

	bool desired_berserk;
	bool desired_stowed;
	char not_sleeping_ticks;

	byte __pad2D3[0x1];

	short berserk_ticks;
	short covered_ticks;
	s_actor_patrol_state patrol_state;

	byte __data2E0[0x4];
};
static_assert(sizeof(actor_state_data) == 0x2E4);

struct actor_input_data
{
	real_point3d __unknown0; // head_position?
	real_point3d __unknownC;
	byte __data18[0xC];
	s_cluster_reference __unknown24;
	byte __data26[0x16];
	long root_vehicle_index;
	short vehicle_driver_type;
	byte __data42[0x1E];
	vector3d facing_vector;
	vector3d aiming_vector;
	vector3d looking_vector;
	byte __data84[0x34];
};
static_assert(sizeof(actor_input_data) == 0xB8);

struct actor_obstacle_data
{
	byte __data0[0x24];
};
static_assert(sizeof(actor_obstacle_data) == 0x24);

struct actor_memory_data
{
	byte __data0[0x70];
};
static_assert(sizeof(actor_memory_data) == 0x70);

struct actor_situation
{
	byte __data0[0x2];
	short highest_prop_class;
	long highest_prop_class_prop_index;
	byte __data8[0x4];
};
static_assert(sizeof(actor_situation) == 0xC);

struct actor_activity_data
{
	short __unknown0;
	short __unknown2;
	short __unknown4;
	byte __data6[0x2];
	long __unknown8;
};
static_assert(sizeof(actor_activity_data) == 0xC);

struct actor_vehicle_data
{
	long attached_vehicle_index;
	long attached_seat_unit_index;

	byte __data8[0x6];

	short attached_seat_index;
	short attachment_status;

	byte __data12[0x1A];
};
static_assert(sizeof(actor_vehicle_data) == 0x2C);

struct actor_player_data
{
	byte __data0[0x18];
};
static_assert(sizeof(actor_player_data) == 0x18);

struct actor_emotion_data
{
	byte __data0[0x10];
};
static_assert(sizeof(actor_player_data) == 0x18);

struct actor_discarded_firing_position
{
	byte __data0[0x6];
};
static_assert(sizeof(actor_discarded_firing_position) == 0x6);

struct actor_firing_position_data
{
	bool current_position_found_outside_range;
	bool search_areas_available;
	bool ignore_default_areas;
	bool current_position_is_goal;
	short goal_status;
	byte __data6[0x2];
	long dynamic_firing_set_index;
	long dynamic_firing_set_support_object_index;
	short dynamic_firing_set_support_object_inaccessible_ticks;
	short current_discarded_firing_positions_entry;
	actor_discarded_firing_position discarded_firing_positions[4];
	long current_position_index;
	byte __data30[0x10];
	bool proxy_valid;
	byte __pad41[0x3];
};
static_assert(sizeof(actor_firing_position_data) == 0x44);

struct actor_orders
{
	byte __data0[0xA0];
};
static_assert(sizeof(actor_orders) == 0xA0);

struct actor_action_data
{
	byte __data0[0x30];
};
static_assert(sizeof(actor_action_data) == 0x30);

struct actor_script_data
{
	byte __data0[0xC];
};
static_assert(sizeof(actor_script_data) == 0xC);

struct actor_datum :
	s_datum_header
{
	byte __pad2[0x2];

	actor_meta_data meta;
	actor_state_data state;
	actor_input_data input;
	actor_obstacle_data obstacles;
	actor_memory_data memory;
	actor_situation situation;

	byte __data4AC[0xA4];

	// offset: 0x4AC, size: 0x???, struct target;
	// 0x4AC	target.flags

	// offset: 0x???, size: 0x???, struct stimuli;
	
	actor_activity_data activity;
	actor_vehicle_data vehicle;
	actor_player_data player;
	actor_emotion_data emotions;
	actor_firing_position_data firing_positions;
	actor_orders orders;
	actor_action_data actions;

	byte __data6C4[0x3C8];

	// offset: 0x6C4, size: 0x???, struct control;
	// 0x814	control.moving_towards_vector
	// 0x8EC	control.desired_facing_vector
	// 0x8F8	control.desired_aiming_vector
	// 0x904	control.desired_looking_vector
	// 0x932	control.current_fire_target_type
	// 0x9B8	control.burst_aim_vector

	// offset: 0x???, size: 0x???, struct output;

	actor_script_data commands;
};
static_assert(0x004 == offsetof(actor_datum, meta));
static_assert(0x070 == offsetof(actor_datum, state));
static_assert(0x354 == offsetof(actor_datum, input));
static_assert(0x40C == offsetof(actor_datum, obstacles));
static_assert(0x430 == offsetof(actor_datum, memory));
static_assert(0x4A0 == offsetof(actor_datum, situation));
static_assert(0x4AC == offsetof(actor_datum, __data4AC));
static_assert(0x550 == offsetof(actor_datum, activity));
static_assert(0x55C == offsetof(actor_datum, vehicle));
static_assert(0x588 == offsetof(actor_datum, player));
static_assert(0x5A0 == offsetof(actor_datum, emotions));
static_assert(0x5B0 == offsetof(actor_datum, firing_positions));
static_assert(0x5F4 == offsetof(actor_datum, orders));
static_assert(0x694 == offsetof(actor_datum, actions));
static_assert(0x6C4 == offsetof(actor_datum, __data6C4));
static_assert(0xA8C == offsetof(actor_datum, commands));
static_assert(sizeof(actor_datum) == 0xA98);

struct ai_reference_frame
{
	byte __data[0x4B0];
};
static_assert(sizeof(ai_reference_frame) == 0x4B0);

struct actor_iterator
{
	actor_datum* datum;
	s_data_iterator iterator;
	bool __unknown10;
	long actor_index;
};
static_assert(sizeof(actor_iterator) == 0x18);

extern void __cdecl actor_iterator_new(actor_iterator* iterator, bool a2);
extern actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator);

