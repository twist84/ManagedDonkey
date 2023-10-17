#pragma once

#include "cseries/cseries.hpp"

struct s_havok_gamestate
{
	dword __unknown0;
	dword __unknown4;
};
static_assert(sizeof(s_havok_gamestate) == 0x8);

struct s_havok_constants
{
	real collision_tolerance;
	real __unknown4;
	long environment_type;
	long debug_mode;
	real shape_radius;
	bool disable_deactivation;
	real deactivation_reference_distance;
	bool jumping_beans;
	bool weld_environment;
	bool shape_cache;
	bool shape_cache_debug;
	bool enable_back_stepping;
	bool enable_sweep_shapes;
	bool display_stats;
	bool initialize_profiling;
	bool impacts_debug;
	bool proxies_debug;
	bool collision_damage_debug;
	bool play_impact_effects;
	bool play_biped_impact_effects;
	bool physics_debug;
	bool cleanup_inactive_agents;
	bool batch_add_entities_disabled;
	real __unknown2C;
	real __unknown30;
	long thread_count;
};
static_assert(sizeof(s_havok_constants) == 0x38);

extern s_havok_constants& g_havok_constants;
extern bool& g_havok_memory_always_system;

extern void __cdecl havok_can_modify_state_allow();
extern void __cdecl havok_can_modify_state_disallow();
extern void __cdecl havok_prepare_fpu_for_update();
extern void __cdecl havok_restore_fpu_from_update();

extern s_havok_constants const* get_havok_constants();
extern void havok_debug_render();

