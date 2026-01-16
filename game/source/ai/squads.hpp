#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_dynamic_area
{
	byte __data0[0x6];
};
COMPILE_ASSERT(sizeof(s_dynamic_area) == 0x6);

struct squad_datum :
	s_datum_header
{
	// e_squad_runtime_flags
	uns16 flags;

	byte __data4[0x8];

	real32 original_vitality;
	real32 original_count;
	int16 current_count;
	int16 current_swarm_count;
	int16 current_non_swarm_count;

	byte __data1A[0x2];

	real32 current_strength_fraction;
	int16 prebattle_living_count;
	int16 current_in_combat_count;
	real_point3d mean_position;
	int16 spawn_count;
	int8 combat_status;

	byte __data33[0x1];

	int16 remain_active_timer;
	int8 dynamic_area_count;

	byte __data37[0x1];

	s_dynamic_area const dynamic_areas[16];

	int32 last_dynamic_area_update_time;
	int16 objective_index;
	int16 task_index;
	int16 previous_objective_index;
	int16 previous_task_index;
	int32 objective_start_time;
	int32 first_actor_index;
	int32 parent_index;
	int32 first_vehicle_index;
	int16 next_sibling_index;
	int8 team;

	byte __padB7[0x1];

	int32 last_active_time;
	int16 ticks_outside_of_pvs;

	byte __padBE[0x2];

	int32 follow_target_unit_index;
	c_static_flags<256> style_bitvector;
	int16 objective_next_squad_index;
	bool temporary_task;

	byte __dataE7[0x1];

	int16 highest_rank;

	byte __padEA[0x2];
};
COMPILE_ASSERT(sizeof(squad_datum) == 0xEC);

struct squad_group_datum :
	s_datum_header
{
	int16 highest_combat_status;
	int32 first_group_child_index;
	int32 first_leaf_child_index;
	int32 next_sibling_index;
	int32 parent_index;
	bool active;
	bool placed;
	bool enemy_visible;
	int16 current_count;
	int16 original_vitality;
	int16 prebattle_living_count;
	int16 current_in_combat_count;
	real32 current_strength_fraction;
};
COMPILE_ASSERT(sizeof(squad_group_datum) == 0x24);

struct squad_actor_iterator
{
	int32 squad_index;
	int32 index;
	int32 next_index;
	bool active_only;
};
COMPILE_ASSERT(sizeof(squad_actor_iterator) == 0x10);

struct squad_iterator
{
	c_data_iterator<squad_datum> iterator;
	int32 squad_index;
};
COMPILE_ASSERT(sizeof(squad_iterator) == 0x14);

struct actor_datum;

extern void __cdecl squad_actor_iterator_new(squad_actor_iterator* iterator, int32 squad_index, bool active_only);
extern actor_datum* __cdecl squad_actor_iterator_next(squad_actor_iterator* iterator);
extern void __cdecl squad_deactivate(int32 squad_index);
extern squad_datum* __cdecl squad_get(int32 squad_index);
extern void __cdecl squad_iterator_new(squad_iterator* iterator);
extern squad_datum* __cdecl squad_iterator_next(squad_iterator* iterator);
extern void __cdecl squads_dispose();
extern void __cdecl squads_dispose_from_old_map();
extern void __cdecl squads_initialize();
extern void __cdecl squads_initialize_for_new_map();
extern void __cdecl squads_update();
extern void __cdecl squads_update_dirty_status();

