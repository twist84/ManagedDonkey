#pragma once

#include "cseries/cseries.hpp"

enum e_recycling_constants
{
	k_recycling_history_count = 64,
	k_recycling_max_volumes = 16,
	k_recycling_max_initial_groups = 112,
	k_recycling_max_volume_groups = 16,
	k_recycling_max_groups = 128,
};

enum e_recycling_methods
{
	_recycle_normally = 0,
	_recycle_everything_ready_excluding_visible = 1,
	_recycle_everything_ready_including_visible = 2,
};

enum e_recycling_object_flag_bits
{
	_recycling_object_waiting = 0,
	_recycling_object_onscreen,
	_recycling_object_active_only,
	k_recycling_object_flag_bits,
};

enum e_object_recycling_flags
{
	_object_recycling_ignore = 0,
	_object_recycling_candidate,
	_object_recycling_active_candidate,
	_object_recycling_filtered,
	_object_recycling_filter_waiting,
	_object_recycling_filter_visible,

	k_object_recycling_flags_count,
};
typedef c_flags<e_object_recycling_flags, uns8, k_object_recycling_flags_count> c_object_recycling_flags;

enum e_recycling_tolerance_flag_bits
{
	_recycling_reduce_memory = 0,
	_recycling_reduce_objects,
	_recycling_reduce_activity,
	_recycling_tolerances_critical,
	k_recycling_tolerance_flag_bits_count,

	_recycling_mask_reduce_activity = FLAG(_recycling_reduce_activity),
	_recycling_mask_reduce_overhead = FLAG(_recycling_reduce_memory) | FLAG(_recycling_reduce_objects),
	_recycling_mask_critical = FLAG(_recycling_tolerances_critical),
	_recycling_mask_any = NONE,
};
typedef c_flags<e_recycling_tolerance_flag_bits, int32, k_recycling_tolerance_flag_bits_count> c_recycling_tolerance_flags;

struct s_recycling_volume
{
	int32 previous_volume;
	int32 next_volume;
	int32 trigger_volume_index;
	int32 max_object_count;
	int32 ending_tick;
};
static_assert(sizeof(s_recycling_volume) == 0x14);

struct s_recycling_volumes
{
	int32 recycling_volume_list_top;
	int32 recycling_volume_free_list_top;
	s_recycling_volume recycling_volume_members[k_recycling_max_volumes];
};
static_assert(sizeof(s_recycling_volumes) == 0x148);

struct s_recycling_group :
	s_datum_header
{
	int16 member_count;
	int32 first_member;
	real_point3d world_position;
};
static_assert(sizeof(s_recycling_group) == 0x14);

struct s_recycling_entry
{
	int32 score;
	int32 index;
	real_rectangle3d bounding_box;
};
static_assert(sizeof(s_recycling_entry) == 0x20);

struct s_recycling_workspace
{
	bool membership_updated;
	int32 group_count;
	s_recycling_entry group_list[k_recycling_max_groups];
};
static_assert(sizeof(s_recycling_workspace) == 0x1008);

struct s_recycling_history_item
{
	real_point3d world_center;
	real32 radius;
};
static_assert(sizeof(s_recycling_history_item) == 0x10);

