#pragma once

#include "cseries/cseries.hpp"

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

struct s_flock_instance;
struct s_flock_palette_entry;

