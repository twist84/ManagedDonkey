#include "interface/first_person_weapons.hpp"

#include "animations/animation_definitions.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"

HOOK_DECLARE_CALL(0x00A45360, first_person_weapon_build_models);

REFERENCE_DECLARE(0x0526070E, bool, debug_animation_fp_sprint_disable);

bool debug_first_person_skeleton = false;

//.text:00A99610 ; void __cdecl __tls_set_g_first_person_weapon_orientations_allocator(void*)
//.text:00A99630 ; void __cdecl __tls_set_g_first_person_weapons_allocator(void*)
//.text:00A99650 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_first_person_weapon_orientations_allocator(void*)>::allocate
//.text:00A99690 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_first_person_weapons_allocator(void*)>::allocate
//.text:00A996D0 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_first_person_weapon_orientations_allocator(void*)>::deallocate
//.text:00A99700 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_first_person_weapons_allocator(void*)>::deallocate
//.text:00A99730 ; bool __cdecl first_person_claw_lower_weapon_on_unit(int32, int32)
//.text:00A997A0 ; bool __cdecl first_person_claw_raise_weapon_on_unit(int32, int32)
//.text:00A99800 ; bool __cdecl first_person_custom_animation_running_on_unit(int32)
//.text:00A99860 ; void __cdecl first_person_end_custom_animation(int32)
//.text:00A998A0 ; void __cdecl first_person_end_custom_animation_on_unit(int32)
//.text:00A99900 ; s_weapon_orientations* __cdecl first_person_get_weapon_orientations(int32, int32)
//.text:00A99930 ; bool __cdecl first_person_handle_lowering_weapon_on_unit(int32, int32)
//.text:00A99A20 ; bool __cdecl first_person_handle_raising_weapon(int32, int32)
//.text:00A99B70 ; bool __cdecl first_person_handle_unit_death(int32)
//.text:00A99BF0 ; bool __cdecl first_person_handle_weapon_down_state(int32)
//.text:00A99D10 ; bool __cdecl first_person_handle_weapon_lowering_complete(int32)
//.text:00A99E70 ; void __cdecl first_person_primary_animation_event_callback(const s_animation_event_data*, int32)
//.text:00A99E90 ; bool __cdecl first_person_scripting_lower_weapon_on_unit(int32, int32)
//.text:00A99F00 ; bool __cdecl first_person_scripting_raise_weapon_on_unit(int32, int32)
//.text:00A99F60 ; void __cdecl first_person_secondary_animation_event_callback(const s_animation_event_data*, int32)
//.text:00A99F80 ; bool __cdecl first_person_setup_custom_animation_on_unit(int32, int32, int32, bool)
//.text:00A9A050 ; bool __cdecl first_person_weapon_adjust_light(int32, int32, real_point3d*, real_vector3d*, real_vector3d*)
//.text:00A9A190 ; void __cdecl first_person_weapon_apply_camera_effect(int32, real_matrix4x3*)
//.text:00A9A1E0 ; bool __cdecl first_person_weapon_attach(int32, int32, bool)
//.text:00A9A4B0 ; void __cdecl first_person_weapon_attach_weapon_slot(int32, int32, const first_person_weapon_attachment*, bool)
//.text:00A9AA40 ; void __cdecl first_person_weapon_build_model(int32, int32, uns32, const real_matrix4x3*, int32, const real_matrix4x3*, const int32*, render_first_person_model*)

int32 __cdecl first_person_weapon_build_models(int32 user_index, int32 object_index, int32 maximum_model_count, render_first_person_model* first_person_model)
{
	int32 model_count = INVOKE(0x00A9AAE0, first_person_weapon_build_models, user_index, object_index, maximum_model_count, first_person_model);

	if (debug_first_person_skeleton)
	{
		first_person_weapon* weapon = first_person_weapon_get(user_index);
		first_person_weapon_data* weapon_data = first_person_weapon_get_weapon_data(weapon, 0);

		if (weapon_data->animation_manager.valid_graph())
		{
			for (int32 node_index = 0; node_index < weapon_data->node_matrices_count; node_index++)
			{
				const c_model_animation_graph* graph = weapon_data->animation_manager.get_graph();
				s_animation_graph_node* node = graph->get_node(node_index);

				real_matrix4x3 node_matrix{};
				matrix4x3_multiply(&weapon->estimated_root_matrix, &weapon_data->node_matrices[node_index], &node_matrix);
				render_debug_matrix(true, &node_matrix, 0.01f);

				if (node->parent_node_index != NONE)
				{
					real_matrix4x3 parent_node_matrix{};
					matrix4x3_multiply(&weapon->estimated_root_matrix, &weapon_data->node_matrices[node->parent_node_index], &parent_node_matrix);
					render_debug_line(true, &node_matrix.position, &parent_node_matrix.position, global_real_argb_white);
				}
			}
		}
	}

	ASSERT(model_count <= maximum_model_count);

	return model_count;
}

//.text:00A9B010 ; void __cdecl first_person_weapon_build_node_matrices(int32, int32)
//.text:00A9B9F0 ; void __cdecl first_person_weapon_clear_animations(int32, int32)
//.text:00A9BA80 ; void __cdecl first_person_weapon_detach_unit(int32)
//.text:00A9BAA0 ; int32 __cdecl first_person_weapon_find_first_output_user_index_from_unit(int32)
//.text:00A9BAE0 ; int32 __cdecl first_person_weapon_find_first_output_user_index_from_weapon(int32)
//.text:00A9BB40 ; int32 __cdecl first_person_weapon_find_next_output_user_index_from_unit(int32, int32)
//.text:00A9BB90 ; int32 __cdecl first_person_weapon_find_next_output_user_index_from_weapon(int32, int32)

first_person_weapon* __cdecl first_person_weapon_get(int32 user_index)
{
	//return INVOKE(0x00A9BC00, first_person_weapon_get, user_index);

	ASSERT(VALID_INDEX(user_index, 4));

	return &first_person_weapons[user_index];
}

//.text:00A9BC30 ; const real_matrix4x3* __cdecl first_person_weapon_get_camera_offset_matrix(int32)
//.text:00A9BC70 ; const real_matrix4x3* __cdecl first_person_weapon_get_camera_relative_node_matrix(int32, int32, int16)
//.text:00A9BCE0 ; int16 __cdecl first_person_weapon_get_markers_by_string_id(int32, int32, object_marker*, int16)
//.text:00A9BE40 ; int16 __cdecl first_person_weapon_get_markers_by_string_id_render(int32, int32, int32, object_marker*, int16)
//.text:00A9BE70 ; int32 __cdecl first_person_weapon_get_non_deterministic_unit_index_from_user(int32)
//.text:00A9BEB0 ; bool __cdecl first_person_weapon_get_render_info(int32, s_first_person_render_info*)

first_person_weapon_data* __cdecl first_person_weapon_get_weapon_data(first_person_weapon* fp_weapon, int32 weapon_slot)
{
	//INVOKE(0x00A9BF20, first_person_weapon_get_weapon_data, fp_weapon, weapon_slot);

	return &fp_weapon->weapon[weapon_slot];
}

//.text:00A9BF40 ; int32 __cdecl first_person_weapon_get_weapon_slot(int32, int32)
//.text:00A9BF90 ; void __cdecl first_person_weapon_get_worldspace_node_matrix(int32, int32, int16, real_matrix4x3*)
//.text:00A9C010 ; void __cdecl first_person_weapon_handle_message(int32, int32, e_unit_messages, int32)
//.text:00A9C170 ; bool __cdecl first_person_weapon_is_visible(int32, int32)
//.text:00A9C1D0 ; int32 __cdecl first_person_weapon_find_first_output_user_index_from_unit(int32)
//.text:00A9C240 ; void __cdecl first_person_weapon_new_unit(int32, int32)
//.text:00A9C260 ; void __cdecl first_person_weapon_next_state(int32, int32)

void __cdecl first_person_weapon_perspective_changed(int32 user_index)
{
	INVOKE(0x00A9C550, first_person_weapon_perspective_changed, user_index);
}

//.text:00A9C630 ; void __cdecl first_person_weapon_reset_interpolation(int32, int32)
//.text:00A9C670 ; void __cdecl first_person_weapon_reset_sound(int32, int32)
//.text:00A9C6D0 ; void __cdecl first_person_weapon_set_attached(int32, int32, bool)
//.text:00A9C730 ; bool __cdecl first_person_weapon_set_overlay(int32, int32, int32)
//.text:00A9C850 ; bool __cdecl first_person_weapon_set_state(int32, int32, int32, bool)
//.text:00A9D0B0 ; void __cdecl first_person_weapon_set_visibility(int32, int32, bool)
//.text:00A9D110 ; void __cdecl first_person_weapon_setup_animations(int32, int32, bool)
//.text:00A9D300 ; void __cdecl first_person_weapon_start_interpolation(int32, int32, real32)
//.text:00A9D3D0 ; int32 __cdecl first_person_weapon_state_from_message(int16)
//.text:00A9D5E0 ; bool __cdecl first_person_weapon_state_makes_weapon_busy(int32)
//.text:00A9D6E0 ; void __cdecl first_person_weapon_suspend_weapon_attachment(int32, bool)
//.text:00A9D7A0 ; void __cdecl first_person_weapon_update(int32)
//.text:00A9E2D0 ; void __cdecl first_person_weapon_update_movement_animation(int32, int32)
//.text:00A9E600 ; void __cdecl first_person_weapon_update_overlay_animation(int32, int32)
//.text:00A9E760 ; 
//.text:00A9E770 ; bool __cdecl first_person_weapon_update_visibility(int32, int32)
//.text:00A9E830 ; bool __cdecl first_person_weapon_validate_output_user_mask_for_weapon_index(int32, int32)
//.text:00A9E880 ; bool __cdecl first_person_weapon_validate_weapon_index(int32, int32)
//.text:00A9E8C0 ; void __cdecl first_person_weapons_dispose()
//.text:00A9E8D0 ; void __cdecl first_person_weapons_dispose_from_old_map()
//.text:00A9E8F0 ; void __cdecl first_person_weapons_handle_deleted_object(int32)
//.text:00A9E9D0 ; void __cdecl first_person_weapons_initialize()
//.text:00A9EA70 ; void __cdecl first_person_weapons_initialize_for_new_map()
//.text:00A9EBE0 ; void __cdecl first_person_weapons_reset_animations()

void __cdecl first_person_weapons_update()
{
	INVOKE(0x00A9ED20, first_person_weapons_update);
}

//.text:00A9ED50 ; void __cdecl first_person_weapons_update_camera_estimate(int32)

void __cdecl first_person_weapons_update_camera_estimates()
{
	INVOKE(0x00A9EDF0, first_person_weapons_update_camera_estimates);
}

//.text:00A9EEA0 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_first_person_weapon_orientations_allocator(void*)>::free_memory
//.text:00A9EED0 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_first_person_weapons_allocator(void*)>::free_memory
//.text:00A9EF00 ; 
//.text:00A9EF10 ; 
//.text:00A9EF20 ; void __cdecl handle_first_person_animation_event_callback(int32, const s_animation_event_data*, int32)
//.text:00A9F0F0 ; void __cdecl model_remap_node_matrices_to_match_animation_graph(int32, const real_matrix4x3*, real_matrix4x3*, int32, const real_matrix4x3*, const int32*)
//.text:00A9F170 ; void __cdecl process_first_person_weapon_ik(int32, render_first_person_model*, const render_first_person_model*)
//.text:00A9F370 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_first_person_weapon_orientations_allocator(void*)>::reserve_memory
//.text:00A9F3B0 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_first_person_weapons_allocator(void*)>::reserve_memory
//.text:00A9F3F0 ; const c_model_animation* __cdecl weapon_get_deterministic_first_person_animation(int32, int32, int32*)
//.text:00A9F580 ; int16 __cdecl weapon_get_deterministic_first_person_animation_duration_ticks(int32, int32, int16)
//.text:00A9F6B0 ; int32 __cdecl weapon_get_deterministic_first_person_animation_from_message(int32, int16)

int32 first_person_weapon_get_current_state_string(int32 unit_index, int32 weapon_slot)
{
	for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
	{
		struct first_person_weapon* first_person_weapon = first_person_weapon_get(user_index);
		if (first_person_weapon->unit_index == unit_index)
		{
			return first_person_weapon_get_weapon_data(first_person_weapon, weapon_slot)->animation_manager.get_state_name();
		}
	}

	return _string_id_invalid;
}

int32 first_person_weapon_get_pending_state_string(int32 unit_index, int32 weapon_slot)
{
	for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
	{
		struct first_person_weapon* first_person_weapon = first_person_weapon_get(user_index);
		if (first_person_weapon->unit_index == unit_index)
		{
			return first_person_weapon_get_weapon_data(first_person_weapon, weapon_slot)->pending_state;
		}
	}

	return _string_id_invalid;
}

