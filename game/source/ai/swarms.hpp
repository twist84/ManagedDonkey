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
	real32 yaw_min;
	real32 yaw_max;
	real32 pitch_min;
	real32 pitch_max;
	real32 velocity_min;
	real32 velocity_max;
	real32 form_lifetime_min;
	real32 form_lifetime_max;
	real32 time_between_min;
	real32 time_between_max;
};
static_assert(sizeof(s_swarm_spawner) == 0x78);
static_assert(sizeof(s_swarm_spawner) * 5 == 0x258);

struct s_spawner_globals
{
	short creatures_count;
};
static_assert(sizeof(s_spawner_globals) == 0x2);

struct creature_datum;
struct swarm_creature_iterator
{
	long next_creature_index;
	long creature_index;
};
static_assert(sizeof(swarm_creature_iterator) == 0x8);

extern void __cdecl swarm_creature_iterator_new(long swarm_index, swarm_creature_iterator* iterator);
extern creature_datum* __cdecl swarm_creature_iterator_next(swarm_creature_iterator* iterator);
extern void __cdecl swarm_delete(long swarm_index);
extern swarm_datum* __cdecl swarm_get(long swarm_index);
extern void __cdecl swarm_spawners_update();
extern void __cdecl swarm_update(long actor_index);
extern void __cdecl swarms_dispose_from_old_map();
extern void __cdecl swarms_initialize();
extern void __cdecl swarms_initialize_for_new_map();

