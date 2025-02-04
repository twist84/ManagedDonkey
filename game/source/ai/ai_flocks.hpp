#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "tag_files/tag_groups.hpp"

struct flock_definition
{
	real forward_scale;
	real leveling_force_scale;
	real sink_scale;
	real ave_throttle;
	real max_throttle;
	real weight_movement_threshold;
	real danger_radius;
	real danger_scale;
	real target_scale;
	real target_distance_max;
	real target_delay_time;
	real target_kill_chance;
	real random_offset_scale;
	real random_offset_period_min;
	real random_offset_period_max;
	real neighborhood_radius;
	real perception_angle;
	real avoidance_scale;
	real avoidance_radius;
	real alignment_scale;
	real position_scale;
	real position_min_radius;
	real position_max_radius;
};
static_assert(sizeof(flock_definition) == 0x5C);

struct s_flock_instance
{
	long name;
	short flock_palette_index;
	short bsp_index;
	short bounding_volume_index;
	word_flags flags;
	real ecology_margin;
	s_tag_block sources;
	s_tag_block sinks;
	real production_frequency_min;
	real production_frequency_max;
	real scale_min;
	real scale_max;
	real scale_to_zero_source_radius;
	real scale_to_zero_sink_radius;
	short creature_index;
	short boid_count_min;
	short boid_count_max;
	short enemy_flock;
};
static_assert(sizeof(s_flock_instance) == 0x48);

struct s_perlin_state
{
	short random_offset_total_ticks;
	short random_offset_ticks;
	real_point3d random_offset;
	real_point3d previous_random_offset;
};
static_assert(sizeof(s_perlin_state) == 0x1C);

struct boid_state
{
	long flock_index;
	long next_boid_index;
	bool erase;
	bool moving;
	bool active;
	bool perched;
	short inactive_ticks;
	short source_index;
	short sink_index;
	short want_to_move_ticks;
	short target_update_ticks;
	real scale;
	long danger_object;
	long target_object_index;
	short danger_ticks;
	short target_shooting_ticks;
	s_perlin_state perlin;
};
static_assert(sizeof(boid_state) == 0x44);

struct flock_datum :
	s_datum_header
{
	short instance_index;
	long definition_index;
	long first_boid_index;
	short boid_count;
	short boid_creation_delay_ticks;
	bool active;
	bool emitting;
	bool spewing;
	short spew_threshold;
	short spew_source_index;
	short active_sources;
	short used_sources;
	short threatening_players;
	real_point3d danger_point;
	real danger_scale;
	short danger_ticks;
	real_rectangle3d bounding_box;
};
static_assert(sizeof(flock_datum) == 0x4C);

struct flock_boid_iterator
{
	long next_boid_index;
	long boid_index;
};
static_assert(sizeof(flock_boid_iterator) == 0x8);

struct flock_iterator
{
	c_data_iterator<flock_datum> iterator;
	long index;
};
static_assert(sizeof(flock_iterator) == 0x14);

struct s_flock_palette_entry;

extern void __cdecl flocks_dispose_from_old_map();
extern void __cdecl flocks_dispose_from_old_structure_bsp();
extern void __cdecl flocks_initialize();
extern void __cdecl flocks_initialize_for_new_map();
extern void __cdecl flocks_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask);
extern void __cdecl flocks_update();

