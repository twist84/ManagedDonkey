#pragma once

#include "ai/joint_behavior.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"

class c_clump_behavior_state
{
public:
	int16 m_state;
	uns16 m_flags;
	int32 m_time;
};
COMPILE_ASSERT(sizeof(c_clump_behavior_state) == 0x8);

struct clump_datum :
	s_datum_header
{
	real_point3d position;
	int16 num_members;
	int16 team_index;
	real32 nearest_player_distance;
	real32 membership_radius;
	int32 first_prop_index;
	int32 first_actor_index;
	bool active;
	bool giant_clump;
	int16 refresh_timer;
	int32 last_active_time;
	real32 importance_threshold;
	real32 min_importance;
	real32 max_salience;
	int16 importance_threshold_freeze_ticks;
	int16 fought_flags;
	c_clump_behavior_state behavior_state;
	real32 max_clump_perception_distance;
	int32 last_new_enemy_sighted_time;
	int32 last_relevance_time;
	bool contains_disposable;
	bool enemy_sighted_vocalized;
	int16 death_count;
	int32 dialogue_state;
	int32 last_grenade_throw_time;
	int32 debug_grenade_throw_actor_index;
	int32 debug_grenade_throw_request_time;
	int32 behavior_check_timer[1];
	invitation_data const invitations[10];
};
COMPILE_ASSERT(sizeof(clump_datum) == 0x108);

struct clump_iterator
{
	c_data_iterator<clump_datum> iterator;
	int32 index;
};
COMPILE_ASSERT(sizeof(clump_iterator) == 0x14);

struct clump_prop_iterator
{
	clump_datum* clump;
	int32 index;
	int32 next_index;
};
COMPILE_ASSERT(sizeof(clump_prop_iterator) == 0xC);

extern const char* const group_state_names[4];

extern void __cdecl clumps_dispose();
extern void __cdecl clumps_dispose_from_old_map();
extern void __cdecl clumps_initialize();
extern void __cdecl clumps_initialize_for_new_map();
extern void __cdecl clumps_update();
extern void __cdecl clumps_update_actor_membership();
extern void __cdecl clumps_update_state();

