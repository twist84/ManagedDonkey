#pragma once

#include "cseries/cseries.hpp"

struct s_performance_throttles
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
static_assert(sizeof(s_performance_throttles) == 0x38);

struct c_performance_throttles
{
public:
	static s_performance_throttles* __cdecl get_current_performance_throttles();
	static real __cdecl get_performance_throttle(char const* name, long player_count);
	static void __cdecl set_performance_throttle(char const* name, long player_count, real value);
	static void __cdecl update_current_performance_throttles();

public:
	// index 0:	default non split screen
	// index 1:	two way split screen
	// index 2:	three way split screen
	// index 3:	four way split screen
	static s_performance_throttles(&m_default_throttles)[4];

	static real(&m_object_modifiers)[15 /* k_object_type_count */];

	static bool& m_ignore_predefined_performance_throttles;
	static s_performance_throttles& m_current_throttles;
};

extern real __cdecl get_performance_throttle(char const* name, long player_count);
extern void __cdecl set_performance_throttle(char const* name, long player_count, real value);

