#include "physics/havok.hpp"

#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/terminal.hpp"
#include "main/main.hpp"
#include "math/random_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "physics/havok_component.hpp"
#include "physics/havok_entity_iterator.hpp"

#include <hkWorld.hpp>

REFERENCE_DECLARE(0x018A2324, s_havok_constants, g_havok_constants);
REFERENCE_DECLARE(0x018A2370, s_havok_globals, g_havok_globals);
REFERENCE_DECLARE(0x02443FFC, hkWorld*, g_havok_world);
REFERENCE_DECLARE(0x04B1DA80, bool, g_havok_memory_always_system);

HOOK_DECLARE(0x005C5520, havok_display_stats_printf);

const s_havok_constants* get_havok_constants()
{
	return &g_havok_constants;
}

//.text:005C3CE0 ; const hkRigidBody* __cdecl get_havok_world_fixed_rigid_body()
//.text:005C3CF0 ; 
//.text:005C3D00 ; public: int16 const __cdecl c_havok_component::get_object_postion_controlling_rigid_body_index() const
//.text:005C3D20 ; 
//.text:005C3D30 ; 
//.text:005C3D50 ; 
//.text:005C3D60 ; 
//.text:005C3D70 ; 
//.text:005C3D80 ; 
//.text:005C3D90 ; void __cdecl havok_add_environment_rigid_bodies_to_world()
//.text:005C3E80 ; bool __cdecl havok_all_entities_in_island_contain_players(const hkpSimulationIsland*)
//.text:005C3EE0 ; void __cdecl havok_base_system_init()
//.text:005C3FC0 ; void __cdecl havok_base_system_quit()
//.text:005C3FE0 ; void __cdecl havok_batch_add_entities()
//.text:005C4300 ; void __cdecl havok_build_environment_surface_mopp(int32)
//.text:005C44E0 ; c_environment_mopp_bv_tree_shape* __cdecl havok_build_mopp(c_environment_surface_shape*)
//.text:005C4560 ; bool __cdecl havok_can_allocate_space_for_instance_of_object_definition(int32)
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

void __cdecl havok_display_stats_printf(bool display_as_event, const real_argb_color* color, const char* format, ...)
{
	//0x005C5520

	va_list arglist;
	va_start(arglist, format);

	char buffer[255]{};
	cvsnzprintf(buffer, sizeof(buffer), format, arglist);

	va_end(arglist);

	if (display_as_event)
	{
		event(_event_warning, "%s", buffer);
	}
	else
	{
		terminal_printf(NULL, "%s", buffer);
	}
}

void __cdecl havok_dispose()
{
	INVOKE(0x005C5550, havok_dispose);
}

//.text:005C5590 ; void __cdecl havok_dispose_all_allocated_state()

void __cdecl havok_dispose_from_old_map()
{
	INVOKE(0x005C55A0, havok_dispose_from_old_map);
}

void __cdecl havok_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x005C55C0, havok_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

//.text:005C55D0 ; void __cdecl havok_dispose_from_old_structure_bsp_internal(uns32)
//.text:005C5700 ; void __cdecl havok_entity_post_update(hkEntity*)
//.text:005C5AB0 ; bool __cdecl havok_environment_bodies_initialized()
//.text:005C5AD0 ; void __cdecl havok_error_stub(const char*, void*)
//.text:005C5AE0 ; void __cdecl havok_garbage_collection_lock(bool)
//.text:005C5AF0 ; bool __cdecl havok_garbage_collection_locked()
//.text:005C5B00 ; const c_environment_mopp_bv_tree_shape* __cdecl havok_get_environment_mopp_shape(int32)
//.text:005C5B20 ; uns32 __cdecl havok_get_environment_shape_key_from_cd_body(const hkCdBody*)
//.text:005C5B60 ; const c_environment_surface_shape* __cdecl havok_get_environment_surface_shape(int32)
//.text:005C5B90 ; hkThreadMemory* __cdecl havok_get_render_thread_memory()
//.text:005C5BA0 ; hkMonitorStream* __cdecl havok_get_render_thread_monitor_stream()

void __cdecl havok_initialize()
{
	INVOKE(0x005C5BB0, havok_initialize);
}

//.text:005C5CF0 ; void __cdecl havok_initialize_all_allocated_state(bool)
//.text:005C5D40 ; void __cdecl havok_initialize_environment_rigid_bodies()

void __cdecl havok_initialize_for_new_map()
{
	INVOKE(0x005C5DB0, havok_initialize_for_new_map);
}

void __cdecl havok_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x005C5E00, havok_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:005C5E10 ; void __cdecl havok_initialize_for_new_structure_bsp_internal(uns32)
//.text:005C5EF0 ; void __cdecl havok_initialize_havok_world()
//.text:005C6240 ; void __cdecl havok_initialize_object_rigid_bodies()
//.text:005C6330 ; bool __cdecl havok_inside_simulation_step()
//.text:005C6340 ; int32 __cdecl havok_last_garbage_collection_state_reset_time_get()
//.text:005C6360 ; void __cdecl havok_last_garbage_collection_state_reset_time_set()
//.text:005C6380 ; int32 __cdecl havok_last_state_reset_time_get()
//.text:005C63A0 ; void __cdecl havok_load_from_game_state(int32)
//.text:005C63F0 ; void __cdecl havok_make_space_for_connected_rigid_bodies(int32)
//.text:005C67C0 ; void __cdecl havok_monitor_stream_data_deallocate(void*)
//.text:005C67D0 ; bool __cdecl havok_multithreading_enabled(void)
//.text:005C67E0 ; bool __cdecl havok_must_rebuild_environtment_when_tag_of_this_group_changes(int32)
//.text:005C68C0 ; void* __cdecl havok_next_monitor_stream_data_allocate()
//.text:005C68E0 ; void __cdecl havok_notify_mopp_object_created(int32, int32)
//.text:005C68F0 ; void __cdecl havok_notify_mopp_object_deleted(int32, int32)
//.text:005C6900 ; void __cdecl havok_notify_object_changed_region_permutation(int32, int32, const char*)
//.text:005C6AF0 ; void __cdecl havok_notify_static_geometry_modfied(int32, int32, int32, int32, int32)
//.text:005C6B60 ; void __cdecl havok_notify_thread_frame_finished(int32)
//.text:005C6BA0 ; void __cdecl havok_notify_thread_frame_started(int32)
//.text:005C6BF0 ; void __cdecl havok_object_activate(int32)
//.text:005C6C90 ; bool __cdecl havok_object_apply_acceleration(int32, int32, const real_point3d*, const real_vector3d*, const real_vector3d*, int32*, real_vector3d*, real_vector3d*)
//.text:005C6F70 ; void __cdecl havok_object_deactivate(int32)
//.text:005C7000 ; void __cdecl havok_object_delete(int32)
//.text:005C7080 ; void __cdecl havok_object_delete_internal(int32)
//.text:005C70D0 ; void __cdecl havok_object_delete_recursive(int32)
//.text:005C7130 ; void __cdecl havok_object_delete_recursive(int32, int32)
//.text:005C71D0 ; void __cdecl havok_object_new(int32)
//.text:005C72B0 ; void __cdecl havok_object_new_recursive(int32)
//.text:005C72D0 ; void __cdecl havok_object_new_recursive(int32, int32)
//.text:005C7360 ; void __cdecl havok_object_rebuild_immediate(int32)
//.text:005C7380 ; void __cdecl havok_object_rebuild_immediate(int32, int32)

void __cdecl havok_object_set_position(int32 object_index, bool a2, bool a3, bool a4)
{
	INVOKE(0x005C7410, havok_object_set_position, object_index, a2, a3, a4);
}

//.text:005C74F0 ; void __cdecl havok_object_set_velocities(int32, const real_vector3d*, const real_vector3d*, bool)
//.text:005C77A0 ; void __cdecl havok_post_process_simulating_entities()

void __cdecl havok_prepare_fpu_for_update()
{
	INVOKE(0x005C7890, havok_prepare_fpu_for_update);

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

	//ASSERT(havok_style_fpu_exceptions_count > 0);
	//
	//if (!--havok_style_fpu_exceptions_count)
	//{
	//    _clearfp();
	//    real_math_reset_precision();
	//}
}

//.text:005C7A60 ; bool __cdecl havok_rigid_bodies_are_active()
//.text:005C7A70 ; void __cdecl havok_save_to_game_state(int32)
//.text:005C7A90 ; void __cdecl havok_set_initial_simulation_island_activity()

void __cdecl havok_update()
{
#if 0
	if (g_havok_world)
	{
		//ASSERT(havok_can_modify_state());

		if (g_havok_constants.havok_jumping_beans)
		{
			havok_update_jumping_beans();
		}
	}
#endif

	INVOKE(0x005C7B50, havok_update);
}

void havok_update_jumping_beans()
{
	// $TODO potential memory leak due to `hkArray` has no destructor therefore no call to `releaseMemory` is ran

	c_havok_entity_iterator iterator(g_havok_world, c_havok_entity_iterator::_iteration_type_inactive_entities);
	hkArray<hkEntity*> entities;

	iterator.begin();
	while (iterator.next())
	{
		hkEntity* entity = iterator.get_entity();
		const int32 havok_component_index = havok_entity_get_havok_component_index(entity);
		if (havok_component_index != NONE)
		{
			const c_havok_component* havok_component = DATUM_TRY_AND_GET(g_havok_component_data, c_havok_component, havok_component_index);
			const object_header_datum* object_header = DATUM_TRY_AND_GET(object_header_data, const object_header_datum, havok_component->m_object_index);
			if (TEST_FLAG(object_header->flags, _object_header_active_bit))
			{
				entities.pushBack(&entity);
			}
		}
	}
	iterator.end();

	int32 entity_count = entities.getSize() & 0xFFFF;
	for (int32 entity_index = 0; entity_index < entity_count; entity_index++)
	{
		const hkEntity* entity = entities[entity_index];
		const int32 havok_component_index = havok_entity_get_havok_component_index(entity);
		const int32 rigid_body_index = havok_entity_get_havok_component_rigid_body_index(entity);
		if (havok_component_index != NONE && rigid_body_index != NONE)
		{
			c_havok_component* havok_component = DATUM_TRY_AND_GET(g_havok_component_data, c_havok_component, havok_component_index);

			real_vector3d acceleration{};
			set_real_vector3d(&acceleration,
				REAL_RANDOM_RANGE(get_random_seed_address(), "jumping beans x", -0.1f, 0.1f),
				REAL_RANDOM_RANGE(get_random_seed_address(), "jumping beans y", -0.1f, 0.1f),
				REAL_RANDOM_RANGE(get_random_seed_address(), "jumping beans z", 0.0f, 0.1f)
			);
			havok_component->rigid_body_apply_acceleration(rigid_body_index, &acceleration);
		}
	}
}

void havok_debug_render()
{
	bool set_single_thread_request_flag = debug_objects
		&& (debug_objects_physics_models
			|| debug_objects_water_physics
			|| debug_objects_contact_points
			|| debug_objects_constraints
			|| debug_objects_vehicle_physics
			|| debug_objects_mass)
		|| get_havok_constants()->havok_display_stats
		|| get_havok_constants()->havok_render_impacts
		|| get_havok_constants()->havok_render_proxies
		|| get_havok_constants()->havok_render_collision_damage
		|| get_havok_constants()->havok_render_shape_properties;

	// this will freeze comment for now
	//main_set_single_thread_request_flag(_single_thread_for_physics_debug, set_single_thread_request_flag);

	if (restricted_region_locked_for_current_thread(k_game_state_update_region))
	{
		havok_prepare_fpu_for_update();

		if (get_havok_constants()->havok_display_stats)
		{
			havok_display_stats(false);
		}

		//if (get_havok_constants()->havok_render_impacts)
		//{
		//	impacts_render_debug(); // $IMPLEMENT
		//}

		if (get_havok_constants()->havok_render_proxies)
		{
			havok_proxies_render_debug();
		}

		//if (get_havok_constants()->havok_render_collision_damage)
		//{
		//	havok_collision_damage_render_debug(); // $IMPLEMENT
		//}

		//if (get_havok_constants()->havok_render_shape_properties)
		//{
		//	// $IMPLEMENT
		//}

		havok_restore_fpu_from_update();
	}
}

