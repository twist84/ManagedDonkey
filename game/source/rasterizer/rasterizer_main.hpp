#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

struct s_rasterizer_game_states
{
	bool motion_blur_enabled;
	bool atmosphere_fog_enabled;
	bool patchy_fog_enabled;
	bool weather_enabled;
	bool cinematic_motion_blur;
	int32 weather_effect_indices[128];
};
static_assert(sizeof(s_rasterizer_game_states) == 0x208);

extern bool render_debug_force_4x3_aspect_ratio;

struct _D3DCAPS9;
struct _D3DDISPLAYMODE;
struct _D3DPRESENT_PARAMETERS_;

extern c_interlocked_long& g_thread_owning_device;
extern _D3DCAPS9& g_global_device_caps;
extern _D3DDISPLAYMODE& g_global_display_mode;
extern _D3DPRESENT_PARAMETERS_& g_presentation_parameters;
extern _D3DPRESENT_PARAMETERS_& g_new_presentation_parameters;

extern int32 __cdecl get_current_thread_index();
extern _D3DCAPS9* __cdecl get_global_device_caps();
extern _D3DDISPLAYMODE* __cdecl get_global_display_mode();
extern _D3DPRESENT_PARAMETERS_* __cdecl get_presentation_parameters();
extern _D3DPRESENT_PARAMETERS_* __cdecl get_new_presentation_parameters();
extern bool __cdecl rasterizer_force_default_lighting();

