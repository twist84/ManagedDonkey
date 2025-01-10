#pragma once

#include "cseries/cseries.hpp"

struct swarm_datum :
	s_datum_header
{
	long actor_index;
	real_point3d swarm_center;
	short member_count;
	short original_member_count;
	long first_swarm_creature_index;
	short num_killed;
	long last_kill_time;
	real_point3d anchor;
	bool anchor_valid;
	bool connected_to_spawner;
};
static_assert(sizeof(swarm_datum) == 0x34);

struct s_swarm_spawner
{
	long swarm_index;
	short lifetime_ticks;
	short ticks_until_next_creation;
	short flags;
	long character_definition_index;
	long object_index;
	long marker;
	real_matrix4x3 local_transform;
	short node_index;
	real yaw_min;
	real yaw_max;
	real pitch_min;
	real pitch_max;
	real velocity_min;
	real velocity_max;
	real form_lifetime_min;
	real form_lifetime_max;
	real time_between_min;
	real time_between_max;
};
static_assert(sizeof(s_swarm_spawner) == 0x78);
static_assert(sizeof(s_swarm_spawner) * 5 == 0x258);

struct s_spawner_globals
{
	short creatures_count;
};
static_assert(sizeof(s_spawner_globals) == 0x2);

extern void __cdecl swarm_delete(long swarm_index);

