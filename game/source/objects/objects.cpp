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

HOOK_DECLARE(0x00B31590, object_placement_data_new);
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

void __cdecl object_detach_predicted_objects(long object_index)
{
	INVOKE(0x00B2D4E0, object_detach_predicted_objects, object_index);
}

void __cdecl object_disconnect_from_map(long object_index, bool broadphase_remove_object)
{
	INVOKE(0x00B2D560, object_disconnect_from_map, object_index, broadphase_remove_object);
}

void __cdecl object_disconnect_from_physics(long object_index)
{
	INVOKE(0x00B2D600, object_disconnect_from_physics, object_index);
}

bool __cdecl object_find_initial_location(long object_index, s_location* location)
{
	return INVOKE(0x00B2D660, object_find_initial_location, object_index, location);
}

void __cdecl object_find_structure_bsp_fake_lightprobe_index(long structure_bsp_index, long object_index)
{
	INVOKE(0x00B2D6E0, object_find_structure_bsp_fake_lightprobe_index, structure_bsp_index, object_index);
}

bool __cdecl object_force_inside_bsp(long object_index, real_point3d const* position, long ignore_object_index)
{
	return INVOKE(0x00B2D7D0, object_force_inside_bsp, object_index, position, ignore_object_index);
}

void __cdecl object_freeze_node_orientations(long object_index, bool a2)
{
	INVOKE(0x00B2D9B0, object_freeze_node_orientations, object_index, a2);
}

bool __cdecl object_function_get_function_value(long object_index, s_object_function_definition const* function, long object_definition_index, real* out_function_magnitude, bool* deterministic)
{
	return INVOKE(0x00B2DA20, object_function_get_function_value, object_index, function, object_definition_index, out_function_magnitude, deterministic);
}

long __cdecl object_get_attachment_marker_name(long object_index, short attachment_index)
{
	return INVOKE(0x00B2DCB0, object_get_attachment_marker_name, object_index, attachment_index);
}

bool __cdecl object_get_base_change_color_by_index(long object_index, long change_color_index, real_rgb_color* color)
{
	return INVOKE(0x00B2DD10, object_get_base_change_color_by_index, object_index, change_color_index, color);
}

real_point3d* __cdecl object_get_center_of_mass(long object_index, real_point3d* center)
{
	return INVOKE(0x00B2DD90, object_get_center_of_mass, object_index, center);
}

bool __cdecl object_get_change_color(long object_index, long change_color_index, real_rgb_color* change_color)
{
	return INVOKE(0x00B2DE50, object_get_change_color, object_index, change_color_index, change_color);
}

void __cdecl object_get_closest_point_and_normal(long object_index, real_point3d const* origin, real_point3d* closest_point, vector3d* normal)
{
	INVOKE(0x00B2DEF0, object_get_closest_point_and_normal, object_index, origin, closest_point, normal);
}

void __cdecl object_get_damage_owner(long object_index, s_damage_owner* owner)
{
	INVOKE(0x00B2DF80, object_get_damage_owner, object_index, owner);
}

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

long __cdecl object_get_variant_index(long object_index)
{
	return INVOKE(0x00B2EB00, object_get_variant_index, object_index);
}

void __cdecl object_get_velocities(long object_index, vector3d* linear_velocity, vector3d* angular_velocity)
{
	INVOKE(0x00B2EB30, object_get_velocities, object_index, linear_velocity, angular_velocity);
}

real_matrix4x3* __cdecl object_get_world_matrix(long object_index, real_matrix4x3* matrix)
{
	return INVOKE(0x00B2EC60, object_get_world_matrix, object_index, matrix);
}

//.text:00B2ECE0 ; real_matrix4x3* __cdecl object_get_world_matrix_interpolated(long, real_matrix4x3*)
//.text:00B2EDF0 ; bool __cdecl object_has_animation_manager(long)
//.text:00B2EE30 ; bool __cdecl object_has_multiplayer_properties(long)
//.text:00B2EE70 ; bool __cdecl object_header_block_allocate(long, short, short, short)
//.text:00B2EF90 ; void __cdecl object_header_delete(long)
//.text:00B2EFF0 ; long __cdecl object_header_new(short)
//.text:00B2F0A0 ; long __cdecl object_index_from_name_index(short)
//.text:00B2F0E0 ; long __cdecl object_index_from_scenario_object_index(enum e_object_type, long)
//.text:00B2F120 ; bool __cdecl object_is_at_rest(long)
//.text:00B2F160 ; bool __cdecl object_is_being_deleted(long)
//.text:00B2F190 ; bool __cdecl object_is_connected_to_map(long)
//.text:00B2F1D0 ; bool __cdecl object_is_hidden(long)
//.text:00B2F220 ; bool __cdecl object_is_hidden_internal(object_header_datum const*, object_datum const*)
//.text:00B2F240 ; bool __cdecl object_is_hidden_non_recursive(long)
//.text:00B2F280 ; 

bool __cdecl object_is_multiplayer_cinematic_object(long object_index)
{
	return INVOKE(0x00B2F300, object_is_multiplayer_cinematic_object, object_index);
}

//.text:00B2F340 ; bool __cdecl object_is_or_contains_player(long)
//.text:00B2F470 ; bool __cdecl object_is_vehicle(long)

e_object_type c_object_identifier::get_type() const
{
	return m_type;
}

void c_object_identifier::clear()
{
	m_type = k_object_type_none;
	m_source = k_object_source_none;
	m_origin_bsp_index = NONE;
	m_unique_id = NONE;
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

//.text:00B2F830 ; long __cdecl object_list_children_by_definition(long, long)

bool __cdecl object_load_scenario_placement_matrices(long object_index)
{
	return INVOKE(0x00B2F890, object_load_scenario_placement_matrices, object_index);
}

//.text:00B2FBD0 ; long __cdecl object_local_physics_object_get(long)
//.text:00B2FC20 ; 
//.text:00B2FC70 ; void __cdecl object_make_noise(long, e_sound_type, e_ai_sound_volume)
//.text:00B2FCE0 ; bool __cdecl object_mark(long)
//.text:00B2FD20 ; bool __cdecl object_mark_get_user_data(long, long *)
//.text:00B2FD60 ; bool __cdecl object_mark_with_user_data(long, long)
//.text:00B2FDC0 ; void __cdecl object_marker_begin()
//.text:00B2FDE0 ; void __cdecl object_marker_end()
//.text:00B2FE00 ; void __cdecl object_marker_reopen()
//.text:00B2FE20 ; void __cdecl object_model_state_changed(long, long, char const*)
//.text:00B2FE50 ; void __cdecl object_move(long)
//.text:00B30050 ; void __cdecl object_move_position(long, real_point3d const*, vector3d const*, vector3d const*, s_location const*)
//.text:00B301F0 ; void __cdecl object_name_list_allocate()
//.text:00B30250 ; 
//.text:00B30280 ; void __cdecl object_name_list_delete(long)
//.text:00B30310 ; 
//.text:00B30350 ; 
//.text:00B303A0 ; bool __cdecl object_needs_rigid_body_update(long)

long __cdecl object_new(object_placement_data* data)
{
	return INVOKE(0x00B30440, object_new, data);
}

//.text:00B30E60 ; long __cdecl object_new_by_name(short, bool, bool)
//.text:00B30FD0 ; long __cdecl object_new_from_scenario(e_object_type, long, s_scenario_object*, s_tag_block*, bool)
//.text:00B31000 ; long __cdecl object_new_from_scenario_bypass_simulation_object_placement_test(e_object_type, long, s_scenario_object*, s_tag_block*, long, bool)
//.text:00B31030 ; 
//.text:00B310C0 ; long __cdecl object_new_from_scenario_internal(e_object_type, short, s_scenario_object const*, s_tag_block*, long, bool, bool, long)
//.text:00B31240 ; bool __cdecl object_node_orientations_frozen(long)
//.text:00B31270 ; 
//.text:00B312F0 ; 
//.text:00B31380 ; bool __cdecl object_owns_object(long, long)
//.text:00B313E0 ; 

void __cdecl object_place(long object_index, s_scenario_object const* scenario_object)
{
	INVOKE(0x00B31470, object_place, object_index, scenario_object);
}

void __cdecl object_placement_data_copy_change_colors(object_placement_data* data, long object_index)
{
	INVOKE(0x00B314E0, object_placement_data_copy_change_colors, data, object_index);
}

void __cdecl object_placement_data_new(object_placement_data* data, long definition_index, long owner_object_index, s_damage_owner const* damage_owner)
{
	//INVOKE(0x00B31590, object_placement_data_new, data, definition_index, owner_object_index, damage_owner);

	csmemset(data, 0, sizeof(object_placement_data));
	
	data->definition_index = definition_index;
	data->model_variant_index = 0;
	data->flags = 0;
	data->forward = *global_forward3d;
	data->up = *global_up3d;
	data->ai_state_type = NONE;
	data->scale = 1.0f;
	data->active_change_colors.clear();
	data->bsp_placement_policy = 0;
	
	if (object_datum* owner_object = object_get(owner_object_index))
	{
		data->owner_object_index = owner_object_index;
		data->owner_player_index = NONE;
		data->owner_team_index = NONE;
	
		SET_BIT(data->flags, 5, TEST_BIT(owner_object->object.physics_flags, 2));
	
		if (TEST_BIT(_object_mask_unit, owner_object->object.object_identifier.get_type()))
		{
			unit_datum* unit = (unit_datum*)owner_object;
			data->owner_player_index = unit->unit.player_index;
			data->owner_team_index = unit->unit.team;
		}
	}
	else
	{
		data->owner_object_index = NONE;
		data->owner_player_index = NONE;
		data->owner_team_index = NONE;
	}
	
	data->damage_owner = *global_damage_owner_unknown;
	if (damage_owner)
		data->damage_owner = *damage_owner;

	data->object_identifier.clear();

	data->scenario_datum_index = NONE;
	data->location_set = false;

	if (owner_object_index != NONE)
		data->multiplayer_cinematic_object = object_is_multiplayer_cinematic_object(owner_object_index);

	data->parent_object_index = NONE;
	data->parent_marker = _string_id_invalid;
	data->connection_marker = _string_id_invalid;
	
	for (s_model_customization_region_permutation& permutation : data->model_customization_overrides)
	{
		permutation.region_name = NONE;
		permutation.permutation_name = NONE;
	}
	data->model_customization_override_count = 0;
}

void __cdecl object_placement_data_set_location(object_placement_data* data, struct s_location const* location)
{
	INVOKE(0x00B31750, object_placement_data_set_location, data, location);

	//ASSERT(data != NULL);
	//ASSERT(location != NULL);
	//
	//data->location = *location;
	//data->location_set = true;
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

void __cdecl object_remove_from_list(long* object_list, long object_index)
{
	INVOKE(0x00B320A0, object_remove_from_list, object_list, object_index);
}

//.text:00B32100 ; 

void __cdecl object_render_debug(long object_index)
{
	if (TEST_BIT(_object_mask_sound_scenery, object_get_type(object_index)) && !debug_objects_sound_spheres)
		return;

	object_render_debug_internal(object_index);
}

//.text:00B32140 ; 

void __cdecl object_reset(long object_index)
{
	INVOKE(0x00B32160, object_reset, object_index);
}

void __cdecl object_reset_interpolation(long object_index)
{
	INVOKE(0x00B321A0, object_reset_interpolation, object_index);
}

void __cdecl object_respond_to_physics(long object_index)
{
	INVOKE(0x00B321F0, object_respond_to_physics, object_index);
}

void __cdecl object_resurrect(long object_index)
{
	INVOKE(0x00B32400, object_resurrect, object_index);
}

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

void __cdecl object_set_body_vitality(long object_index, real body_vitality, short body_stun_ticks)
{
	INVOKE(0x00B329A0, object_set_body_vitality, object_index, body_vitality, body_stun_ticks);
}

void __cdecl object_set_custom_animation_speed(long object_index, real custom_animation_speed)
{
	INVOKE(0x00B329E0, object_set_custom_animation_speed, object_index, custom_animation_speed);
}

void __cdecl object_set_custom_animations_hold_on_last_frame(bool custom_animations_hold_on_last_frame)
{
	INVOKE(0x00B32A70, object_set_custom_animations_hold_on_last_frame, custom_animations_hold_on_last_frame);
}

void __cdecl object_set_custom_animations_prevent_lipsync_head_movement(bool custom_animations_prevent_lipsync_head_movement)
{
	INVOKE(0x00B32AA0, object_set_custom_animations_prevent_lipsync_head_movement, custom_animations_prevent_lipsync_head_movement);
}

void __cdecl object_set_damage_owner(long object_index, s_damage_owner const* damage_owner, bool a3)
{
	INVOKE(0x00B32AD0, object_set_damage_owner, object_index, damage_owner, a3);
}

void __cdecl object_set_desired_velocities(long object_index, vector3d const* transitional_velocity, vector3d const* angular_velocity, bool a4)
{
	INVOKE(0x00B32B80, object_set_desired_velocities, object_index, transitional_velocity, angular_velocity, a4);
}

void __cdecl object_set_garbage(long object_index, bool a2, long collection_ticks)
{
	INVOKE(0x00B32C20, object_set_garbage, object_index, a2, collection_ticks);
}

void __cdecl object_set_hidden(long object_index, bool hidden)
{
	INVOKE(0x00B32D50, object_set_hidden, object_index, hidden);
}

void __cdecl object_set_in_limbo(long object_index, bool deactivate)
{
	INVOKE(0x00B32E20, object_set_in_limbo, object_index, deactivate);
}

void __cdecl object_shield_stun_infinite(long object_index)
{
	INVOKE(0x00B32EE0, object_shield_stun_infinite, object_index);
}

void __cdecl object_set_initial_change_colors(long object_index, c_flags<char, byte, 5> active_change_colors, real_rgb_color const* change_colors)
{
	INVOKE(0x00B32F20, object_set_initial_change_colors, object_index, active_change_colors, change_colors);
}

void __cdecl object_set_invalid_for_recycling(long object_index, bool a2)
{
	INVOKE(0x00B33290, object_set_invalid_for_recycling, object_index, a2);
}

bool __cdecl object_set_model_state(long object_index, long a2, long a3, bool a4)
{
	return INVOKE(0x00B332F0, object_set_model_state, object_index, a3, a3, a4);
}

void __cdecl object_set_model_state_property(long object_index, long a2, long a3, bool a4, bool a5)
{
	INVOKE(0x00B333F0, object_set_model_state_property, object_index, a3, a3, a4, a5);
}

void __cdecl object_set_model_state_property_per_region(long object_index, long a2, long a3, short a4, bool a5)
{
	INVOKE(0x00B33410, object_set_model_state_property_per_region, object_index, a3, a3, a4, a5);
}

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

//.text:00B344E0 ; s_multiplayer_object_properties* __cdecl object_try_and_get_multiplayer(long object_index)

void* __cdecl object_try_and_get_unsafe_and_verify_type(long object_index, dword object_type_mask)
{
	return INVOKE(0x00B34540, object_try_and_get_unsafe_and_verify_type, object_index, object_type_mask);
}

bool __cdecl object_unmarked(long object_index)
{
	return INVOKE(0x00B34590, object_unmarked, object_index);
}

void __cdecl object_unregister_scenario_object(long object_index)
{
	INVOKE(0x00B345C0, object_unregister_scenario_object, object_index);
}

bool __cdecl object_update(long object_index)
{
	return INVOKE(0x00B34630, object_update, object_index);
}

void __cdecl object_update_collision_culling(long object_index)
{
	INVOKE(0x00B347F0, object_update_collision_culling, object_index);
}

//.text:00B34870 ; 

void __cdecl object_update_visibility_culling(long object_index)
{
	INVOKE(0x00B348A0, object_update_visibility_culling, object_index);
}

bool __cdecl object_visible_to_any_player(long object_index)
{
	return INVOKE(0x00B34930, object_visible_to_any_player, object_index);
}

void __cdecl object_wake(long object_index)
{
	INVOKE(0x00B34D00, object_wake, object_index);
}

//.text:00B34D60 ; void __cdecl objects_activation(s_game_cluster_bit_vectors const*, s_game_cluster_bit_vectors const*)
//.text:00B35150 ; bool __cdecl objects_activation_find_scenario_reference_in_currently_active_clusters(s_game_cluster_bit_vectors const*, s_game_cluster_bit_vectors const*, s_scenario_object_reference const*)

bool __cdecl objects_can_connect_to_map()
{
	return INVOKE(0x00B35260, objects_can_connect_to_map);
}

long __cdecl objects_compact_memory_pool()
{
	return INVOKE(0x00B35290, objects_compact_memory_pool);
}

void __cdecl objects_detach_from_simulation()
{
	INVOKE(0x00B352D0, objects_detach_from_simulation);
}

void __cdecl objects_dispose()
{
	INVOKE(0x00B35380, objects_dispose);
}

void __cdecl objects_dispose_from_old_map()
{
	INVOKE(0x00B35430, objects_dispose_from_old_map);
}

void __cdecl objects_dispose_from_old_structure_bsp(dword a1)
{
	INVOKE(0x00B35530, objects_dispose_from_old_structure_bsp, a1);
}

void __cdecl objects_enable_warthog_chaingun_light(bool enable_warthog_chaingun_light)
{
	INVOKE(0x00B35710, objects_enable_warthog_chaingun_light, enable_warthog_chaingun_light);

	//TLS_DATA_GET_VALUE_REFERENCE(object_globals);
	//object_globals->warthog_chaingun_light_disabled = !enable_warthog_chaingun_light;
}

void __cdecl objects_garbage_collection()
{
	INVOKE(0x00B35740, objects_garbage_collection);
}

long __cdecl objects_get_active_garbage_count()
{
	return INVOKE(0x00B35790, objects_get_active_garbage_count);
}

render_lighting* __cdecl objects_get_cinematic_lighting()
{
	return INVOKE(0x00B357B0, objects_get_cinematic_lighting);
}

long __cdecl objects_get_contiguous_free_memory_size()
{
	return INVOKE(0x00B357D0, objects_get_contiguous_free_memory_size);
}

long __cdecl objects_get_first_garbage_object()
{
	return INVOKE(0x00B357F0, objects_get_first_garbage_object);
}

long __cdecl objects_get_free_object_header_count()
{
	return INVOKE(0x00B35810, objects_get_free_object_header_count);
}

long __cdecl objects_get_next_garbage_object(long object_index)
{
	return INVOKE(0x00B35830, objects_get_next_garbage_object, object_index);
}

void __cdecl objects_handle_deleted_object(long object_index)
{
	INVOKE(0x00B35870, objects_handle_deleted_object, object_index);
}

void __cdecl objects_handle_deleted_player(long object_index)
{
	INVOKE(0x00B35910, objects_handle_deleted_player, object_index);
}

//.text:00B35990 ; long __cdecl objects_in_clusters_by_indices(dword, bool, c_flags<e_object_collision_cull_flag, word, 13>, long, s_cluster_reference const*, long, long*)
//.text:00B35B60 ; long __cdecl objects_in_sphere(dword, dword, s_location const*, real_point3d const*, real, long*, long)

void __cdecl objects_information_get(objects_information* objects_information_out)
{
	INVOKE(0x00B35E40, objects_information_get, objects_information_out);
}

void __cdecl objects_initialize()
{
	INVOKE(0x00B35F50, objects_initialize);
}

void __cdecl objects_initialize_for_new_map()
{
	INVOKE(0x00B36130, objects_initialize_for_new_map);
}

void __cdecl objects_initialize_for_new_structure_bsp(dword a1)
{
	INVOKE(0x00B36310, objects_initialize_for_new_structure_bsp, a1);
}

bool __cdecl objects_is_warthog_chaingun_light_enabled()
{
	return INVOKE(0x00B36480, objects_is_warthog_chaingun_light_enabled);

	//TLS_DATA_GET_VALUE_REFERENCE(object_globals);
	//return !object_globals->warthog_chaingun_light_disabled;
}

void __cdecl objects_memory_compact()
{
	INVOKE(0x00B364B0, objects_memory_compact);
}

void __cdecl objects_move()
{
	INVOKE(0x00B36510, objects_move);
}

void __cdecl objects_post_update()
{
	INVOKE(0x00B36610, objects_post_update);
}

void __cdecl objects_purge_deleted_objects()
{
	INVOKE(0x00B36710, objects_purge_deleted_objects);
}

void __cdecl objects_render_thread_update()
{
	INVOKE(0x00B367A0, objects_render_thread_update);
}

void __cdecl objects_setup_structure_bsp_fake_lightprobes(long structure_bsp_index)
{
	INVOKE(0x00B367D0, objects_setup_structure_bsp_fake_lightprobes, structure_bsp_index);
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
	//			//ASSERT(location_valid(&object_header->datum->object.location) == scenario_location_valid(&object_header->datum->object.location));
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

void __cdecl objects_update_header_callback(long object_index, dword datum_handle)
{
	INVOKE(0x00B369A0, objects_update_header_callback, object_index, datum_handle);
}

//.text:00B369E0 ; 
//.text:00B36A30 ; 

void __cdecl recursive_wake_children_awoken_by_movement(long object_index)
{
	INVOKE(0x00B36A90, recursive_wake_children_awoken_by_movement, object_index);
}

//.text:00B36B10 ; 
//.text:00B36B50 ; 
//.text:00B36B90 ; 
//.text:00B36BD0 ; 
//.text:00B36C10 ; 
//.text:00B36C50 ; 
//.text:00B36C90 ; 
//.text:00B36CD0 ; 
//.text:00B36D10 ; 
//.text:00B36D50 ; 
//.text:00B36D90 ; 
//.text:00B36DD0 ; 

void __cdecl reset_object_cached_render_state(long object_index)
{
	INVOKE(0x00B36E20, reset_object_cached_render_state, object_index);
}

void __cdecl reset_object_render_message_queue()
{
	INVOKE(0x00B36E90, reset_object_render_message_queue);
}

//.text:00B36EE0 ; 

bool __cdecl sample_object_lighting_from_probes(long object_index)
{
	return INVOKE(0x00B36F50, sample_object_lighting_from_probes, object_index);
}

void __cdecl scripted_object_function_set(long object_function_index, real object_function_value)
{
	INVOKE(0x00B371F0, scripted_object_function_set, object_function_index, object_function_value);
}

//.text:00B373D0 ; void __cdecl set_memory_pool_address(void*)

void __cdecl object_debug_teleport(long object_index, real_point3d const* position)
{
	if (object_datum* object = object_get(object_index))
	{
		havok_can_modify_state_allow();

		if (object->object.flags.test(_object_in_limbo_bit))
			object_set_in_limbo(object_index, false);

		object_set_position_internal(object_index, position, NULL, NULL, NULL, false, true, false, true);

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

	tag group_tag = tag_get_group_tag(object->definition_index);
	char const* group_tag_name = tag_group_get_name(group_tag);
	char const* tag_name = tag_get_name(object->definition_index);

	if (!full_name)
		tag_name = tag_name_strip_path(tag_get_name(object->definition_index));

	name->append_print("%s.%s|n", tag_name, group_tag_name);

	if (object->object.variant_index == NONE)
	{
		name->append("[default]|n");
	}
	else
	{
		struct object_definition* object_definition = (struct object_definition*)tag_get(OBJECT_TAG, object->definition_index);

		s_model_definition* model_definition = NULL;
		if (object_definition->object.model.index != NONE)
			model_definition = object_definition->object.model.cast_to<s_model_definition>();

		//if (model_definition && object_definition->object.variant_index < 128 && object_definition->object.variant_index < model_definition)
		//{
		//
		//}
		//else
		//{
		//	name->append_print("[invalid! %d]|n", object_definition->object.variant_index);
		//}
	}
}

void __cdecl object_render_debug_internal(long object_index)
{
	object_header_datum const* object_header = object_header_get(object_index);
	object_datum* object = object_get(object_index);

	struct object_definition* object_definition = (struct object_definition*)tag_get(OBJECT_TAG, object->definition_index);

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
		for (s_object_function_definition& function : object_definition->object.functions)
		{
			real function_magnitude = 0.0f;
			bool deterministic = false;

			bool import_function_value = object_get_function_value(object_index, function.import_name.get_value(), object->definition_index, &function_magnitude);
			string.append_print("%s: %s %.2f->", function.import_name.get_string(), import_function_value ? "ON" : "OFF", function_magnitude);

			bool export_function_value = object_function_get_function_value(object_index, &function, object->definition_index, &function_magnitude, &deterministic);
			string.append_print("%s: %s %.2f|n", function.export_name.get_string(), export_function_value ? "ON" : "OFF", function_magnitude);
		}
	}

	if (debug_objects_position_velocity)
	{
		real_matrix4x3 matrix{};
		vector3d linear_velocity{};

		object_get_world_matrix(object_index, &matrix);
		object_get_velocities(object_index, &linear_velocity, NULL);

		render_debug_matrix(true, &matrix, object->object.bounding_sphere_radius);
		render_debug_vector(true, &matrix.position, &linear_velocity,1.0f, global_real_argb_yellow);
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

	if (debug_objects_dynamic_render_bounding_spheres && object_definition->object.dynamic_light_sphere_radius > 0.0f)
	{
		real_point3d point{};
		point_from_line3d(&object->object.bounding_sphere_center, (vector3d*)&object_definition->object.dynamic_light_sphere_offset, 1.0f, &point);
		render_debug_sphere(true, &point, object_definition->object.dynamic_light_sphere_radius + object->object.scale, global_real_argb_black);
	}

	s_model_definition* model_definition = NULL;
	if (object_definition->object.model.index != NONE)
		model_definition = object_definition->object.model.cast_to<s_model_definition>();

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
				render_debug_vector(true, &markers[0].node_matrix.position, &markers[0].node_matrix.forward, target.size, global_real_argb_darkgreen);

				if (target.cone_angle <= 3.1414928f)
					render_debug_cone_outline(true, &markers[0].node_matrix.position, &markers[0].node_matrix.forward, target.size, target.cone_angle, global_real_argb_darkgreen);
				else
					render_debug_sphere(true, &markers[0].node_matrix.position, target.size, global_real_argb_darkgreen);
			}
			break;
			case 2:
			{
				vector3d height{};
				vector_from_points3d(&markers[0].node_matrix.position, &markers[1].node_matrix.position, &height);
				render_debug_pill(true, &markers[0].node_matrix.position, &height, target.size, global_real_argb_darkgreen);
			}
			break;
			}
		}
	}

	//collision_model_instance instance{};
	//if (debug_objects_collision_models && collision_model_instance_new(&instance, object_index))
	//{
	//	render_debug_collision_model(&instance);
	//}

	if (debug_objects_early_movers && object_definition->object.flags.test(_object_definition_flag_early_mover_bit))
	{
		char const* early_mover_string = "early mover";
		if (object_definition->object.flags.test(_object_definition_flag_early_mover_localized_physics_bit))
			early_mover_string = "early mover + localized physics";

		real_matrix4x3* root_node_matrix = object_get_node_matrix(object_index, 0);
		render_debug_string_at_point(&root_node_matrix->position, early_mover_string, global_real_argb_darkgreen);
	}

	//real object_cpu_times[2];
	//if (debug_objects_profile_times && object_profile_query_object_instance_cpu_times(object_index, &object_cpu_times))
	//{
	//
	//}

	if (object->object.havok_component_index == NONE)
	{
		//if (physics_model_instance_new(&instance, object_index) && debug_objects_physics_models)
		//	render_debug_physics_model(&instance, global_real_argb_black);
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
		//sector_debug_object_pathfinding(object_index);
	}

	if (!string.is_empty())
	{
		real_matrix4x3* root_node_matrix = object_get_node_matrix(object_index, 0);
		render_debug_string_at_point(&root_node_matrix->position, string.get_string(), global_real_argb_green);
	}

	if (debug_objects_node_bounds)
	{

	}

	if (debug_objects_animation)
	{

	}
}

#pragma warning(push)
#pragma warning(disable : 26495)
object_placement_data::object_placement_data()
{
	csmemset(this, 0, sizeof(*this));

	multiplayer_properties.game_engine_flags.clear();
	multiplayer_properties.spawn_flags.clear();
}
#pragma warning(pop)

