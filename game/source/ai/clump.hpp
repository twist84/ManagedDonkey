#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_clump_behavior_state
{
	short m_state;
	word_flags m_flags;
	long m_time;
};
static_assert(sizeof(c_clump_behavior_state) == 0x8);

struct clump_datum :
	s_datum_header
{
	byte __pad2[0x2];

	real_point3d position;
	short num_members;
	short team;
	real nearest_player_distance;
	real membership_radius;
	long first_prop_index;
	long first_actor_index;
	bool active;
	bool giant_clump;
	short refresh_timer;
	long last_active_time;
	real importance_threshold;
	real min_importance;
	real max_salience;
	short importance_threshold_freeze_ticks;
	word_flags fought_flags;
	c_clump_behavior_state behavior_state;
	real max_clump_perception_distance;
	long last_new_enemy_sighted_time;
	long last_relevance_time;
	bool contains_disposable;
	bool enemy_sighted_vocalized;
	short death_count;
	long dialogue_state;
	long last_grenade_throw_time;

	// padding?
	byte __data5C[0x8];

	long const behavior_check_timer[1];
	invitation_data const invitations[10];
};
static_assert(sizeof(clump_datum) == 0x108);

