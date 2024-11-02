#pragma once

#include "cseries/cseries.hpp"

struct s_performane_throttle
{
	// e_performance_throttle_flags
	dword_flags flags;

	real water_lod;

	// 0 = off
	real decorator_fade_distance_scale;

	// effect lod limit
	real effect_lod_distance_scale;

	real instance_fade_modifier;
	real object_fade_modifier;
	real object_lod_threshold_modifier;
	real decal_fade_distance_scale;

	// will quickly fade cpu lights when we try to render more than this many
	long max_cpu_dynamic_lights;

	// scales the size used for distance-fade (set smaller to make it fade earlier)
	real cpu_light_fade_distance_scale;

	// will quickly fade gpu lights when we try to render more than this many
	long max_gpu_dynamic_lights;

	// scales the size used for distance-fade (set smaller to make it fade earlier)
	real gpu_light_fade_distance_scale;

	// 0 = off
	long max_shadow_casting_objects;

	real shadow_quality_lod;

	// less than 1.0 - off, equals 1.0 - 4x, larger than 1.0 - 8x
	// real anisotropy_level;
};
static_assert(sizeof(s_performane_throttle) == 0x38);

struct s_performance_throttles
{
	// index 0:	default non split screen
	// index 1:	two way split screen
	// index 2:	three way split screen
	// index 3:	four way split screen
	s_performane_throttle throttles[4];
};

struct c_performance_throttles
{
	static void __cdecl update_current_performance_throttles();
};

extern bool& g_ignore_predefined_performance_throttles;
extern s_performance_throttles& g_default_performance_throttles;
extern s_performane_throttle& g_current_performance_throttles;