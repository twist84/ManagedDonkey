#pragma once

#include "cseries/cseries.hpp"

struct s_new_interaction_event
{
	real_point3d position;
	real_vector3d object_velocity;
	real_vector3d water_velocity;
	long water_ripple_definition_index;
};
static_assert(sizeof(s_new_interaction_event) == 0x28);
static_assert(sizeof(s_new_interaction_event) * 128 == 0x1400);

struct c_water_renderer
{
public:
	static void __cdecl set_player_window(long window_index, long window_count, bool is_widescreen);

	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl game_update();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	static bool __cdecl is_active_ripple_exist();
	static void __cdecl render_shading();
	static void __cdecl render_tessellation(bool is_screenshot);
	static void __cdecl render_underwater_fog();
	static void __cdecl ripple_add(uint32 valid_event_count);
	static void __cdecl ripple_apply();
	static uint32 __cdecl ripple_check_new();
	static void __cdecl ripple_slope();
	static void __cdecl ripple_update();
	static void __cdecl set_performance_throttles();
	static bool __cdecl update_water_part_list();

	static void frame_advance(real32 seconds_elapsed);
};

extern bool& render_water_enabled;
extern bool& render_water_tessellation_enabled;
extern bool& render_water_interaction_enabled;
extern bool& render_underwater_fog_enabled;
extern bool& render_water_wireframe_enabled;

