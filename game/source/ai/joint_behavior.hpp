#pragma once

#include "ai/actor_firing_position.hpp"
#include "ai/ai_actions.hpp"
#include "ai/sector.hpp"
#include "cseries/cseries.hpp"

struct invitation_data
{
	short behavior_index;
	short slot;
	long joint_type;
	long jindex;
	long expiration_time;
};
static_assert(sizeof(invitation_data) == 0x10);

struct participant_entry
{
	long actor_index;
	short status;
	short joint_priority;
	real grade;
};
static_assert(sizeof(participant_entry) == 0xC);

struct crossfire_data
{
	long target_object_index;
	real_vector3d target_to_initiator;
	long crossfire_timer;
	long position_timer;
	bool reached_cross_position;
};
static_assert(sizeof(crossfire_data) == 0x1C);

struct surprise_data
{
	long target_object_index;
};
static_assert(sizeof(surprise_data) == 0x4);

struct joint_formation_data
{
	bool started;
	short formation_type;
	long point_ref;
	long point_ref_destination;
};
static_assert(sizeof(joint_formation_data) == 0xC);

struct joint_scared_by_leader_data
{
	long scared_actor_index;
	long leader_actor_index;
	bool played_berserk;
};
static_assert(sizeof(joint_scared_by_leader_data) == 0xC);

struct coordinated_ambush_data
{
	bool launch_attack;
	bool finished;
};
static_assert(sizeof(coordinated_ambush_data) == 0x2);

struct pursuit_location
{
	short type;
	s_cluster_reference static_cluster_ref;
	c_sector_ref sector_ref;
	c_ai_point3d position;
};
static_assert(sizeof(pursuit_location) == 0x18);

struct pursuit_sync_data
{
	bool finished;
	long object_index;
	long last_completion_test_time;
	short ticks;
	real position_time;
	pursuit_location pursuit_location;
	real orphan_time;
	real_vector3d orphan_vector;
};
static_assert(sizeof(pursuit_sync_data) == 0x3C);

struct joint_vehicle_flip_data
{
	long vehicle_index;
	short num_ready;
	short num_required;
	real_vector3d target_direction;
	short current_position_index;
};
static_assert(sizeof(joint_vehicle_flip_data) == 0x18);

struct joint_presearch_data
{
	long start_time;
	short min_presearch_time;
	short max_presearch_time;
	long pref_index;
	bool vocalized_end;
};
static_assert(sizeof(joint_presearch_data) == 0x10);

struct joint_huddle_data
{
	firing_position_ref position;
	long initiator_index;
	short actor_type;
};
static_assert(sizeof(joint_huddle_data) == 0xC);

struct joint_state_datum :
	s_datum_header
{
	participant_entry participants[10];
	short behavior_index;
	short num_members;
	char status;
	long joint_type;
	long clump_index;
	short pending_timer;

	union
	{
		crossfire_data crossfire;
		surprise_data surprise;
		joint_formation_data formation;
		joint_scared_by_leader_data scared_by_leader;
		coordinated_ambush_data coordinated_ambush;
		pursuit_sync_data pursuit_sync;
		joint_vehicle_flip_data vehicle_flip;
		joint_presearch_data presearch;
		joint_huddle_data huddle;
	};
};
static_assert(sizeof(joint_state_datum) == 0xCC);

extern void __cdecl joint_behavior_create_for_new_map();
extern void __cdecl joint_behavior_dispose();
extern void __cdecl joint_behavior_dispose_from_old_map();
extern bool __cdecl joint_behavior_get_participant_limits(short behavior_index, short* min_participants_ref, short* max_participants_ref);
extern void __cdecl joint_behavior_initialize();
extern void __cdecl joint_behavior_initialize_for_new_map();
extern void __cdecl joint_behaviors_update();

