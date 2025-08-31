#include "rasterizer/rasterizer_main.hpp"

#include "memory/thread_local.hpp"

#include <d3d9.h>

REFERENCE_DECLARE(0x050DCA38, c_interlocked_long, g_thread_owning_device);
REFERENCE_DECLARE(0x050DD7E8, D3DCAPS9, g_global_device_caps);
REFERENCE_DECLARE(0x050DD918, D3DDISPLAYMODE, g_global_display_mode);
REFERENCE_DECLARE(0x050DD928, D3DPRESENT_PARAMETERS, g_presentation_parameters);
REFERENCE_DECLARE(0x050DD960, D3DPRESENT_PARAMETERS, g_new_presentation_parameters);

bool render_debug_force_4x3_aspect_ratio = false;

int32 __cdecl get_current_thread_index()
{
	return INVOKE(0x0051C410, get_current_thread_index);

	//return g_registered_thread_index;
}

D3DCAPS9* __cdecl get_global_device_caps()
{
	return INVOKE(0x00A1FC30, get_global_device_caps);
}

D3DDISPLAYMODE* __cdecl get_global_display_mode()
{
	return INVOKE(0x00A1FC40, get_global_display_mode);
}

D3DPRESENT_PARAMETERS* __cdecl get_presentation_parameters()
{
	return INVOKE(0x00A1FC50, get_presentation_parameters);
}

D3DPRESENT_PARAMETERS* __cdecl get_new_presentation_parameters()
{
	return INVOKE(0x00A1FC60, get_new_presentation_parameters);
}

bool __cdecl rasterizer_force_default_lighting()
{
	return INVOKE(0x00A20150, rasterizer_force_default_lighting);
}

