#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"

#define MAXIMUM_DISC_COUNT 256

struct disc
{
	word_flags flags;
	short obstacle_index;
	long object_index;
	real_point2d center;
	real radius;

	// debug only?
	//real __unknown14;
};
static_assert(sizeof(struct disc) == 0x14);

struct obstacles
{
	short obstacle_count;
	short disc_count;
	short __unknown4;
	short __unknown6;
	short __unknown8;
	byte __dataA[2];
	disc discs[MAXIMUM_DISC_COUNT];
};
static_assert(sizeof(struct obstacles) == 0x140C);

extern struct disc const* __cdecl obstacles_get_disc(struct obstacles const* obstacles, short disc_index);
extern void __cdecl obstacles_new(struct obstacles* obstacles);

extern void render_debug_obstacles(struct obstacles const* obstacles, real radius);

