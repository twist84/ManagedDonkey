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

e_object_type __cdecl object_get_type(long object_index)
{
	return INVOKE(0x0046DC70, object_get_type, object_index);
}

void __cdecl object_get_bounding_sphere(long object_index, real_point3d* center, real* radius)
{
	INVOKE(0x00537990, object_get_bounding_sphere, object_index, center, radius);
}

//.text:00B26C60 ; 
//.text:00B26CA0 ; 
//.text:00B26CD0 ; 
//.text:00B26D00 ; 
//.text:00B26D30 ; 
//.text:00B26D40 ; 
//.text:00B26D60 ; 
//.text:00B26D80 ; 
//.text:00B26DA0 ; 
//.text:00B26DC0 ; 
//.text:00B26DE0 ; 
//.text:00B26E00 ; 
//.text:00B26E20 ; 
//.text:00B26E40 ; 
//.text:00B26E60 ; 
//.text:00B26E80 ; 
//.text:00B26EA0 ; 
//.text:00B26EC0 ; 
//.text:00B26ED0 ; 
//.text:00B26F00 ; 
//.text:00B26F30 ; 
//.text:00B26F60 ; 
//.text:00B26F80 ; 
//.text:00B26F90 ; 
//.text:00B26FA0 ; 
//.text:00B26FB0 ; 
//.text:00B26FC0 ; 
//.text:00B26FD0 ; 
//.text:00B26FE0 ; 
//.text:00B26FF0 ; 
//.text:00B27000 ; 
//.text:00B27010 ; 
//.text:00B27020 ; 
//.text:00B27030 ; 
//.text:00B27040 ; 
//.text:00B27050 ; 
//.text:00B27060 ; void __cdecl __tls_set_g_collideable_cluster_reference_allocator(void*)
//.text:00B27090 ; void __cdecl __tls_set_g_collideable_data_reference_allocator(void*)
//.text:00B270C0 ; void __cdecl __tls_set_g_collideable_first_data_allocator(void*)
//.text:00B270E0 ; void __cdecl __tls_set_g_noncollideable_cluster_reference_allocator(void*)
//.text:00B27110 ; void __cdecl __tls_set_g_noncollideable_data_reference_allocator(void*)
//.text:00B27140 ; void __cdecl __tls_set_g_noncollideable_first_data_allocator(void*)
//.text:00B27160 ; void __cdecl __tls_set_g_object_globals_allocator(void*)
//.text:00B27180 ; void __cdecl __tls_set_g_object_header_data_allocator(void*)
//.text:00B271B0 ; void __cdecl __tls_set_g_object_message_queue_allocator(void*)
//.text:00B271D0 ; void __cdecl __tls_set_g_object_name_list_allocator(void*)
//.text:00B271F0 ; void __cdecl __tls_set_g_object_render_data_allocator(void*)
//.text:00B27210 ; public: void __cdecl t_message_queue<s_object_render_thread_message, 2048>::add_message(s_object_render_thread_message const*)
//.text:00B27290 ; void __cdecl adopt_old_object_render_message_queue(void*)
//.text:00B272C0 ; 
//.text:00B27300 ; 
//.text:00B27340 ; 
//.text:00B27380 ; 
//.text:00B273C0 ; 
//.text:00B27400 ; 
//.text:00B27440 ; 
//.text:00B27480 ; 
//.text:00B274C0 ; 
//.text:00B27500 ; 
//.text:00B27540 ; 
//.text:00B27580 ; 
//.text:00B275D0 ; 
//.text:00B27630 ; 
//.text:00B276D0 ; 
//.text:00B27770 ; void __cdecl attachments_delete(long)
//.text:00B27820 ; void __cdecl attachments_model_state_changed(long)
//.text:00B278A0 ; void __cdecl attachments_new(long)
//.text:00B27A10 ; void __cdecl attachments_update(long)
//.text:00B27BD0 ; 
//.text:00B27BF0 ; 
//.text:00B27C20 ; 
//.text:00B27C40 ; 
//.text:00B27C60 ; 
//.text:00B27C70 ; 
//.text:00B27C80 ; 

void c_object_identifier::clear()
{
	//DECLFUNC(0x00B27CB0, void, __thiscall, c_object_identifier*)(this);

	m_type = k_object_type_none;
	m_source = k_object_source_none;
	m_origin_bsp_index = NONE;
	m_unique_id = NONE;
}

//.text:00B27CC0 ; 
//.text:00B27CE0 ; void __cdecl clear_all_object_render_data()
//.text:00B27D20 ; public: void __cdecl c_object_identifier::clear_for_deletion()
//.text:00B27D30 ; public: void __cdecl c_static_flags_no_init<255>::clear_range(long)
//.text:00B27D90 ; void __cdecl cluster_build_object_payload(long, s_object_cluster_payload* payload)

long __cdecl cluster_get_first_collideable_object(long* datum_index, s_cluster_reference cluster_reference)
{
	return INVOKE(0x00B27EB0, cluster_get_first_collideable_object, datum_index, cluster_reference);
}

long __cdecl cluster_get_first_collideable_object_and_payload(long* datum_index, s_cluster_reference cluster_reference, s_object_cluster_payload const** payload)
{
	return INVOKE(0x00B27EE0, cluster_get_first_collideable_object_and_payload, datum_index, cluster_reference, payload);
}

//.text:00B27F10 ; 

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

//.text:00B27FD0 

long __cdecl cluster_get_next_noncollideable_object_and_payload(long* datum_index, s_object_cluster_payload const** payload)
{
	return INVOKE(0x00B28000, cluster_get_next_noncollideable_object_and_payload, datum_index, payload);
}

//.text:00B28030 ; 
//.text:00B28040 ; real __cdecl compute_all_quiet() // probably needs to be a double for actual use
//.text:00B28100 ; real __cdecl compute_electrical_power() // probably needs to be a double for actual use
//.text:00B28110 ; real __cdecl compute_holiday() // probably needs to be a double for actual use
//.text:00B281E0 ; real __cdecl compute_time_hours() // probably needs to be a double for actual use
//.text:00B28240 ; real __cdecl compute_time_minutes() // probably needs to be a double for actual use
//.text:00B28290 ; real __cdecl compute_time_seconds() // probably needs to be a double for actual use
//.text:00B282E0 ; public: void __cdecl c_object_identifier::create_dynamic(e_object_type)
//.text:00B28320 ; public: void __cdecl c_object_identifier::create_from_parent(e_object_type)
//.text:00B28360 ; public: void __cdecl c_object_identifier::create_from_scenario(e_object_type, long)
//.text:00B28380 ; public: void __cdecl c_object_identifier::create_from_sky(e_object_type, long)
//.text:00B283A0 ; public: void __cdecl c_object_identifier::create_from_structure(e_object_type, short, long)
//.text:00B283C0 ; 
//.text:00B283F0 ; 
//.text:00B28420 ; 
//.text:00B28450 ; 
//.text:00B28480 ; 
//.text:00B284B0 ; 
//.text:00B284E0 ; 
//.text:00B28510 ; 
//.text:00B28540 ; 
//.text:00B28570 ; 
//.text:00B285A0 ; 
//.text:00B285C0 ; 
//.text:00B28600 ; 
//.text:00B28610 ; public: void __cdecl t_message_container<s_object_render_thread_message, 2048>::filter_messages(bool(__cdecl*const)(s_object_render_thread_message*, dword), dword)
//.text:00B286A0 ; bool __cdecl filter_object_messages(s_object_render_thread_message*, dword)

long __cdecl find_first_predicted_object_recursive(long object_index)
{
	return INVOKE(0x00B286C0, find_first_predicted_object_recursive, object_index);
}

//.text:00B28740 ; public: long __cdecl c_object_identifier::find_object_index() const
//.text:00B28800 ; public: s_scenario_object* __cdecl c_object_identifier::find_scenario_object(long*) const
//.text:00B28820 ; public: s_scenario_object* __cdecl c_object_identifier::find_scenario_object_from_scenario(scenario*, long*) const
//.text:00B288F0 ; 
//.text:00B28900 ; 
//.text:00B28910 ; 
//.text:00B28920 ; 
//.text:00B28950 ; 
//.text:00B28980 ; 
//.text:00B289B0 ; 
//.text:00B289E0 ; 
//.text:00B28A10 ; 
//.text:00B28A40 ; 
//.text:00B28A70 ; 
//.text:00B28AA0 ; 
//.text:00B28AD0 ; 
//.text:00B28B00 ; 
//.text:00B28B20 ; 
//.text:00B28B50 ; void __cdecl game_state_after_load_objects_setup_structure_bsp_fake_lightprobes(long)

void __cdecl garbage_collect_now()
{
	INVOKE(0x00B28BD0, garbage_collect_now);
}

void __cdecl garbage_collect_unsafe()
{
	INVOKE(0x00B28C00, garbage_collect_unsafe);
}

bool __cdecl garbage_collection_can_run()
{
	return INVOKE(0x00B28C30, garbage_collection_can_run);
}

//.text:00B28C50 ; 
//.text:00B28C60 ; 
//.text:00B28C70 ; 
//.text:00B28C80 ; 
//.text:00B28C90 ; 
//.text:00B28CA0 ; 
//.text:00B28DD0 ; 
//.text:00B28DE0 ; 
//.text:00B28DF0 ; real_orientation* __cdecl get_node_orientation_scratchpad_for_model(render_model_definition const*, long)
//.text:00B28E60 ; public: long __cdecl c_object_identifier::get_unique_id_direct() const
//.text:00B28E70 ; similar to `objects_compact_memory_pool`
//.text:00B28ED0 ; void __cdecl handle_object_render_message(s_object_render_thread_message*, long)
//.text:00B28F50 ; 
//.text:00B28F70 ; 
//.text:00B28F90 ; 
//.text:00B28FA0 ; void __cdecl hs_object_definition_predict_low(long)
//.text:00B28FB0 ; void __cdecl internal_object_compute_animated_node_orientations(long, real, render_model_definition const*, c_animation_manager*, c_static_flags<256> const*, long, real_orientation*, bool)
//.text:00B29080 ; short __cdecl internal_object_get_markers_by_string_id(long, long, object_marker*, short, bool)
//.text:00B29260 ; 
//.text:00B29280 ; 
//.text:00B292D0 ; 
//.text:00B292E0 ; public: bool __cdecl c_object_identifier::is_equal(c_object_identifier const*) const
//.text:00B29330 ; 
//.text:00B29350 ; 
//.text:00B29370 ; 
//.text:00B29390 ; 
//.text:00B293B0 ; 

long __cdecl object_accepted_local_physics_object_get(long object_index)
{
	return INVOKE(0x00B293E0, object_accepted_local_physics_object_get, object_index);
}

void __cdecl object_activate(long object_index)
{
	INVOKE(0x00B29440, object_activate, object_index);
}

//.text:00B294C0 ; 

void __cdecl object_adjust_garbage_timer(long object_index, long time)
{
	INVOKE(0x00B29500, object_adjust_garbage_timer, object_index, time);
}
//.text:00B29540 ; 
//.text:00B295B0 ; void __cdecl object_adjust_node_orientations_for_facing_change(long, vector3d const*, vector3d const*)
//.text:00B296D0 ; void __cdecl object_adjust_placement(object_placement_data*)
//.text:00B29720 ; void __cdecl object_align_marker_to_matrix(long, object_marker const*, real_matrix4x3 const*)
//.text:00B29900 ; void __cdecl object_animation_callback(s_animation_event_data const*, long)
//.text:00B29B50 ; void __cdecl object_apply_acceleration(long, long, real_point3d const*, vector3d const*, vector3d const*)
//.text:00B29D40 ; void __cdecl object_apply_function_overlay_node_orientations(long, render_model_definition const*, c_animation_manager const*, c_static_flags<256> const*, long, real_orientation*)
//.text:00B29F20 ; void __cdecl object_attach_gamestate_entity(long object_index, long gamestate_entity_index)
//.text:00B29F60 ; void __cdecl object_attach_to_marker(long, long, long, long)
//.text:00B29FD0 ; void __cdecl object_attach_to_marker_immediate(long, long, long, long)
//.text:00B2A250 ; void __cdecl object_attach_to_node(long, long, short)
//.text:00B2A2B0 ; void __cdecl object_attach_to_node_immediate(long, long, short)
//.text:00B2A700 ; 

bool __cdecl object_can_be_melee_instant_killed(long object_index)
{
	return INVOKE(0x00B2A780, object_can_be_melee_instant_killed, object_index);
}

bool __cdecl object_can_interpolate(long object_index)
{
	return INVOKE(0x00B2A7E0, object_can_interpolate, object_index);
}

//.text:00B2A820 ; void __cdecl object_choose_initial_permutation(long, word, s_model_customization_region_permutation const*, long)

void __cdecl object_choose_variant(long object_index, long name)
{
	INVOKE(0x00B2A960, object_choose_variant, object_index, name);
}

void __cdecl object_cinematic_collision(long object_index, bool enable)
{
	INVOKE(0x00B2A9C0, object_cinematic_collision, object_index, enable);
}

void __cdecl object_cinematic_lod(long object_index, bool enable)
{
	INVOKE(0x00B2AA20, object_cinematic_lod, object_index, enable);
}

void __cdecl object_cinematic_visibility(long object_index, bool enable)
{
	INVOKE(0x00B2AA70, object_cinematic_visibility, object_index, enable);
}

//.text:00B2AAC0 ; void __cdecl object_clear_sync_action(long)
//.text:00B2AB10 ; bool __cdecl object_compute_bounding_sphere(long)
//.text:00B2AD00 ; void __cdecl object_compute_bounding_sphere_recursive(long, real_point3d const*, real*)
//.text:00B2AEC0 ; bool __cdecl object_compute_change_colors(long)
//.text:00B2B120 ; bool __cdecl object_compute_function_value(long, long, long, real*, bool*, bool*)
//.text:00B2B830 ; void __cdecl object_compute_instance_bitfield_visible(long)
//.text:00B2B8E0 ; void __cdecl object_compute_node_matrices(long)
//.text:00B2B970 ; void __cdecl object_compute_node_matrices_non_recursive(long)
//.text:00B2C240 ; void __cdecl object_compute_origin_matrix(long, real_point3d const*, vector3d const*, vector3d const*, real, bool, real_matrix4x3 const*, bool, real_matrix4x3*)
//.text:00B2C3D0 ; real_matrix4x3 const* __cdecl object_compute_render_time_node_matrices(long, long, byte const*, real_matrix4x3 const*)

void __cdecl object_connect_lights(long object_index, bool disconnect_this_object, bool reconnect_this_object)
{
	INVOKE(0x00B2C790, object_connect_lights, object_index, disconnect_this_object, reconnect_this_object);

	//object_connect_lights_recursive(object_index, disconnect_this_object, reconnect_this_object, false, false);
}

void __cdecl object_connect_lights_recursive(long object_index, bool disconnect_this_object, bool reconnect_this_object, bool a3, bool a4)
{
	INVOKE(0x00B2C7B0, object_connect_lights_recursive, object_index, disconnect_this_object, reconnect_this_object, a3, a4);
}

long __cdecl object_count(long type_flags, byte header_mask)
{
	return INVOKE(0x00B2C8E0, object_count, type_flags, header_mask);
}

void __cdecl object_create_attachments(long object_index)
{
	INVOKE(0x00B2C940, object_create_attachments, object_index);
}

void __cdecl object_create_children(long object_index)
{
	INVOKE(0x00B2C980, object_create_children, object_index);
}

bool __cdecl object_custom_animations_hold_on_last_frame()
{
	return INVOKE(0x00B2CBA0, object_custom_animations_hold_on_last_frame);
}

//.text:00B2CBC0 ; 

//void __cdecl object_deactivate(long object_index)
//{
//	INVOKE(0x00B2CBE0, object_deactivate, object_index);
//
//	//object_deactivate(object_index, false);
//}
//
//void __cdecl object_deactivate(long object_index, bool a2)
//{
//	INVOKE(0x00B2CC00, object_deactivate, object_index, a2);
//}

//.text:00B2CCE0 ; 
//.text:00B2CCF0 ; 

bool __cdecl object_definition_predict_low(long object_definition_index)
{
	return INVOKE(0x00B2CD00, object_definition_predict_low, object_definition_index);
}

void __cdecl object_delete(long object_index)
{
	INVOKE(0x00B2CD10, object_delete, object_index);
}

void __cdecl object_delete_all_multiplayer_cinematic_objects()
{
	INVOKE(0x00B2CE90, object_delete_all_multiplayer_cinematic_objects);
}

void __cdecl object_delete_attachments(long object_index)
{
	INVOKE(0x00B2CF20, object_delete_attachments, object_index);
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

real_matrix4x3* __cdecl object_get_world_matrix_interpolated(long object_index, real_matrix4x3* out_matrix)
{
	return INVOKE(0x00B2ECE0, object_get_world_matrix_interpolated, object_index, out_matrix);
}

bool __cdecl object_has_animation_manager(long object_index)
{
	return INVOKE(0x00B2EDF0, object_has_animation_manager, object_index);
}

bool __cdecl object_has_multiplayer_properties(long object_index)
{
	return INVOKE(0x00B2EE30, object_has_multiplayer_properties, object_index);
}

bool __cdecl object_header_block_allocate(long object_index, short block_reference_offset, short size, short alignment_bits)
{
	return INVOKE(0x00B2EE70, object_header_block_allocate, object_index, block_reference_offset, size, alignment_bits);
}

void __cdecl object_header_delete(long object_index)
{
	return INVOKE(0x00B2EF90, object_header_delete, object_index);
}

long __cdecl object_header_new(short size)
{
	return INVOKE(0x00B2EFF0, object_header_new, size);
}

long __cdecl object_index_from_name_index(short name_index)
{
	return INVOKE(0x00B2F0A0, object_index_from_name_index, name_index);
}

long __cdecl object_index_from_scenario_object_index(e_object_type object_type, long object_index)
{
	return INVOKE(0x00B2F0E0, object_index_from_scenario_object_index, object_type, object_index);
}

bool __cdecl object_is_at_rest(long object_index)
{
	return INVOKE(0x00B2F120, object_is_at_rest, object_index);
}

bool __cdecl object_is_being_deleted(long object_index)
{
	return INVOKE(0x00B2F160, object_is_being_deleted, object_index);
}

bool __cdecl object_is_connected_to_map(long object_index)
{
	return INVOKE(0x00B2F190, object_is_connected_to_map, object_index);
}

bool __cdecl object_is_hidden(long object_index)
{
	return INVOKE(0x00B2F1D0, object_is_hidden, object_index);
}

bool __cdecl object_is_hidden_internal(object_header_datum const* object_header, object_datum const* object)
{
	return INVOKE(0x00B2F220, object_is_hidden_internal, object_header, object);
}

bool __cdecl object_is_hidden_non_recursive(long object_index)
{
	return INVOKE(0x00B2F240, object_is_hidden_non_recursive, object_index);
}

//.text:00B2F280 ; bool __cdecl sub_B2F280(long object_index, long render_model_index);

bool __cdecl object_is_multiplayer_cinematic_object(long object_index)
{
	return INVOKE(0x00B2F300, object_is_multiplayer_cinematic_object, object_index);
}

bool __cdecl object_is_or_contains_player(long object_index)
{
	return INVOKE(0x00B2F340, object_is_or_contains_player, object_index);
}

bool __cdecl object_is_running_sync_action(long object_index)
{
	return INVOKE(0x00B2F430, object_is_running_sync_action, object_index);
}

bool __cdecl object_is_vehicle(long object_index)
{
	return INVOKE(0x00B2F470, object_is_vehicle, object_index);
}

//.text:00B2F4B0 ; protected: void __cdecl c_object_iterator_base::object_iterator_begin_internal(dword, dword, dword, long)

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

long __cdecl object_list_children_by_definition(long object_index, long definition_index)
{
	return INVOKE(0x00B2F830, object_list_children_by_definition, object_index, definition_index);
}

bool __cdecl object_load_scenario_placement_matrices(long object_index)
{
	return INVOKE(0x00B2F890, object_load_scenario_placement_matrices, object_index);
}

long __cdecl object_local_physics_object_get(long object_index)
{
	return INVOKE(0x00B2FBD0, object_local_physics_object_get, object_index);
}

long __cdecl object_lookup_variant_index_from_name(long object_index, long name)
{
	return INVOKE(0x00B2FC20, object_lookup_variant_index_from_name, object_index, name);
}

//void __cdecl object_make_noise(long object_index, e_sound_type sound_type, e_ai_sound_volume ai_sound_volume)
void __cdecl object_make_noise(long object_index, long sound_type, long ai_sound_volume)
{
	INVOKE(0x00B2FC70, object_make_noise, object_index, sound_type, ai_sound_volume);
}

//.text:00B2FCE0 ; bool __cdecl object_mark(long)
//.text:00B2FD20 ; bool __cdecl object_mark_get_user_data(long, long*)
//.text:00B2FD60 ; bool __cdecl object_mark_with_user_data(long, long)

void __cdecl object_marker_begin()
{
	INVOKE(0x00B2FDC0, object_marker_begin);
}

void __cdecl object_marker_end()
{
	INVOKE(0x00B2FDE0, object_marker_end);
}

void __cdecl object_marker_reopen()
{
	INVOKE(0x00B2FE00, object_marker_reopen);
}

//.text:00B2FE20 ; void __cdecl object_model_state_changed(long, long, char const*)

void __cdecl object_move(long object_index)
{
	INVOKE(0x00B2FE50, object_move, object_index);
}

void __cdecl object_move_position(long object_index, real_point3d const* position, vector3d const* forward, vector3d const* up, s_location const* location)
{
	INVOKE(0x00B30050, object_move_position, object_index, position, forward, up, location);
}

void __cdecl object_name_list_allocate()
{
	INVOKE(0x00B301F0, object_name_list_allocate);
}

void __cdecl object_name_list_clear()
{
	INVOKE(0x00B30250, object_name_list_clear);
}

void __cdecl object_name_list_delete(long object_index)
{
	INVOKE(0x00B30280, object_name_list_delete, object_index);
}

long __cdecl object_name_list_lookup(short name_index)
{
	return INVOKE(0x00B30310, object_name_list_lookup, name_index);
}

void __cdecl object_name_list_new(long name, short name_index)
{
	INVOKE(0x00B30350, object_name_list_new, name, name_index);
}

bool __cdecl object_needs_rigid_body_update(long object_index)
{
	return INVOKE(0x00B303A0, object_needs_rigid_body_update, object_index);
}

long __cdecl object_new(object_placement_data* data)
{
	return INVOKE(0x00B30440, object_new, data);
}

//.text:00B30E60 ; long __cdecl object_new_by_name(short, bool, bool)
//.text:00B30FD0 ; long __cdecl object_new_from_scenario(e_object_type, long, s_scenario_object*, s_tag_block*, bool)
//.text:00B31000 ; long __cdecl object_new_from_scenario_bypass_simulation_object_placement_test(e_object_type, long, s_scenario_object*, s_tag_block*, long, bool)
//.text:00B31030 ; 
//.text:00B310C0 ; long __cdecl object_new_from_scenario_internal(e_object_type, short, s_scenario_object const*, s_tag_block*, long, bool, bool, long)

bool __cdecl object_node_orientations_frozen(long object_index)
{
	return INVOKE(0x00B31240, object_node_orientations_frozen, object_index);
}

void __cdecl object_notify_in_local_physics_object(long object_index, long local_physics_object_index)
{
	INVOKE(0x00B31270, object_notify_in_local_physics_object, object_index, local_physics_object_index);
}

//.text:00B312F0 ; void __cdecl object_offset_interpolation(long, vector3d const*)
//.text:00B31380 ; bool __cdecl object_owns_object(long, long)
//.text:00B313E0 ; 

void __cdecl object_place(long object_index, s_scenario_object const* scenario_object)
{
	INVOKE(0x00B31470, object_place, object_index, scenario_object);

	//object_datum* object = object_get(object_index);
	//struct object_definition* object_definition = (struct object_definition*)tag_get(OBJECT_TAG, object->definition_index);
	//if (object_definition->object.ai_properties.count() > 0)
	//{
	//	bool non_flight_blocking = object_definition->object.ai_properties[0].ai_flags.test(_ai_properties_non_flight_blocking_bit);
	//	SET_BIT(object->object.object_ai_flags, 2, non_flight_blocking);
	//}
}

void __cdecl object_placement_data_copy_change_colors(object_placement_data* data, long object_index)
{
	INVOKE(0x00B314E0, object_placement_data_copy_change_colors, data, object_index);

	//object_datum* object = object_get(object_index);
	//long change_color_count = 0;
	//if (real_rgb_color* change_colors = (real_rgb_color*)object_header_block_get_with_count(object_index, &object->object.change_colors, sizeof(real_rgb_color), &change_color_count))
	//{
	//	ASSERT(change_color_count % 2 == 0);
	//	for (long change_color_index = 0; change_color_index < change_color_count / 2; change_color_index++)
	//	{
	//		data->change_colors[change_color_index] = change_colors[change_color_index];
	//		data->active_change_colors.set(change_color_index, true);
	//	}
	//}
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

void __cdecl object_remove_pending_messages(long object_index)
{
	INVOKE(0x00B32100, object_remove_pending_messages, object_index);
}

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

//.text:00B36EE0 ; void __cdecl resursive_add_child_objects_to_list(long, long, long)

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
		struct scenario* scenario = global_scenario_get();
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

