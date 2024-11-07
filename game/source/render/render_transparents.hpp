#pragma once

#include "cseries/cseries.hpp"

struct c_transparency_renderer
{
	static void __cdecl render(bool a1);
	static void __cdecl set_active_camo_bounds(short_rectangle2d const* rect_a, short_rectangle2d const* rect_b);
	static void __cdecl sort();
};

extern short& render_debug_transparent_sort_method;

