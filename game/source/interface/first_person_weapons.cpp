#include "interface/first_person_weapons.hpp"

#include "cseries/cseries.hpp"
#include "memory/thread_local.hpp"

REFERENCE_DECLARE(0x0526070E, bool, debug_animation_fp_sprint_disable);

//.text:00A99610 ; void __cdecl __tls_set_g_first_person_weapon_orientations_allocator(void*)
//.text:00A99630 ; void __cdecl __tls_set_g_first_person_weapons_allocator(void*)
//.text:00A99650 ; 
//.text:00A99690 ; 
//.text:00A996D0 ; 
//.text:00A99700 ; 
//.text:00A99730 ; bool __cdecl first_person_claw_lower_weapon_on_unit(long, long)
//.text:00A997A0 ; bool __cdecl first_person_claw_raise_weapon_on_unit(long, long)
//.text:00A99800 ; 
//.text:00A99860 ; 
//.text:00A998A0 ; 
//.text:00A99900 ; s_weapon_orientations* __cdecl first_person_get_weapon_orientations(e_output_user_index, long)
//.text:00A99930 ; bool __cdecl first_person_handle_lowering_weapon_on_unit(e_output_user_index, long)
//.text:00A99A20 ; bool __cdecl first_person_handle_raising_weapon(e_output_user_index, long)
//.text:00A99B70 ; bool __cdecl first_person_handle_unit_death(long)
//.text:00A99BF0 ; bool __cdecl first_person_handle_weapon_down_state(e_output_user_index)
//.text:00A99D10 ; bool __cdecl first_person_handle_weapon_lowering_complete(e_output_user_index)
//.text:00A99E70 ; void __cdecl first_person_primary_animation_event_callback(s_animation_event_data const*, long)
//.text:00A99E90 ; bool __cdecl first_person_scripting_lower_weapon_on_unit(long, long)
//.text:00A99F00 ; bool __cdecl first_person_scripting_raise_weapon_on_unit(long, long)
//.text:00A99F60 ; void __cdecl first_person_secondary_animation_event_callback(s_animation_event_data const*, long)
//.text:00A99F80 ; bool __cdecl first_person_setup_custom_animation_on_unit(long, long, long, bool)
//.text:00A9A050 ; bool __cdecl first_person_weapon_adjust_light(long, long, real_point3d*, vector3d*, vector3d*)
//.text:00A9A190 ; void __cdecl first_person_weapon_apply_camera_effect(e_output_user_index, real_matrix4x3*)
//.text:00A9A1E0 ; bool __cdecl first_person_weapon_attach(e_output_user_index, long, bool)
//.text:00A9A4B0 ; void __cdecl first_person_weapon_attach_weapon_slot(e_output_user_index, long, first_person_weapon_attachment const*, bool)
//.text:00A9AA40 ; 
//.text:00A9AAE0 ; long __cdecl first_person_weapon_build_models(e_output_user_index, long, long, render_first_person_model*)
//.text:00A9B010 ; void __cdecl first_person_weapon_build_node_matrices(e_output_user_index, long)
//.text:00A9B9F0 ; void __cdecl first_person_weapon_clear_animations(e_output_user_index, long)
//.text:00A9BA80 ; void __cdecl first_person_weapon_detach_unit(e_output_user_index)
//.text:00A9BAA0 ; 
//.text:00A9BAE0 ; e_output_user_index __cdecl first_person_weapon_find_first_output_user_index_from_weapon(long)
//.text:00A9BB40 ; 
//.text:00A9BB90 ; e_output_user_index __cdecl first_person_weapon_find_next_output_user_index_from_weapon(long, e_output_user_index)

first_person_weapon* __cdecl first_person_weapon_get(e_output_user_index output_user_index)
{
	//return INVOKE(0x00A9BC00, first_person_weapon_get, output_user_index);

	TLS_DATA_GET_VALUE_REFERENCE(first_person_weapons);

	ASSERT(VALID_INDEX(output_user_index, k_number_of_output_users));

	return &first_person_weapons[output_user_index];
}

//.text:00A9BC30 ; 
//.text:00A9BC70 ; real_matrix4x3 const* __cdecl first_person_weapon_get_camera_relative_node_matrix(e_output_user_index, long, short)
//.text:00A9BCE0 ; short __cdecl first_person_weapon_get_markers_by_string_id(long, long, object_marker*, short)
//.text:00A9BE40 ; short __cdecl first_person_weapon_get_markers_by_string_id_render(e_output_user_index, long, long, object_marker*, short)
//.text:00A9BE70 ; 
//.text:00A9BEB0 ; bool __cdecl first_person_weapon_get_render_info(e_output_user_index, s_first_person_render_info*)

first_person_weapon_data* __cdecl first_person_weapon_get_weapon_data(first_person_weapon* fp_weapon, long weapon_slot)
{
	//INVOKE(0x00A9BF20, first_person_weapon_get_weapon_data, fp_weapon, weapon_slot);

	return &fp_weapon->weapon_slots[weapon_slot];
}

//.text:00A9BF40 ; long __cdecl first_person_weapon_get_weapon_slot(long, long)
//.text:00A9BF90 ; void __cdecl first_person_weapon_get_worldspace_node_matrix(e_output_user_index, long, short, real_matrix4x3*)
//.text:00A9C010 ; void __cdecl first_person_weapon_handle_message(long, long, e_unit_messages, long)
//.text:00A9C170 ; 
//.text:00A9C1D0 ; 
//.text:00A9C240 ; void __cdecl first_person_weapon_new_unit(e_output_user_index, long)
//.text:00A9C260 ; void __cdecl first_person_weapon_next_state(e_output_user_index, long)

void __cdecl first_person_weapon_perspective_changed(e_output_user_index output_user_index)
{
	INVOKE(0x00A9C550, first_person_weapon_perspective_changed, output_user_index);
}

//.text:00A9C630 ; 
//.text:00A9C670 ; void __cdecl first_person_weapon_reset_sound(e_output_user_index, long)
//.text:00A9C6D0 ; void __cdecl first_person_weapon_set_attached(e_output_user_index, long, bool)
//.text:00A9C730 ; bool __cdecl first_person_weapon_set_overlay(e_output_user_index, long, long)
//.text:00A9C850 ; bool __cdecl first_person_weapon_set_state(e_output_user_index, long, long, bool)
//.text:00A9D0B0 ; 
//.text:00A9D110 ; void __cdecl first_person_weapon_setup_animations(e_output_user_index, long, bool)
//.text:00A9D300 ; void __cdecl first_person_weapon_start_interpolation(e_output_user_index, long, real)
//.text:00A9D3D0 ; long __cdecl first_person_weapon_state_from_message(short)
//.text:00A9D5E0 ; bool __cdecl first_person_weapon_state_makes_weapon_busy(long)
//.text:00A9D6E0 ; void __cdecl first_person_weapon_suspend_weapon_attachment(long, bool)
//.text:00A9D7A0 ; void __cdecl first_person_weapon_update(e_output_user_index)
//.text:00A9E2D0 ; void __cdecl first_person_weapon_update_movement_animation(e_output_user_index, long)
//.text:00A9E600 ; void __cdecl first_person_weapon_update_overlay_animation(e_output_user_index, long)
//.text:00A9E760 ; 
//.text:00A9E770 ; bool __cdecl first_person_weapon_update_visibility(e_output_user_index, long)
//.text:00A9E830 ; 
//.text:00A9E880 ; bool __cdecl first_person_weapon_validate_weapon_index(long, long)
//.text:00A9E8C0 ; void __cdecl first_person_weapons_dispose()
//.text:00A9E8D0 ; void __cdecl first_person_weapons_dispose_from_old_map()
//.text:00A9E8F0 ; void __cdecl first_person_weapons_handle_deleted_object(long)
//.text:00A9E9D0 ; void __cdecl first_person_weapons_initialize()
//.text:00A9EA70 ; void __cdecl first_person_weapons_initialize_for_new_map()
//.text:00A9EBE0 ; 

void __cdecl first_person_weapons_update()
{
	INVOKE(0x00A9ED20, first_person_weapons_update);
}

//.text:00A9ED50 ; void __cdecl first_person_weapons_update_camera_estimate(e_output_user_index)

void __cdecl first_person_weapons_update_camera_estimates()
{
	INVOKE(0x00A9EDF0, first_person_weapons_update_camera_estimates);
}

//.text:00A9EEA0 ; 
//.text:00A9EED0 ; 
//.text:00A9EF00 ; 
//.text:00A9EF10 ; 
//.text:00A9EF20 ; void __cdecl handle_first_person_animation_event_callback(e_output_user_index, s_animation_event_data const*, long)
//.text:00A9F0F0 ; 
//.text:00A9F170 ; 
//.text:00A9F370 ; 
//.text:00A9F3B0 ; 
//.text:00A9F3F0 ; c_model_animation const* __cdecl weapon_get_deterministic_first_person_animation(long, long, long*)
//.text:00A9F580 ; short __cdecl weapon_get_deterministic_first_person_animation_duration_ticks(long, long, short)
//.text:00A9F6B0 ; long __cdecl weapon_get_deterministic_first_person_animation_from_message(long, short)

long first_person_weapon_get_current_state_string(long unit_index, long weapon_slot)
{
	for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	{
		struct first_person_weapon* first_person_weapon = first_person_weapon_get(user_index);
		if (first_person_weapon->unit_index == unit_index)
		{
			//return first_person_weapon_get_weapon_data(first_person_weapon, weapon_slot)->animation_manager.get_state_name();
			return first_person_weapon_get_weapon_data(first_person_weapon, weapon_slot)->current_state_string;
		}
	}

	return _string_id_invalid;
}

long first_person_weapon_get_pending_state_string(long unit_index, long weapon_slot)
{
	for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	{
		struct first_person_weapon* first_person_weapon = first_person_weapon_get(user_index);
		if (first_person_weapon->unit_index == unit_index)
		{
			return first_person_weapon_get_weapon_data(first_person_weapon, weapon_slot)->pending_state_string;
		}
	}

	return _string_id_invalid;
}

