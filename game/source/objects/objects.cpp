#include "objects/objects.hpp"

#include "cache/cache_files.hpp"
#include "items/items.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "models/model_definitions.hpp"
#include "physics/havok.hpp"
#include "profiler/profiler.hpp"
#include "render/render_debug.hpp"

#include <intrin.h>
#include <math.h>

HOOK_DECLARE(0x00B32130, object_render_debug);

bool debug_objects = true;
bool debug_objects_early_movers = false;
bool debug_objects_sound_spheres = false;
bool debug_objects_indices = false;
bool debug_objects_programmer = false;
bool debug_objects_garbage = false;
bool debug_objects_names = false;
bool debug_objects_full_names = false;
bool debug_objects_active_nodes = false;
bool debug_objects_animation_times = false;
bool debug_objects_functions = false;
bool debug_objects_position_velocity = false;
bool debug_objects_origin = false;
bool debug_objects_root_node = false;
bool debug_objects_bounding_spheres = false;
bool debug_objects_attached_bounding_spheres = false;
bool debug_objects_dynamic_render_bounding_spheres = false;
bool debug_objects_model_targets = false;
bool debug_objects_collision_models = false;
bool debug_objects_profile_times = false;
bool debug_objects_water_physics = false;
bool debug_objects_physics_models = false;
bool debug_objects_contact_points = false;
bool debug_objects_constraints = false;
bool debug_objects_vehicle_physics = false;
bool debug_objects_mass = false;
bool debug_objects_pathfinding = false;
bool debug_objects_node_bounds = false;
bool debug_objects_animation = false;

void* __cdecl object_header_block_get(long object_index, object_header_block_reference const* reference)
{
	object_header_datum const* object_header = object_header_get(object_index);
	object_datum* object = object_get(object_index);

	ASSERT(reference->offset > 0);
	ASSERT(reference->size > 0);
	ASSERT(reference->offset + reference->size <= object_header->data_size);

	return (byte*)object + reference->offset;
}

void* __cdecl object_header_block_get_with_count(long object_index, object_header_block_reference const* reference, unsigned int element_size, long* element_count)
{
	ASSERT(element_count);

	if (reference->offset == NONE)
	{
		*element_count = 0;
		return NULL;
	}

	void* block = object_header_block_get(object_index, reference);

	ASSERT(reference->size % element_size == 0);

	*element_count = reference->size / element_size;

	return block;
}

object_header_datum const* __cdecl object_header_get(long object_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(object_header_data);
	return static_cast<object_header_datum*>(datum_try_and_get(*object_header_data, object_index));
}

object_datum* __cdecl object_get(long object_index)
{
	return (object_datum*)object_get_and_verify_type(object_index, 0xFFFFFFFF);
}

void* __cdecl object_get_and_verify_type(long object_index, dword object_type_mask)
{
	//ASSERT(game_state_is_locked(), "someone is calling object_get when the game state is locked");

	object_header_datum const* object_header = object_header_get(object_index);
	if (!object_header)
		return NULL;

	object_datum* object = object_header->datum;

	if (!_bittest((long*)&object_type_mask, object->object.object_identifier.m_type.get()))
	{
		c_static_string<256> string_builder;
		string_builder.print_line("got an object type we didn't expect (expected one of 0x%08x but got #%d).",
			object_type_mask,
			object->object.object_identifier.m_type.get());

		ASSERT(!_bittest((long*)&object_type_mask, object->object.object_identifier.m_type.get()), string_builder.get_string());
	}

	return object;
}

void __cdecl object_get_bounding_sphere(long object_index, real_point3d* center, real* radius)
{
	INVOKE(0x00537990, object_get_bounding_sphere, object_index, center, radius);
}

e_object_type __cdecl object_get_type(long object_index)
{
	return INVOKE(0x0046DC70, object_get_type, object_index);
}

long __cdecl cluster_get_first_collideable_object(long* datum_index, s_cluster_reference cluster_reference)
{
	return INVOKE(0x00B27EB0, cluster_get_first_collideable_object, datum_index, cluster_reference);
}

long __cdecl cluster_get_first_collideable_object_and_payload(long* datum_index, s_cluster_reference cluster_reference, s_object_cluster_payload const** payload)
{
	return INVOKE(0x00B27EE0, cluster_get_first_collideable_object_and_payload, datum_index, cluster_reference, payload);
}

long __cdecl cluster_get_first_noncollideable_object_and_payload(long* datum_index, s_cluster_reference cluster_reference, s_object_cluster_payload const** payload)
{
	return INVOKE(0x00B27F40, cluster_get_first_noncollideable_object_and_payload, datum_index, cluster_reference, payload);
}

long __cdecl cluster_get_next_collideable_object(long* datum_index)
{
	return INVOKE(0x00B27F70, cluster_get_next_collideable_object, datum_index);
}

long __cdecl cluster_get_next_collideable_object_and_payload(long* datum_index, s_object_cluster_payload const** payload)
{
	return INVOKE(0x00B27FA0, cluster_get_next_collideable_object_and_payload, datum_index, payload);
}

long __cdecl cluster_get_next_noncollideable_object_and_payload(long* datum_index, s_object_cluster_payload const** payload)
{
	return INVOKE(0x00B28000, cluster_get_next_noncollideable_object_and_payload, datum_index, payload);
}

void __cdecl object_delete(long object_index)
{
	INVOKE(0x00B2CD10, object_delete, object_index);
}

void __cdecl object_delete_all_multiplayer_cinematic_objects()
{
	INVOKE(0x00B2CE90, object_delete_all_multiplayer_cinematic_objects);
}

void __cdecl object_delete_immediately(long object_index)
{
	INVOKE(0x00B2CF40, object_delete_immediately, object_index);
}

void __cdecl object_delete_recursive(long object_index, bool deactivate)
{
	INVOKE(0x00B2CF70, object_delete_recursive, object_index, deactivate);
}

void __cdecl object_destroy_instance_group(long object_index, long instance_group_index)
{
	INVOKE(0x00B2D0B0, object_destroy_instance_group, object_index, instance_group_index);
}

void __cdecl object_detach(long object_index)
{
	INVOKE(0x00B2D180, object_detach, object_index);
}

void __cdecl object_detach_from_node(long object_index, real_matrix4x3 const* node)
{
	INVOKE(0x00B2D1D0, object_detach_from_node, object_index, node);
}

void __cdecl object_detach_gamestate_entity(long object_index, long gamestate_index)
{
	INVOKE(0x00B2D270, object_detach_gamestate_entity, object_index, gamestate_index);
}

void __cdecl object_detach_immediate(long object_index)
{
	INVOKE(0x00B2D2C0, object_detach_immediate, object_index);
}

void __cdecl object_detach_internal(long object_index, bool compute_bounding_sphere)
{
	INVOKE(0x00B2D460, object_detach_internal, object_index, compute_bounding_sphere);
}

//.text:00B2D4E0 ; void __cdecl object_detach_predicted_objects(long object_index)
//.text:00B2D560 ; void __cdecl object_disconnect_from_map(long object_index, bool broadphase_remove_object)
//.text:00B2D600 ; void __cdecl object_disconnect_from_physics(long object_index)
//.text:00B2D660 ; bool __cdecl object_find_initial_location(long object_index, s_location* location)
//.text:00B2D6E0 ; void __cdecl object_find_structure_bsp_fake_lightprobe_index(long structure_bsp_index, long object_index)
//.text:00B2D7D0 ; bool __cdecl object_force_inside_bsp(long object_index, real_point3d const* position, long a3)
//.text:00B2D9B0 ; void __cdecl object_freeze_node_orientations(long object_index, bool a2)

bool __cdecl object_function_get_function_value(long object_index, s_object_function_definition const* function, long object_definition_index, real* out_function_magnitude, bool* deterministic)
{
	return INVOKE(0x00B2DA20, object_function_get_function_value, object_index, function, object_definition_index, out_function_magnitude, deterministic);
}

//.text:00B2DCB0 ; long __cdecl object_get_attachment_marker_name(long object_index, short attachment_index)
//.text:00B2DD10 ; bool __cdecl object_get_base_change_color_by_index(long object_index, long change_color_index, real_rgb_color* color)

real_point3d* __cdecl object_get_center_of_mass(long object_index, real_point3d* center)
{
	return INVOKE(0x00B2DD90, object_get_center_of_mass, object_index, center);
}

//.text:00B2DE50 ; bool __cdecl object_get_change_color(long object_index, e_change_color_reference change_color_reference, real_rgb_color* color)
//.text:00B2DEF0 ; void __cdecl object_get_closest_point_and_normal(long object_index, real_point3d const* origin, real_point3d* closest_point, vector3d* normal)
//.text:00B2DF80 ; void __cdecl object_get_damage_owner(long object_index, s_damage_owner* owner)

bool __cdecl object_get_function_value(long object_index, long function_name, long object_definition_index, real* out_function_magnitude)
{
	return INVOKE(0x00B2E030, object_get_function_value, object_index, function_name, object_definition_index, out_function_magnitude);
}

short __cdecl object_get_markers_by_string_id(long object_index, string_id marker_name, object_marker* markers, short maximum_marker_count)
{
	return INVOKE(0x00B2E3C0, object_get_markers_by_string_id, object_index, marker_name, markers, maximum_marker_count);
}

real_matrix4x3* __cdecl object_get_node_matrix(long object_index, short node_index)
{
	return INVOKE(0x00B2E450, object_get_node_matrix, object_index, node_index);
}

void __cdecl object_get_orientation(long object_index, vector3d* forward, vector3d* up)
{
	INVOKE(0x00B2E490, object_get_orientation, object_index, forward, up);
}

real_point3d* __cdecl object_get_origin(long object_index, real_point3d* origin)
{
	if (object_datum* object = object_get(object_index))
		return INVOKE(0x00B2E5A0, object_get_origin, object_index, origin);

	return origin;
}

long __cdecl object_get_ultimate_parent(long object_index)
{
	return INVOKE(0x00B2EAB0, object_get_ultimate_parent, object_index);
}

void __cdecl object_get_velocities(long object_index, vector3d* linear_velocity, vector3d* angular_velocity)
{
	INVOKE(0x00B2EB30, object_get_velocities, object_index, linear_velocity, angular_velocity);
}

real_matrix4x3* __cdecl object_get_world_matrix(long object_index, real_matrix4x3* matrix)
{
	return INVOKE(0x00B2EC60, object_get_world_matrix, object_index, matrix);
}

e_object_type c_object_identifier::get_type() const
{
	return m_type;
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

bool __cdecl object_load_scenario_placement_matrices(long object_index)
{
	return INVOKE(0x00B2F890, object_load_scenario_placement_matrices, object_index);
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

void __cdecl object_render_debug(long object_index)
{
	if (TEST_BIT(_object_mask_sound_scenery, object_get_type(object_index)) && !debug_objects_sound_spheres)
		return;

	object_render_debug_internal(object_index);
}

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
	//INVOKE(0x00B33600, object_set_position_in_editor, object_index, desired_position, desired_forward, desired_up, location, at_rest);
	
	object_set_position_direct(object_index, desired_position, desired_forward, desired_up, location, true);
	object_load_scenario_placement_matrices(object_index);
	if (at_rest && TEST_BIT(_object_mask_editor_placeable_objects, object_get_type(object_index)))
		object_set_at_rest(object_index, false);
}

void __cdecl object_set_position_in_sandbox_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location)
{
	//INVOKE(0x00B33670, object_set_position_in_sandbox_editor, object_index, desired_position, desired_forward, desired_up, location);

	object_set_position_direct(object_index, desired_position, desired_forward, desired_up, location, true);
}

bool __cdecl object_set_position_internal(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool compute_node_matrices, bool set_havok_object_position, bool in_editor, bool disconnected)
{
	return INVOKE(0x00B33690, object_set_position_internal, object_index, desired_position, desired_forward, desired_up, location, compute_node_matrices, set_havok_object_position, in_editor, disconnected);
}

void* __cdecl object_try_and_get_and_verify_type(long object_index, dword object_type_mask)
{
	return INVOKE(0x00B34490, object_try_and_get_and_verify_type, object_index, object_type_mask);
}

void __cdecl objects_move()
{
	INVOKE(0x00B36510, objects_move);
}

void __cdecl objects_post_update()
{
	INVOKE(0x00B36610, objects_post_update);
}

void __cdecl objects_update()
{
	INVOKE(0x00B36840, objects_update);

	//TLS_DATA_GET_VALUE_REFERENCE(object_header_data);
	//TLS_DATA_GET_VALUE_REFERENCE(object_globals);
	//
	//PROFILER(object_update)
	//{
	//	PROFILER(objects_update)
	//	{
	//		long* object_early_movers = NULL;
	//		long object_early_movers_count = 0;
	//		object_get_early_movers(&object_early_movers, &object_early_movers_count);
	//		//object_profile_reset();
	//
	//		object_globals->garbage_collection_cannot_run = true;
	//
	//		//if (debug_objects_force_awake)
	//		//{
	//		//	c_data_iterator<object_header_datum> object_header_iter;
	//		//	object_header_iter.begin(*object_header_data);
	//		//	while (object_header_iter.next())
	//		//	{
	//		//		object_header_datum* object_header = object_header_iter.get_datum();
	//		//		object_header->flags.set(_object_header_awake_bit, true);
	//		//	}
	//		//}
	//
	//		//if (debug_objects_dump_memory_stats)
	//		//{
	//		//	dump_object_memory_stats();
	//		//	debug_objects_dump_memory_stats = false;
	//		//}
	//
	//		ASSERT(object_globals->object_update_absolute_index == NONE);
	//		object_globals->object_update_absolute_index = 0;
	//
	//		for (long object_early_mover_index = 0; object_early_mover_index < object_early_movers_count; object_early_mover_index++)
	//		{
	//			long object_index = object_early_movers[object_early_mover_index];
	//			object_header_datum* object_header = (object_header_datum*)datum_get(*object_header_data, object_index);
	//
	//			if (object_header->flags.test(_object_header_active_bit) &&
	//				object_header->flags.test(_object_header_awake_bit) &&
	//				!object_header->flags.test(_object_header_post_update_bit))
	//			{
	//				object_update(object_index);
	//				object_early_mover_update(object_index, object_early_mover_index);
	//
	//				//ASSERT(TEST_FLAG(object_header->datum->object.physics_flags, _object_is_early_mover_bit));
	//
	//				if (object_header->flags.test(_object_header_requires_motion_bit) &&
	//					object_header->flags.test(_object_header_post_update_bit))
	//				{
	//					object_early_mover_notify_local_objects(object_index);
	//				}
	//			}
	//		}
	//
	//		c_data_iterator<object_header_datum> object_header_iter;
	//		object_header_iter.begin(*object_header_data);
	//		while (object_header_iter.next())
	//		{
	//			long object_update_absolute_index = object_header_iter.get_absolute_index();
	//			object_header_datum* object_header = object_header_iter.get_datum();
	//			ASSERT(location_valid(&object_header->datum->object.location) == scenario_location_valid(&object_header->datum->object.location));
	//
	//			if (object_header->flags.test(_object_header_active_bit) &&
	//				object_header->flags.test(_object_header_awake_bit) &&
	//				!object_header->flags.test(_object_header_post_update_bit))
	//			{
	//				object_datum* object = (object_datum*)object_get_and_verify_type(object_header_iter.get_index(), NONE);
	//				ASSERT(object->object.parent_object_index == NONE);
	//				ASSERT(object->object.next_object_index == NONE);
	//
	//				if (object->object.flags.test(_object_in_limbo_bit))
	//				{
	//					item_datum* item = (item_datum*)object_get_and_verify_type(object_header_iter.get_index(), _object_mask_item);
	//					//ASSERT(TEST_FLAG(item->item.flags, _item_in_unit_inventory_bit));
	//					//ASSERT(TEST_FLAG(item->item.flags, _item_hidden_in_unit_inventory_bit));
	//				}
	//
	//				//if (object->object.physics_flags.test(0))
	//				if (TEST_BIT(object->object.physics_flags, 0))
	//				{
	//					object_globals->object_update_absolute_index = object_update_absolute_index;
	//					object_update(object_update_absolute_index);
	//				}
	//			}
	//		}
	//
	//		object_globals->object_update_absolute_index = NONE;
	//		object_globals->garbage_collection_cannot_run = false;
	//
	//		damage_update();
	//	}
	//}
}

void __cdecl object_debug_teleport(long object_index, real_point3d const* position)
{
	if (object_datum* object = object_get(object_index))
	{
		havok_can_modify_state_allow();

		if (object->object.flags.test(_object_in_limbo_bit))
			object_set_in_limbo(object_index, false);

		object_set_position_internal(object_index, position, nullptr, nullptr, nullptr, false, true, false, true);

		havok_can_modify_state_disallow();
	}
	else
	{
		generate_event(_event_level_warning, "Failed to get a valid object in %s.", __FUNCTION__);
	}
}

void __cdecl object_get_debug_name(long object_index, bool full_name, c_static_string<256>* name)
{
	object_datum* object = object_get(object_index);

	if (!object)
	{
		name->print("Unknown");
		return;
	}

	name->clear();
	if (object->object.name_index != NONE)
	{
		s_scenario* scenario = global_scenario_get();
		scenario_object_name& object_name = scenario->object_names[object->object.name_index];

		name->append_print("%s|n", object_name.name.get_string());
	}

	tag group_tag = tag_get_group_tag(object->object.definition_index);
	char const* group_tag_name = tag_group_get_name(group_tag);
	char const* tag_name = tag_get_name(object->object.definition_index);

	if (!full_name)
		tag_name = tag_name_strip_path(tag_get_name(object->object.definition_index));

	name->append_print("%s.%s|n", tag_name, group_tag_name);

	if (object->object.variant_index == NONE)
	{
		name->append("[default]|n");
	}
	else
	{
		_object_definition* object_definition = static_cast<_object_definition*>(tag_get(OBJECT_TAG, object->object.definition_index));

		s_model_definition* model_definition = nullptr;
		if (object_definition->model.index != NONE)
			model_definition = object_definition->model.cast_to<s_model_definition>();

		//if (model_definition && object->variant_index < 128 && object->variant_index < model_definition)
		//{
		//
		//}
		//else
		//{
		//	name->append_print("[invalid! %d]|n", object->variant_index);
		//}
	}
}

void __cdecl object_render_debug_internal(long object_index)
{
	object_header_datum const* object_header = object_header_get(object_index);
	object_datum* object = object_get(object_index);

	_object_definition* object_definition = static_cast<_object_definition*>(tag_get(OBJECT_TAG, object->object.definition_index));

	c_static_string<4096> string;

	if (debug_objects_indices)
		string.append_print("%d (%#x)|n", object_index, object_index);

	if (debug_objects_programmer)
	{
		string.append_print("header flags: %04x|n", object_header->flags.get_unsafe());
		string.append_print("datum flags: %08x|n", object->object.flags.get_unsafe());
	}

	if (debug_objects_garbage)
	{
		if (TEST_BIT(object->object.recycling_flags, 0))
		{
			string.append("never-garbage|n");
		}
		else if (TEST_BIT(object->object.recycling_flags, 1))
		{
			if (game_time_get() >= object->object.recycling_time)
			{
				string.append("garbage|n");
			}
			else
			{
				string.append_print("garbage in %d|n", object->object.recycling_time - game_time_get());
			}
		}
	}

	if (debug_objects_names)
	{
		c_static_string<256> name;
		object_get_debug_name(object_index, debug_objects_full_names, &name);
		string.append_print("%s", name.get_string());
	}

	if (debug_objects_active_nodes)
	{
		//debug_objects_animation_times
	}

	if (debug_objects_functions)
	{
		for (s_object_function_definition& function : object_definition->functions)
		{
			real function_magnitude = 0.0f;
			bool deterministic = false;

			bool import_function_value = object_get_function_value(object_index, function.import_name.get_value(), object->object.definition_index, &function_magnitude);
			string.append_print("%s: %s %.2f->", function.import_name.get_string(), import_function_value ? "ON" : "OFF", function_magnitude);

			bool export_function_value = object_function_get_function_value(object_index, &function, object->object.definition_index, &function_magnitude, &deterministic);
			string.append_print("%s: %s %.2f|n", function.export_name.get_string(), export_function_value ? "ON" : "OFF", function_magnitude);
		}
	}

	if (debug_objects_position_velocity)
	{
		real_matrix4x3 matrix{};
		vector3d linear_velocity{};

		object_get_world_matrix(object_index, &matrix);
		object_get_velocities(object_index, &linear_velocity, nullptr);

		render_debug_matrix(true, &matrix, object->object.bounding_sphere_radius);
		render_debug_vector(true, &matrix.center, &linear_velocity,1.0f, global_real_argb_yellow);
	}

	if (debug_objects_origin)
	{
		real a1 = 0.1f;
		real a2 = 0.4f;
		real seconds = game_ticks_to_seconds(real(game_time_get()));
		real angle = (seconds * TWO_PI) / 3.0f;
		real cos_angle = cosf(angle);
		real radius = (((cos_angle + 1.0f) * a2) / 2) + a1;

		real_point3d origin{};
		object_get_origin(object_index, &origin);
		render_debug_sphere(true, &origin, radius, global_real_argb_purple);
	}

	if (debug_objects_root_node)
	{
		real_matrix4x3* root_node_matrix = object_get_node_matrix(object_index, 0);
		render_debug_matrix(true, root_node_matrix, object->object.bounding_sphere_radius);
	}

	if (debug_objects_bounding_spheres)
	{
		long parent_object_index = object_get_ultimate_parent(object_index);
		object_header_datum const* parent_object_header = object_header_get(parent_object_index);
		object_datum* parent_object = object_get(parent_object_index);

		render_debug_sphere(true, &object->object.bounding_sphere_center, object->object.bounding_sphere_radius > 0.0f ? object->object.bounding_sphere_radius : 0.25f, global_real_argb_blue);

		if (debug_objects_attached_bounding_spheres)
		{
			render_debug_point(true, &object->object.attached_bounds_center, 0.1f, global_real_argb_blue);
			render_debug_sphere(true, &object->object.attached_bounds_center, object->object.attached_bounds_radius, global_real_argb_blue);
		}
	}

	if (debug_objects_dynamic_render_bounding_spheres)
	{

	}

	s_model_definition* model_definition = nullptr;
	if (object_definition->model.index != NONE)
		model_definition = object_definition->model.cast_to<s_model_definition>();

	if (debug_objects_model_targets && model_definition)
	{
		for (s_model_target& target : model_definition->targets)
		{
			object_marker markers[2]{};
			short marker_count = object_get_markers_by_string_id(object_index, target.marker_name.get_value(), markers, NUMBEROF(markers));
			switch (marker_count)
			{
			case 1:
			{
				render_debug_vector(true, &markers[0].node_matrix.center, &markers[0].node_matrix.matrix.forward, target.size, global_real_argb_darkgreen);

				if (target.cone_angle <= 3.1414928f)
					render_debug_cone_outline(true, &markers[0].node_matrix.center, &markers[0].node_matrix.matrix.forward, target.size, target.cone_angle, global_real_argb_darkgreen);
				else
					render_debug_sphere(true, &markers[0].node_matrix.center, target.size, global_real_argb_darkgreen);
			}
			break;
			case 2:
			{
				vector3d height{};
				vector_from_points3d(&markers[0].node_matrix.center, &markers[1].node_matrix.center, &height);
				render_debug_pill(true, &markers[0].node_matrix.center, &height, target.size, global_real_argb_darkgreen);
			}
			break;
			}
		}
	}

	//collision_model_instance instance{};
	//if (debug_objects_collision_models && collision_model_instance_new(&instance, object_index))
	//{
	//
	//}

	if (debug_objects_early_movers && object_definition->flags.test(_object_definition_flag_early_mover_bit))
	{
		char const* early_mover_string = "early mover";
		if (object_definition->flags.test(_object_definition_flag_early_mover_localized_physics_bit))
			early_mover_string = "early mover + localized physics";

		real_matrix4x3* root_node_matrix = object_get_node_matrix(object_index, 0);
		render_debug_string_at_point(&root_node_matrix->center, early_mover_string, global_real_argb_darkgreen);
	}

	//real object_cpu_times[2];
	//if (debug_objects_profile_times && object_profile_query_object_instance_cpu_times(object_index, &object_cpu_times))
	//{
	//
	//}

	REFERENCE_DECLARE(object + 0xA0, long, havok_component_index);
	if (havok_component_index == NONE)
	{

	}
	else
	{
		//debug_objects_water_physics
		//debug_objects_physics_models
		//debug_objects_contact_points
		//debug_objects_constraints
		//debug_objects_vehicle_physics
		//debug_objects_mass
	}

	if (debug_objects_pathfinding)
	{

	}

	if (!string.is_empty())
	{
		real_matrix4x3* root_node_matrix = object_get_node_matrix(object_index, 0);
		render_debug_string_at_point(&root_node_matrix->center, string.get_string(), global_real_argb_green);
	}

	if (debug_objects_node_bounds)
	{

	}

	if (debug_objects_animation)
	{

	}
}

