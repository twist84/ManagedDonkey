#include "objects/objects.hpp"

#include "cseries/cseries_console.hpp"
#include "memory/thread_local.hpp"
#include "physics/havok.hpp"

void __cdecl object_delete(long object_index)
{
	INVOKE(0x00B2CD10, object_delete, object_index);
}

real_point3d* __cdecl object_get_origin(long object_index, real_point3d* origin)
{
	return INVOKE(0x00B2E5A0, object_get_origin, object_index, origin);
}

long __cdecl object_get_ultimate_parent(long object_index)
{
	return INVOKE(0x00B2EAB0, object_get_ultimate_parent, object_index);
}

long c_object_iterator_base::get_index()
{
	return m_index;
}

void __cdecl c_object_iterator_base::object_iterator_begin_internal(dword_flags type_flags, dword header_mask, dword match_flags, long object_index)
{
	DECLFUNC(0x00B2F4B0, void, __thiscall, c_object_iterator_base*, dword_flags, dword, dword, long)(this, type_flags, header_mask, match_flags, object_index);
}

bool __cdecl c_object_iterator_base::object_iterator_next_internal()
{
	return DECLFUNC(0x00B2F500, bool, __thiscall, c_object_iterator_base*)(this);
}

bool __cdecl c_object_iterator_base::object_iterator_next_with_match_flags_internal()
{
	return DECLFUNC(0x00B2F5A0, bool, __thiscall, c_object_iterator_base*)(this);
}

object_datum* __cdecl c_object_iterator_base::get_datum_internal()
{
	return m_datum;
}

long __cdecl object_new(object_placement_data* placement_data)
{
	return INVOKE(0x00B30440, object_new, placement_data);
}

void __cdecl object_placement_data_new(object_placement_data* placement_data, long object_definition_index, long object_datum_index, s_damage_owner const* damage_owner)
{
	INVOKE(0x00B31590, object_placement_data_new, placement_data, object_definition_index, object_datum_index, damage_owner);
}

void __cdecl object_placement_data_set_location(object_placement_data* data, struct s_location const* location)
{
	INVOKE(0x00B31750, object_placement_data_set_location, data, location);
}

void __cdecl object_postprocess_node_matrices(long object_index)
{
	INVOKE(0x00B31770, object_postprocess_node_matrices, object_index);
}

void __cdecl object_pre_delete_recursive(long object_index, bool a2)
{
	INVOKE(0x00B31810, object_pre_delete_recursive, object_index, a2);
}

bool __cdecl object_predict(long object_index, bool low)
{
	return INVOKE(0x00B31890, object_predict, object_index, low);
}

bool __cdecl object_predict_all(long object_index)
{
	//return INVOKE(0x00B31910, object_predict_all, object_index);

	return object_predict(object_index, false);
}

bool __cdecl object_predict_low(long object_index)
{
	//return INVOKE(0x00B31930, object_predict_low, object_index);

	return object_predict(object_index, true);
}

void object_prepare_axis_vectors(long object_index, vector3d* forward, vector3d* up)
{
	INVOKE(0x00B31950, object_prepare_axis_vectors, object_index, forward, up);
}

//.text:00B31B80 ; void __cdecl object_queue_render_thread_message(long, enum e_object_render_thread_message_type, short)

void __cdecl object_reconnect_to_map(long object_index, bool a2, s_location const* location)
{
	INVOKE(0x00B31BD0, object_reconnect_to_map, object_index, a2, location);
}

void __cdecl object_reconnect_to_physics(long object_index)
{
	INVOKE(0x00B31E20, object_reconnect_to_physics, object_index);
}

void __cdecl object_register_scenario_object(long object_index)
{
	INVOKE(0x00B31E80, object_register_scenario_object, object_index);
}

void __cdecl object_reinitialize_from_placement(long object_index, object_placement_data* data, bool at_rest)
{
	INVOKE(0x00B31EF0, object_reinitialize_from_placement, object_index, data, at_rest);
}

//.text:00B320A0
//.text:00B32100
//.text:00B32130
//.text:00B32140

void __cdecl object_reset(long object_index)
{
	INVOKE(0x00B32160, object_reset, object_index);
}

void __cdecl object_reset_interpolation(long object_index)
{
	INVOKE(0x00B321A0, object_reset_interpolation, object_index);
}

//.text:00B321F0
//.text:00B32400
//.text:00B324A0 ; object_reverse_compute_node_orientations_from_matrices, 2 args
//.text:00B32540 ; object_reverse_compute_node_orientations_from_matrices, 5 args

void __cdecl object_set_always_active(long object_index, bool always_active)
{
	INVOKE(0x00B32730, object_set_always_active, object_index, always_active);
}

void __cdecl object_set_at_rest(long object_index, bool at_rest)
{
	INVOKE(0x00B327F0, object_set_at_rest, object_index, at_rest);
}

bool __cdecl object_set_base_change_color_by_index(long object_index, long color_index, real_rgb_color const* color)
{
	return INVOKE(0x00B328F0, object_set_base_change_color_by_index, object_index, color_index, color);
}

//.text:00B329A0
//.text:00B329E0

void __cdecl object_set_custom_animations_hold_on_last_frame(bool custom_animations_hold_on_last_frame)
{
	INVOKE(0x00B32A70, object_set_custom_animations_hold_on_last_frame, custom_animations_hold_on_last_frame);
}

void __cdecl object_set_custom_animations_prevent_lipsync_head_movement(bool custom_animations_prevent_lipsync_head_movement)
{
	INVOKE(0x00B32AA0, object_set_custom_animations_prevent_lipsync_head_movement, custom_animations_prevent_lipsync_head_movement);
}

//.text:00B32AD0
//.text:00B32B80
//.text:00B32C20 ; object_set_garbage

void __cdecl object_set_hidden(long object_index, bool hidden)
{
	INVOKE(0x00B32D50, object_set_hidden, object_index, hidden);
}

void __cdecl object_set_in_limbo(long object_index, bool deactivate)
{
	INVOKE(0x00B32E20, object_set_in_limbo, object_index, deactivate);
}

//.text:00B32EE0
//.text:00B32F20 ; object_set_initial_change_colors
//.text:00B33290 ; object_set_invalid_for_recycling
//.text:00B332F0
//.text:00B333F0 ; object_set_model_state_property
//.text:00B33410 ; object_set_model_state_property_per_region

void __cdecl object_set_object_index_for_name_index(short name_index, long object_index)
{
	INVOKE(0x00B334F0, object_set_object_index_for_name_index, name_index, object_index);
}

void __cdecl object_set_position(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location)
{
	//INVOKE(0x00B33530, object_set_position, object_index, desired_position, desired_forward, desired_up, location);

	object_set_position_direct(object_index, desired_position, desired_forward, desired_up, location, false);
}

void __cdecl object_set_position_direct(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool in_editor)
{
	INVOKE(0x00B33550, object_set_position_direct, object_index, desired_position, desired_forward, desired_up, location, in_editor);
}

void __cdecl object_set_position_in_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool at_rest)
{
	INVOKE(0x00B33600, object_set_position_in_editor, object_index, desired_position, desired_forward, desired_up, location, at_rest);
	
	//object_set_position_direct(object_index, desired_position, desired_forward, desired_up, location, true);
	//object_load_scenario_placement_matrices(object_index);
	//if (at_rest && ((1 << object_get_type(object_index)) & EDITOR_PLACEABLE_OBJECTS_MASK) != 0)
	//	object_set_at_rest(object_index, false);
}

void __cdecl object_set_position_in_sandbox_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location)
{
	//INVOKE(0x00B33670, object_set_position_in_sandbox_editor, object_index, desired_position, desired_forward, desired_up, location);

	object_set_position_direct(object_index, desired_position, desired_forward, desired_up, location, true);
}

void* __cdecl object_try_and_get_and_verify_type(long object_index, dword object_type)
{
	return INVOKE(0x00B34490, object_try_and_get_and_verify_type, object_index, object_type);
}

void __cdecl object_debug_teleport(long object_index, real_point3d const* position)
{
	void* object = object_try_and_get_and_verify_type(object_index, -1);
	if (object)
	{
		dword_flags flags = reinterpret_cast<dword_flags*>(object)[1];

		havok_can_modify_state_allow();
		if (TEST_BIT(flags, 7))
			object_set_in_limbo(object_index, false);
		object_set_position(object_index, position, nullptr, nullptr, nullptr);
		havok_can_modify_state_disallow();
	}
	else
	{
		c_console::write_line("Failed to get a valid object in %s.", __FUNCTION__);
	}
}

