#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_dynamic_area
{
	byte __data0[0x6];
};
static_assert(sizeof(s_dynamic_area) == 0x6);

struct squad_datum :
	s_datum_header
{
	// e_squad_runtime_flags
	word_flags flags;

	byte __data4[0x8];

	real original_vitality;
	real original_count;
	short current_count;
	short current_swarm_count;
	short current_non_swarm_count;

	byte __data1A[0x2];

	real current_strength_fraction;
	short prebattle_living_count;
	short current_in_combat_count;
	real_point3d mean_position;
	short spawn_count;
	char combat_status;

	byte __data33[0x1];

	short remain_active_timer;
	char dynamic_area_count;

	byte __data37[0x1];

	s_dynamic_area const dynamic_areas[16];

	long last_dynamic_area_update_time;
	short objective_index;
	short task_index;
	short previous_objective_index;
	short previous_task_index;
	long objective_start_time;
	long first_actor_index;
	long parent_index;
	long first_vehicle_index;
	short next_sibling_index;
	char team;

	byte __padB7[0x1];

	long last_active_time;
	short ticks_outside_of_pvs;

	byte __padBE[0x2];

	long follow_target_unit_index;
	c_static_flags<256> style_bitvector;
	short objective_next_squad_index;
	bool temporary_task;

	byte __dataE7[0x1];

	short highest_rank;

	byte __padEA[0x2];
};
static_assert(sizeof(squad_datum) == 0xEC);

struct squad_group_datum :
	s_datum_header
{
	byte __data[0x22];
};
static_assert(sizeof(squad_group_datum) == 0x24);

