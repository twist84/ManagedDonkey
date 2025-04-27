#pragma once

#include "ai/joint_behavior.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_clump_behavior_state
{
	short m_state;
	uint16 m_flags;
	long m_time;
};
static_assert(sizeof(c_clump_behavior_state) == 0x8);

struct clump_datum :
	s_datum_header
{
	real_point3d position;
	short num_members;
	short team_index;
	real32 nearest_player_distance;
	real32 membership_radius;
	long first_prop_index;
	long first_actor_index;
	bool active;
	bool giant_clump;
	short refresh_timer;
	long last_active_time;
	real32 importance_threshold;
	real32 min_importance;
	real32 max_salience;
	short importance_threshold_freeze_ticks;
	short fought_flags;
	c_clump_behavior_state behavior_state;
	real32 max_clump_perception_distance;
	long last_new_enemy_sighted_time;
	long last_relevance_time;
	bool contains_disposable;
	bool enemy_sighted_vocalized;
	short death_count;
	long dialogue_state;
	long last_grenade_throw_time;
	long debug_grenade_throw_actor_index;
	long debug_grenade_throw_request_time;
	long behavior_check_timer[1];
	invitation_data const invitations[10];
};
static_assert(sizeof(clump_datum) == 0x108);

struct clump_iterator
{
	c_data_iterator<clump_datum> iterator;
	long index;
};
static_assert(sizeof(clump_iterator) == 0x14);

struct clump_prop_iterator
{
	clump_datum* clump;
	long index;
	long next_index;
};
static_assert(sizeof(clump_prop_iterator) == 0xC);

extern char const* const group_state_names[4];

extern void __cdecl clumps_dispose();
extern void __cdecl clumps_dispose_from_old_map();
extern void __cdecl clumps_initialize();
extern void __cdecl clumps_initialize_for_new_map();
extern void __cdecl clumps_update();
extern void __cdecl clumps_update_actor_membership();
extern void __cdecl clumps_update_state();

