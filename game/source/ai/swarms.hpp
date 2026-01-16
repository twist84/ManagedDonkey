#pragma once

#include "cseries/cseries.hpp"

struct swarm_datum :
	s_datum_header
{
	int32 actor_index;
	real_point3d swarm_center;
	int16 member_count;
	int16 original_member_count;
	int32 first_swarm_creature_index;
	int16 num_killed;
	int32 last_kill_time;
	real_point3d anchor;
	bool anchor_valid;
	bool connected_to_spawner;
};
COMPILE_ASSERT(sizeof(swarm_datum) == 0x34);

struct s_swarm_spawner
{
	int32 swarm_index;
	int16 lifetime_ticks;
	int16 ticks_until_next_creation;
	int16 flags;
	int32 character_definition_index;
	int32 object_index;
	int32 marker;
	real_matrix4x3 local_transform;
	int16 node_index;
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
COMPILE_ASSERT(sizeof(s_swarm_spawner) == 0x78);
COMPILE_ASSERT(sizeof(s_swarm_spawner) * 5 == 0x258);

struct s_spawner_globals
{
	int16 creatures_count;
};
COMPILE_ASSERT(sizeof(s_spawner_globals) == 0x2);

struct creature_datum;
struct swarm_creature_iterator
{
	int32 next_creature_index;
	int32 creature_index;
};
COMPILE_ASSERT(sizeof(swarm_creature_iterator) == 0x8);

extern void __cdecl swarm_creature_iterator_new(int32 swarm_index, swarm_creature_iterator* iterator);
extern creature_datum* __cdecl swarm_creature_iterator_next(swarm_creature_iterator* iterator);
extern void __cdecl swarm_delete(int32 swarm_index);
extern swarm_datum* __cdecl swarm_get(int32 swarm_index);
extern void __cdecl swarm_spawners_update();
extern void __cdecl swarm_update(int32 actor_index);
extern void __cdecl swarms_dispose_from_old_map();
extern void __cdecl swarms_initialize();
extern void __cdecl swarms_initialize_for_new_map();

