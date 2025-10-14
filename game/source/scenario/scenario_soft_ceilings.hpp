#pragma once

#include "cseries/cseries.hpp"

struct s_soft_ceiling_debug_cache
{
	int32 time_stamp;
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

extern void __cdecl scenario_soft_ceiling_enable(string_id name, bool enable);
extern bool __cdecl scenario_soft_ceiling_is_active(string_id name);
extern void __cdecl scenario_soft_ceilings_dispose();
extern void __cdecl scenario_soft_ceilings_dispose_from_old_map();
extern void __cdecl scenario_soft_ceilings_initialize();
extern void __cdecl scenario_soft_ceilings_initialize_for_new_map();

extern void __cdecl scenario_soft_ceilings_render_debug(
	const real_point3d* point,
	bool render_soft_ceilings,
	bool render_soft_kill,
	bool render_slip_surfaces);

