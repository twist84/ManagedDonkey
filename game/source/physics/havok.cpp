#include "physics/havok.hpp"

#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/terminal.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"

REFERENCE_DECLARE(0x018A2324, s_havok_constants, g_havok_constants);
REFERENCE_DECLARE(0x018A2370, s_havok_globals, g_havok_globals);
REFERENCE_DECLARE(0x04B1DA80, bool, g_havok_memory_always_system);

HOOK_DECLARE(0x005C5520, havok_display_stats_printf);

void __cdecl havok_can_modify_state_allow()
{
	INVOKE(0x005C45C0, havok_can_modify_state_allow);
}

void __cdecl havok_can_modify_state_disallow()
{
	INVOKE(0x005C45D0, havok_can_modify_state_disallow);
}

void __cdecl havok_display_stats(bool display_as_event)
{
	INVOKE(0x005C4FC0, havok_display_stats, display_as_event);
}

void __cdecl havok_display_stats_printf(bool display_as_event, real_argb_color const* color, char const* format, ...)
{
	va_list list;
	va_start(list, format);

	c_static_string<255> string;
	string.print(format, list);

	va_end(list);

	// missing in Halo Online
	if (display_as_event)
		generate_event(_event_level_warning, "%s", string.get_string());
	else
		terminal_printf(NULL, "%s", string.get_string());
}

void __cdecl havok_prepare_fpu_for_update()
{
	INVOKE(0x005C7890, havok_prepare_fpu_for_update);

	//TLS_DATA_GET_VALUE_REFERENCE(havok_style_fpu_exceptions_count);
	//ASSERT(havok_style_fpu_exceptions_count >= 0);
	//
	//if (!havok_style_fpu_exceptions_count)
	//    //_controlfp(_MCW_EM | _PC_53, _MCW_EM);
	//    _controlfp(0x9001F, 0x8001F);
	//
	//havok_style_fpu_exceptions_count++;
}

void __cdecl havok_restore_fpu_from_update()
{
	INVOKE(0x005C7A30, havok_restore_fpu_from_update);

	//TLS_DATA_GET_VALUE_REFERENCE(havok_style_fpu_exceptions_count);
	//ASSERT(havok_style_fpu_exceptions_count > 0);
	//
	//if (!--havok_style_fpu_exceptions_count)
	//{
	//    _clearfp();
	//    real_math_reset_precision();
	//}
}

void __cdecl havok_update()
{
	INVOKE(0x005C7B50, havok_update);
}

s_havok_constants const* get_havok_constants()
{
	return &g_havok_constants;
}

void havok_debug_render()
{
	return;

	bool set_single_thread_request_flag = debug_objects
		&& (debug_objects_physics_models
			|| debug_objects_water_physics
			|| debug_objects_contact_points
			|| debug_objects_constraints
			|| debug_objects_vehicle_physics
			|| debug_objects_mass)
		|| get_havok_constants()->display_stats
		|| get_havok_constants()->impacts_debug
		|| get_havok_constants()->proxies_debug
		|| get_havok_constants()->collision_damage_debug
		|| get_havok_constants()->physics_debug;

	// this will freeze comment for now
	//main_set_single_thread_request_flag(1, set_single_thread_request_flag);

	if (restricted_region_locked_for_current_thread(k_game_state_update_region))
	{
		havok_prepare_fpu_for_update();

		if (get_havok_constants()->display_stats)
			havok_display_stats(false);

		//if (get_havok_constants()->impacts_debug)
		//    impacts_render_debug(); // #TODO: implement
		//
		//if (get_havok_constants()->proxies_debug)
		//    havok_proxies_render_debug(); // #TODO: implement
		//
		//if (get_havok_constants()->collision_damage_debug)
		//    havok_collision_damage_render_debug(); // #TODO: implement
		//
		//if (get_havok_constants()->physics_debug)
		//{
		//    // #TODO: implement
		//}

		havok_restore_fpu_from_update();
	}
}

