#pragma once

#include "cseries/cseries.hpp"

struct s_soft_ceiling_debug_cache
{
	long time_stamp;
	real32 search_radius;
	real_point3d triangle[3];
	real_point3d position;
	real_point3d contact_position;
	real_vector3d normal;
};
static_assert(sizeof(s_soft_ceiling_debug_cache) == 0x50);

struct s_scenario_soft_ceilings_globals
{
	c_static_flags<128> flags;
};
static_assert(sizeof(s_scenario_soft_ceilings_globals) == 0x10);

extern bool soft_ceilings_disable;

extern void __cdecl scenario_soft_ceilings_dispose();
extern void __cdecl scenario_soft_ceilings_dispose_from_old_map();
extern void __cdecl scenario_soft_ceilings_initialize();
extern void __cdecl scenario_soft_ceilings_initialize_for_new_map();

extern void __cdecl scenario_soft_ceilings_render_debug(
	real_point3d const* point,
	bool render_soft_ceilings,
	bool render_soft_kill,
	bool render_slip_surfaces);

