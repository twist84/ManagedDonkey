#pragma once

#include "cseries/cseries.hpp"

struct geometry_material;
struct s_render_geometry;

enum
{
	k_ps_water_is_lightmap_exist = 0,
	k_ps_water_is_interaction = 1,
	k_ps_water_is_tessellated = 2,
	k_ps_water_is_underwater = 3,
	k_vs_water_index_offset = 131,
	k_vs_water_ripple_buffer_radius = 133,
	k_vs_water_ripple_buffer_center = 134,
	k_ps_water_view_xform_inverse = 213,
	k_ps_water_camera_position = 219,
	k_ps_water_view_depth_constant = 217,
	k_ps_water_player_view_constant = 218,
	k_ps_water_underwater_murkiness = 220,
	k_ps_water_underwater_fog_color = 221,
};

struct s_new_interaction_event
{
	real_point3d position;
	real_vector3d object_velocity;
	real_vector3d water_velocity;
	int32 water_ripple_definition_index;
};
COMPILE_ASSERT(sizeof(s_new_interaction_event) == 0x28);
COMPILE_ASSERT(sizeof(s_new_interaction_event) * 128 == 0x1400);

class c_water_renderer
{
public:
	static void __cdecl set_player_window(int32 window_index, int32 window_count, bool is_widescreen);

	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl game_interation_event_add(int32 water_ripple_definition_index, const real_point3d* position, const real_vector3d* object_velocity, const real_vector3d* water_velocity);
	static void __cdecl game_update();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	static bool __cdecl is_active_ripple_exist();

private:
	static void __cdecl render_cluster_parts(e_entry_point entry_point, int32 mesh_part_mask);

public:
	static void __cdecl render_shading();
	static void __cdecl render_tessellation(bool is_screenshot);
	static void __cdecl render_underwater_fog();

private:
	static void __cdecl render_water_part(const s_render_geometry* geometry, int32 mesh_index, int32 part_index, e_entry_point entry_point, const geometry_material* materials);

public:
	static void __cdecl ripple_add(uns32 valid_event_count);
	static void __cdecl ripple_apply();
	static uns32 __cdecl ripple_check_new();
	static void __cdecl ripple_slope();
	static void __cdecl ripple_update();
	static void __cdecl set_performance_throttles();
	static bool __cdecl update_water_part_list();

	static void frame_advance(real32 seconds_elapsed);
};

extern bool& render_water_enabled;
extern bool& render_water_tessllation_on;
extern bool& render_water_interaction_on;
extern int& g_is_underwater;
extern bool& render_water_wireframe_enabled;

extern void __cdecl water_interaction_clear_all(int32 game_state_restore_flags);

