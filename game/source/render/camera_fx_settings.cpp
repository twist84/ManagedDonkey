#include "render/camera_fx_settings.hpp"

void __cdecl s_scripted_exposure::dispose()
{
	INVOKE(0x00A2A7E0, dispose);
}

void __cdecl s_scripted_exposure::dispose_from_old_map()
{
	INVOKE(0x00A2A7F0, dispose_from_old_map);
}

void __cdecl s_scripted_exposure::initialize()
{
	INVOKE(0x00A2AA90, initialize);
}

void __cdecl s_scripted_exposure::initialize_for_new_map()
{
	INVOKE(0x00A2AAF0, initialize_for_new_map);
}

void __cdecl scripted_exposure_update(real game_seconds_elapsed)
{
	INVOKE(0x00A2AF70, scripted_exposure_update, game_seconds_elapsed);
}

