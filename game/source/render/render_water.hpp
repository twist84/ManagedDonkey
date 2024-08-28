#pragma once

#include "cseries/cseries.hpp"

struct s_ripple_data
{
	real_point3d __point0;
	vector3d __vectorC;
	vector3d __vector18;
	long water_ripple_definition_index;
};
static_assert(sizeof(s_ripple_data) == 0x28);

struct s_interaction_event
{
	c_static_array<s_ripple_data, 128> ripples;
};
static_assert(sizeof(s_interaction_event) == 0x1400);

struct c_water_renderer
{
public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl game_update();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
};

extern bool& render_water_enabled;
extern bool& render_water_tessellation_enabled;
extern bool& render_water_interaction_enabled;
extern bool& render_water_wireframe_enabled;

