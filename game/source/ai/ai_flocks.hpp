#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "tag_files/tag_groups.hpp"

struct flock_definition
{
	real32 forward_scale;
	real32 leveling_force_scale;
	real32 sink_scale;
	real32 ave_throttle;
	real32 max_throttle;
	real32 weight_movement_threshold;
	real32 danger_radius;
	real32 danger_scale;
	real32 target_scale;
	real32 target_distance_max;
	real32 target_delay_time;
	real32 target_kill_chance;
	real32 random_offset_scale;
	real32 random_offset_period_min;
	real32 random_offset_period_max;
	real32 neighborhood_radius;
	real32 perception_angle;
	real32 avoidance_scale;
	real32 avoidance_radius;
	real32 alignment_scale;
	real32 position_scale;
	real32 position_min_radius;
	real32 position_max_radius;
};
COMPILE_ASSERT(sizeof(flock_definition) == 0x5C);

struct s_flock_instance
{
	int32 name;
	int16 flock_palette_index;
	int16 bsp_index;
	int16 bounding_volume_index;
	uns16 flags;
	real32 ecology_margin;
	s_tag_block sources;
	s_tag_block sinks;
	real32 production_frequency_min;
	real32 production_frequency_max;
	real32 scale_min;
	real32 scale_max;
	real32 scale_to_zero_source_radius;
	real32 scale_to_zero_sink_radius;
	int16 creature_index;
	int16 boid_count_min;
	int16 boid_count_max;
	int16 enemy_flock;
};
COMPILE_ASSERT(sizeof(s_flock_instance) == 0x48);

struct s_perlin_state
{
	int16 random_offset_total_ticks;
	int16 random_offset_ticks;
	real_point3d random_offset;
	real_point3d previous_random_offset;
};
COMPILE_ASSERT(sizeof(s_perlin_state) == 0x1C);

struct boid_state
{
	int32 flock_index;
	int32 next_boid_index;
	bool erase;
	bool moving;
	bool active;
	bool perched;
	int16 inactive_ticks;
	int16 source_index;
	int16 sink_index;
	int16 want_to_move_ticks;
	int16 target_update_ticks;
	real32 scale;
	int32 danger_object;
	int32 target_object_index;
	int16 danger_ticks;
	int16 target_shooting_ticks;
	s_perlin_state perlin;
};
COMPILE_ASSERT(sizeof(boid_state) == 0x44);

struct flock_datum :
	s_datum_header
{
	int16 instance_index;
	int32 definition_index;
	int32 first_boid_index;
	int16 boid_count;
	int16 boid_creation_delay_ticks;
	bool active;
	bool emitting;
	bool spewing;
	int16 spew_threshold;
	int16 spew_source_index;
	int16 active_sources;
	int16 used_sources;
	int16 threatening_players;
	real_point3d danger_point;
	real32 danger_scale;
	int16 danger_ticks;
	real_rectangle3d bounding_box;
};
COMPILE_ASSERT(sizeof(flock_datum) == 0x4C);

struct flock_boid_iterator
{
	int32 next_boid_index;
	int32 boid_index;
};
COMPILE_ASSERT(sizeof(flock_boid_iterator) == 0x8);

struct flock_iterator
{
	c_data_iterator<flock_datum> iterator;
	int32 index;
};
COMPILE_ASSERT(sizeof(flock_iterator) == 0x14);

struct s_flock_palette_entry;

extern void __cdecl flocks_dispose_from_old_map();
extern void __cdecl flocks_dispose_from_old_structure_bsp();
extern void __cdecl flocks_initialize();
extern void __cdecl flocks_initialize_for_new_map();
extern void __cdecl flocks_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl flocks_update();

