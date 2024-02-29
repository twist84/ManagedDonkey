#include "rasterizer/rasterizer_main.hpp"

#include "memory/thread_local.hpp"

bool render_debug_force_4x3_aspect_ratio = false;

long __cdecl get_current_thread_index()
{
	return INVOKE(0x0051C410, get_current_thread_index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_registered_thread_index);
	//return g_registered_thread_index;
}

