#pragma once

#include "cseries/cseries.hpp"

struct c_rasterizer_loading_screen
{
	static bool __cdecl active();
	static void __cdecl dispose();
	static bool __cdecl fading_out();
	static real __cdecl get_completion_remainder();
	static void __cdecl initialize();
	static void __cdecl render();
	static void __cdecl set_completion_remainder(real completion_remainder);
	static void __cdecl start();
	static void __cdecl stop();
	static bool __cdecl suppress_render_scene();
};

