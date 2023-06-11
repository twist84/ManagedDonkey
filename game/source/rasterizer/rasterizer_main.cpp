#include "rasterizer/rasterizer_main.hpp"

#include "memory/thread_local.hpp"

long __cdecl get_current_thread_index()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_registered_thread_index);
	return g_registered_thread_index;
}

