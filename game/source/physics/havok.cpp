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

//.text:005C3CE0 ; hkRigidBody const* __cdecl get_havok_world_fixed_rigid_body()
//.text:005C3CF0 ; 
//.text:005C3D00 ; public: short const __cdecl c_havok_component::get_object_postion_controlling_rigid_body_index() const
//.text:005C3D20 ; 
//.text:005C3D30 ; 
//.text:005C3D50 ; 
//.text:005C3D60 ; 
//.text:005C3D70 ; 
//.text:005C3D80 ; 
//.text:005C3D90 ; void __cdecl havok_add_environment_rigid_bodies_to_world()
//.text:005C3E80 ; bool __cdecl havok_all_entities_in_island_contain_players(hkpSimulationIsland const*)
//.text:005C3EE0 ; void __cdecl havok_base_system_init()
//.text:005C3FC0 ; void __cdecl havok_base_system_quit()
//.text:005C3FE0 ; void __cdecl havok_batch_add_entities()
//.text:005C4300 ; void __cdecl havok_build_environment_surface_mopp(long)
//.text:005C44E0 ; c_environment_mopp_bv_tree_shape* __cdecl havok_build_mopp(c_environment_surface_shape*)
//.text:005C4560 ; bool __cdecl havok_can_allocate_space_for_instance_of_object_definition(long)
//.text:005C4590 ; bool __cdecl havok_can_modify_state()

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
	//0x005C5520

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

//.text:005C5550 ; void __cdecl havok_dispose()
//.text:005C5590 ; 
//.text:005C55A0 ; void __cdecl havok_dispose_from_old_map()
//.text:005C55C0 ; void __cdecl havok_dispose_from_old_structure_bsp(dword)
//.text:005C55D0 ; void __cdecl havok_dispose_from_old_structure_bsp_internal(dword)
//.text:005C5700 ; void __cdecl havok_entity_post_update(hkEntity*)
//.text:005C5AB0 ; 
//.text:005C5AD0 ; void __cdecl havok_error_stub(char const*, void*)
//.text:005C5AE0 ; void __cdecl havok_garbage_collection_lock(bool)
//.text:005C5AF0 ; bool __cdecl havok_garbage_collection_locked()
//.text:005C5B00 ; c_environment_mopp_bv_tree_shape const* __cdecl havok_get_environment_mopp_shape(long)
//.text:005C5B20 ; dword __cdecl havok_get_environment_shape_key_from_cd_body(hkCdBody const*)
//.text:005C5B60 ; c_environment_surface_shape const* __cdecl havok_get_environment_surface_shape(long)
//.text:005C5B90 ; hkThreadMemory* __cdecl havok_get_render_thread_memory()
//.text:005C5BA0 ; hkMonitorStream* __cdecl havok_get_render_thread_monitor_stream()
//.text:005C5BB0 ; void __cdecl havok_initialize()
//.text:005C5CF0 ; void __cdecl havok_initialize_all_allocated_state(bool)
//.text:005C5D40 ; void __cdecl havok_initialize_environment_rigid_bodies()
//.text:005C5DB0 ; void __cdecl havok_initialize_for_new_map()
//.text:005C5E00 ; void __cdecl havok_initialize_for_new_structure_bsp(dword)
//.text:005C5E10 ; void __cdecl havok_initialize_for_new_structure_bsp_internal(dword)
//.text:005C5EF0 ; void __cdecl havok_initialize_havok_world()
//.text:005C6240 ; void __cdecl havok_initialize_object_rigid_bodies()
//.text:005C6330 ; bool __cdecl havok_inside_simulation_step()
//.text:005C6340 ; long __cdecl havok_last_garbage_collection_state_reset_time_get()
//.text:005C6360 ; void __cdecl havok_last_garbage_collection_state_reset_time_set()
//.text:005C6380 ; long __cdecl havok_last_state_reset_time_get()
//.text:005C63A0 ; void __cdecl havok_load_from_game_state(long)
//.text:005C63F0 ; void __cdecl havok_make_space_for_connected_rigid_bodies(long)
//.text:005C67C0 ; void __cdecl havok_monitor_stream_data_deallocate(void*)
//.text:005C67D0 ; 
//.text:005C67E0 ; bool __cdecl havok_must_rebuild_environtment_when_tag_of_this_group_changes(long)
//.text:005C68C0 ; void* __cdecl havok_next_monitor_stream_data_allocate()
//.text:005C68E0 ; 
//.text:005C68F0 ; 
//.text:005C6900 ; void __cdecl havok_notify_object_changed_region_permutation(long, long, char const*)
//.text:005C6AF0 ; void __cdecl havok_notify_static_geometry_modfied(long, long, long, long, long)
//.text:005C6B60 ; void __cdecl havok_notify_thread_frame_finished(long)
//.text:005C6BA0 ; void __cdecl havok_notify_thread_frame_started(long)
//.text:005C6BF0 ; void __cdecl havok_object_activate(long)
//.text:005C6C90 ; bool __cdecl havok_object_apply_acceleration(long, long, real_point3d const*, vector3d const*, vector3d const*, long*, vector3d*, vector3d*)
//.text:005C6F70 ; void __cdecl havok_object_deactivate(long)
//.text:005C7000 ; void __cdecl havok_object_delete(long)
//.text:005C7080 ; void __cdecl havok_object_delete_internal(long)
//.text:005C70D0 ; void __cdecl havok_object_delete_recursive(long)
//.text:005C7130 ; void __cdecl havok_object_delete_recursive(long, long)
//.text:005C71D0 ; void __cdecl havok_object_new(long)
//.text:005C72B0 ; void __cdecl havok_object_new_recursive(long)
//.text:005C72D0 ; void __cdecl havok_object_new_recursive(long, long)
//.text:005C7360 ; void __cdecl havok_object_rebuild_immediate(long)
//.text:005C7380 ; void __cdecl havok_object_rebuild_immediate(long, long)

void __cdecl havok_object_set_position(long object_index, bool a2, bool a3, bool a4)
{
	INVOKE(0x005C7410, havok_object_set_position, object_index, a2, a3, a4);
}

//.text:005C74F0 ; void __cdecl havok_object_set_velocities(long, vector3d const*, vector3d const*, bool)
//.text:005C77A0 ; void __cdecl havok_post_process_simulating_entities()

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

//.text:005C78D0 ; void __cdecl havok_register_all_agents(hkpCollisionDispatcher*)
//.text:005C7A00 ; void __cdecl havok_reset_allocated_state()

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

//.text:005C7A60 ; bool __cdecl havok_rigid_bodies_are_active()
//.text:005C7A70 ; void __cdecl havok_save_to_game_state(long)
//.text:005C7A90 ; void __cdecl havok_set_initial_simulation_island_activity()

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

