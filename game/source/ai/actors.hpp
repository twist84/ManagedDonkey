#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct actor_meta_data
{
	short type;

	byte ___data2[0xE];

	long unit_index;
	long swarm_index;

	byte __data18[0x1];

	short _team;

	byte ___data1E[0x2];

	long prevehicle_squad_index;
	short squad_adoption_timeout;
	short squad_adoption_attempts;
	short outside_map_ticks;

	byte __data2A[0x2];

	long squad_index;

	byte ___data30[0xC];

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

	byte __data290[0x40];

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

	byte __data[0x736];

	actor_script_data commands;

	// offset: 0x354, size: ?????, struct input;
	// 0x354	input.__unknown0
	// 0x390	input.root_vehicle_index
	// 0x394	input.vehicle_driver_type
	// 0x3B4	input.facing_vector
	// 0x3C0	input.aiming_vector
	// 0x3CC	input.looking_vector

	// offset: 0x40C?, size: 0x??, struct obstacles;
	// offset: 0x???, size: 0x???, struct memory;
	// offset: 0x4A0, size: 0x00C, struct situation;
	
	// offset: 0x4AC, size: 0x???, struct target;
	// 0x4AC	target.flags

	// offset: 0x???, size: 0x???, struct stimuli;
	// offset: 0x550, size: 0x00C, struct activity;
	// offset: 0x55C, size: 0x02C, struct vehicle;
	
	// offset: 0x588, size: 0x018, struct player;
	
	// offset: 0x5A0?, size: 0x???, struct interaction;
	
	// offset: 0x???, size: 0x???, struct emotions;
	
	// offset: 0x5B0, size: 0x044, struct firing_positions;
	// 0x5B0	firing_positions.current_position_found_outside_range
	// 0x5B1	firing_positions.search_areas_available
	// 0x5B2	firing_positions.ignore_default_areas
	// 0x5B3	firing_positions.current_position_is_goal
	// 0x5B4	firing_positions.goal_status
	// 0x5B8	firing_positions.dynamic_firing_set_index
	// 0x5BC	firing_positions.dynamic_firing_set_support_object_index
	// 0x5C0	firing_positions.dynamic_firing_set_support_object_inaccessible_ticks
	// 0x5C2	firing_positions.current_discarded_firing_positions_entry
	// 0x5C4	firing_positions.discarded_firing_positions
	// 0x5DC	firing_positions.current_position_index
	// 0x5F0	firing_positions.proxy_valid
	
	// offset: 0x5F4, size: 0x0A0, struct orders;
	// offset: 0x694, size: 0x030, struct actions;
	
	// offset: 0x6C4, size: 0x???, struct control;
	// 0x814	control.moving_towards_vector
	// 0x8EC	control.desired_facing_vector
	// 0x8F8	control.desired_aiming_vector
	// 0x904	control.desired_looking_vector
	// 0x932	control.current_fire_target_type
	// 0x9B8	control.burst_aim_vector

	// offset: 0x???, size: 0x???, struct output;
};
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

