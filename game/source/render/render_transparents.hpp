#pragma once

#include "cseries/cseries.hpp"

struct s_transparent_types
{
	bool use_plane;
	real32 z_sort;
	real_point3d centroid;
	real_plane3d plane;
	int32 sort_layer;
	real_point3d anchor_points[9];
	real32 importance;
	void(__cdecl* render_callback)(const void*, int32);
	const void* user_data;
	int32 user_context;
};
static_assert(sizeof(s_transparent_types) == 0xA4);

struct s_transparency_marker
{
	uns16 starting_transparent_index;
};
static_assert(sizeof(s_transparency_marker) == sizeof(uns16));

template<typename t_type, int32 k_count>
class c_sorter
{
public:
	c_static_array<uns16, k_count> m_order;
	int32 m_range[2];
	uns16 m_count;
	t_type* m_data;
};
static_assert(sizeof(c_sorter<s_transparent_types, 1024>) == 0x810);

class c_transparency_renderer
{
public:
	static void __cdecl render(bool depth_test);
	static void __cdecl set_active_camo_bounds(const rectangle2d* window_pixel_bounds, const rectangle2d* render_pixel_bounds);
	static void __cdecl sort();

//private:
	//static int32& m_current_marker_index;
	//static c_sorter<s_transparent_types, 768>& transparent_sorted_order;
	//static bool& m_using_active_camo;
	//static bool& m_needs_active_camo_ldr_resolve;
	//static s_transparency_marker(&m_markers)[6][3];
	//static int32& m_total_transparent_count;
	//static rectangle2d& m_active_camo_resolve_bounds;
	//static real_vector4d& m_active_camo_distort_bounds;
	//static c_static_array<s_transparent_types, 768>& transparents;
};

extern int16& render_debug_transparent_sort_method;

