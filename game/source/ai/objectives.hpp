#pragma once

#include "ai/clump.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "tag_files/tag_groups.hpp"

struct s_objective_datum :
	s_datum_header
{
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(s_objective_datum) == 0xC);

struct s_task_record
{
	long leader;
	short body_count;
	short lowest_rank;
	short highest_rank;
	word_flags flags; // assume odst flags
	long __timeC;
	long __time10;
	long __time14;
	long __time18;
	long __time1C;
	long __time20;
	long __time24;
	real_point3d position;
	bool position_calculated;
	byte __data35[0x1];
	short __unknown36;
	c_clump_behavior_state state;
};
static_assert(sizeof(s_task_record) == 0x40);

struct s_area_reference
{
	short area_type;
	word_flags flags;
	word_flags character_flags;
	short zone;
	short area;
	angle yaw;
	//dword_flags connection_flags;
};
static_assert(sizeof(s_area_reference) == 0x10);

enum e_task_flags
{
	_task_latch_on_bit = 0,
	_task_latch_off_bit,
	_task_gate_bit,
	_task_single_use_bit,
	_task_suppress_combat_bit,
	_task_active_camo_bit,
	_task_blind_bit,
	_task_deaf_bit,
	_task_braindead_bit,
	_task_magic_player_sight_bit,
	_task_disable_bit,
	_task_ignore_fronts_bit,
	_task_dont_generate_front_bit,
	_task_reverse_direction_bit,

	k_task_flags
};

enum e_inhibit_behavior_flags
{
	_inhibit_behavior_cover_bit = 0,
	_inhibit_behavior_retreat_bit,
	_inhibit_behavior_vehicles_bit,
	_inhibit_behavior_grenades_bit,
	_inhibit_behavior_berserk_bit,
	_inhibit_behavior_equipment_bit,
	_inhibit_behavior_pureform_ranged_bit,
	_inhibit_behavior_pureform_tank_bit,
	_inhibit_behavior_pureform_stalker_bit,

	k_inhibit_behavior_flags
};

enum e_inhibit_difficulty_flags
{
	_inhibit_difficulty_easy_bit = 0,
	_inhibit_difficulty_normal_bit,
	_inhibit_difficulty_heroic_bit,
	_inhibit_difficulty_legendary_bit,

	k_inhibit_difficulty_flags
};

enum e_task_runtime_flags
{
	_task_runtime_area_connectivity_valid_bit = 0,

	k_task_runtime_flags
};

enum e_filter_flags
{
	_filter_exclusive_bit = 0,

	k_filter_flags
};

enum e_filter
{
	_filter_none = 0,
	_filter_leader,
	_filter_no_leader,
	_filter_arbiter,
	_filter_player_in_my_vehicle,
	_filter_in_combat,
	_filter_sighted_player,
	_filter_sighted_enemy,
	_filter_target_disengaged,
	_filter_infantry,
	_filter_has_an_engineer,
	_filter_strength_gt_0_25,
	_filter_strength_gt_0_5,
	_filter_strength_gt_0_75,
	_filter_strength_lt_0_25,
	_filter_strength_lt_0_5,
	_filter_strength_lt_0_75,
	_filter_human_team,
	_filter_covenant_team,
	_filter_flood_team,
	_filter_sentinel_team,
	_filter_heretic_team,
	_filter_prophet_team,
	_filter_elite,
	_filter_jackal,
	_filter_grunt,
	_filter_hunter,
	_filter_marine,
	_filter_combat_form,
	_filter_carrier_form,
	_filter_sentinel,
	_filter_brute,
	_filter_prophet,
	_filter_bugger,
	_filter_pureform,
	_filter_guardian,
	_filter_engineer,
	_filter_sniper,
	_filter_rifle,
	_filter_vehicle,
	_filter_scorpion,
	_filter_ghost,
	_filter_warthog,
	_filter_spectre,
	_filter_wraith,
	_filter_phantom,
	_filter_pelican,
	_filter_banshee,
	_filter_hornet,
	_filter_chopper,
	_filter_mauler,
	_filter_mongoose,

	k_filter_count
};

struct s_task
{
	c_flags<e_task_flags, word, k_task_flags> flags;
	c_flags<e_inhibit_behavior_flags, word, k_inhibit_behavior_flags> inhibit_groups;
	c_flags<e_inhibit_difficulty_flags, word, k_inhibit_difficulty_flags> inhibit_on_difficulty;
	int16_bounds round_range;
	int16_bounds set_range;
	short movement;
	short follow;
	short follow_squad; // short_block_index
	real follow_radius;

	// Don't follow at areas outside of this vertical margin
	real follow_z_clamp; // wu/s

	real player_front_radius;

	// Exhaust this task after it has been active for this long
	real maximum_duration; // seconds

	// When a task exhausts, hold actors in the task for this long before releasing them
	real exhaustion_delay; // seconds

	c_static_string<k_tag_string_length> entry_script;
	c_static_string<k_tag_string_length> command_script;
	c_static_string<k_tag_string_length> exhaustion_script;
	short entry_script_index;
	short command_script_index;
	short exhaustion_script_index;

	short squad_group_filter; // short_block_index

	// when someone enters this task for the first time, they play this type of dialogue
	short dialogue_type;

	c_flags<e_task_runtime_flags, word, k_task_runtime_flags> runtime_flags;
	s_tag_block pureform_distribution;

	// The number of guys under this task that should be allowed to fight the player at a time
	short kungfu_count;

	byte post_kungfu_count[2];


	// UI-Controlled
	// You don't need to modify these here. They are managed by the objectives ui

	c_string_id name;
	short priority;
	short first_child; // short_block_index
	short next_sibling; // short_block_index
	short parent; // short_block_index
	s_tag_block activation_script;
	short activation_script_index; // short_block_index
	short lifetime_count;
	c_flags<e_filter_flags, word, k_filter_flags> filter_flags;
	c_enum<e_filter, short, _filter_none, k_filter_count> filter;
	int16_bounds capacity;

	// task becomes inactive after the given number of casualties
	short max_body_count;

	short style;

	// task becomes inactive after the strength of the participants falls below the given level
	real min_strength; // [0,1]

	c_typed_tag_block<s_area_reference> areas;
	s_tag_block direction;
};
static_assert(sizeof(s_task) == 0xE8);

enum e_objective_flags
{
	_objective_use_players_as_front_bit,

	k_objective_flags
};

struct s_objective
{
	c_string_id name;
	s_tag_block opposing_objectives;
	c_flags<e_objective_flags, word, k_objective_flags> objective_flags;
	short zone_index; // short_block_index
	short first_task_index; // short_block_index
	short editor_folder; // short_block_index
	c_typed_tag_block<s_task> tasks;
};
static_assert(sizeof(s_objective) == 0x24);

extern s_task_record* __cdecl objective_get_task_record(short objective_index, short task_index);

extern void ai_debug_render_objectives(long squad_index, real_point3d const* position);

