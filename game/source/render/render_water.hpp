#pragma once

#include "cseries/cseries.hpp"

struct s_interaction_event
{
	byte __data[0x1400];
};
static_assert(sizeof(s_interaction_event) == 0x1400);

struct c_water_renderer
{
public:
	static void __cdecl game_update();
};

extern bool& render_water_enabled;
extern bool& render_water_tessellation_enabled;
extern bool& render_water_interaction_enabled;
extern bool& render_water_wireframe_enabled;

