#pragma once

#include "cseries/cseries.hpp"

enum e_performance_throttle_flags
{

};

struct s_performance_throttles
{
	// e_performance_throttle_flags
	uns32 flags;

	real32 water_lod;

	// 0 = off
	real32 decorator_fade_distance_scale;

	// effect lod limit
	real32 effect_lod_distance_scale;

	real32 instance_fade_modifier;
	real32 object_fade_modifier;
	real32 object_lod_threshold_modifier;
	real32 decal_fade_distance_scale;

	// will quickly fade cpu lights when we try to render more than this many
	int32 max_cpu_dynamic_lights;

	// scales the size used for distance-fade (set smaller to make it fade earlier)
	real32 cpu_light_fade_distance_scale;

	// will quickly fade gpu lights when we try to render more than this many
	int32 max_gpu_dynamic_lights;

	// scales the size used for distance-fade (set smaller to make it fade earlier)
	real32 gpu_light_fade_distance_scale;

	// 0 = off
	int32 max_shadow_casting_objects;

	real32 shadow_quality_lod;

	// less than 1.0 - off, equals 1.0 - 4x, larger than 1.0 - 8x
	// real32 anisotropy_level;
};
static_assert(sizeof(s_performance_throttles) == 0x38);

class c_performance_throttles
{
public:
	static s_performance_throttles* __cdecl get_current_performance_throttles();
	static real32 __cdecl get_performance_throttle(const char* name, int32 player_count);
	static void __cdecl set_performance_throttle(const char* name, int32 player_count, real32 value);
	static void __cdecl update_current_performance_throttles();

public:
	// index 0:	default non split screen
	// index 1:	two way split screen
	// index 2:	three way split screen
	// index 3:	four way split screen
	static s_performance_throttles(&m_default_throttles)[4];

	static real32(&m_object_modifiers)[15 /* k_object_type_count */];

	static bool& m_ignore_predefined_performance_throttles;
	static s_performance_throttles& m_current_throttles;
};

extern real32 __cdecl get_performance_throttle(const char* name, int32 player_count);
extern void __cdecl set_performance_throttle(const char* name, int32 player_count, real32 value);

