#include "render/render_water.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x019146EE, bool, render_water_enabled);
REFERENCE_DECLARE(0x019146F1, bool, render_water_tessellation_enabled);
REFERENCE_DECLARE(0x019146F2, bool, render_water_interaction_enabled);
REFERENCE_DECLARE(0x050FAB18, bool, render_water_wireframe_enabled);

void __cdecl c_water_renderer::dispose()
{
	INVOKE(0x00A35AE0, dispose);
}

void __cdecl c_water_renderer::dispose_from_old_map()
{
	INVOKE(0x00A35BA0, dispose_from_old_map);
}

void __cdecl c_water_renderer::game_update()
{
	INVOKE(0x00A35CB0, game_update);
}

void __cdecl c_water_renderer::initialize()
{
	INVOKE(0x00A35D70, initialize);
}

void __cdecl c_water_renderer::initialize_for_new_map()
{
	INVOKE(0x00A36220, initialize_for_new_map);
}

