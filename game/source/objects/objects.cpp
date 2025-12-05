#include "objects/objects.hpp"

#include "cache/cache_files.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "cseries/cseries_events.hpp"
#include "items/items.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "models/model_definitions.hpp"
#include "objects/object_types.hpp"
#include "objects/watch_window.hpp"
#include "physics/collision_models.hpp"
#include "physics/havok.hpp"
#include "physics/havok_component.hpp"
#include "physics/physics_models.hpp"
#include "profiler/profiler.hpp"
#include "render/render_debug.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"

#include <intrin.h>
#include <math.h>

HOOK_DECLARE(0x00B31590, object_placement_data_new);
HOOK_DECLARE(0x00B32130, object_render_debug);

s_object_override_globals object_override_globals;

bool debug_objects = false;
bool debug_objects_early_movers = true;
bool debug_objects_bounding_spheres = true;
bool debug_objects_render_models = true;
bool debug_objects_collision_models = true;
bool debug_objects_physics_models = true;
bool debug_objects_sound_spheres = false;
bool debug_objects_indices = false;
bool debug_objects_programmer = false;
bool debug_objects_garbage = false;
bool debug_objects_names = false;
bool debug_objects_names_full = false;
bool debug_objects_active_nodes = false;
bool debug_objects_animation_times = false;
bool debug_objects_functions = false;
bool debug_objects_position_velocity = false;
bool debug_objects_origin = false;
bool debug_objects_root_node = false;
bool debug_objects_root_node_print = false;
bool debug_objects_attached_bounding_spheres = false;
bool debug_objects_dynamic_render_bounding_spheres = false;
bool debug_objects_model_targets = false;
bool debug_objects_profile_times = false;
bool debug_objects_water_physics = false;
bool debug_objects_expensive_physics = false;
bool debug_objects_contact_points = false;
bool debug_objects_constraints = false;
bool debug_objects_vehicle_physics = false;
bool debug_objects_mass = false;
bool debug_objects_pathfinding = false;
bool debug_objects_node_bounds = false;
bool debug_objects_animation = false;
bool debug_objects_skeletons = false;

bool point_in_bounds(const real_point3d* point, real32 bounds)
{
	return point->x >= -bounds && point->x <= bounds
		&& point->y >= -bounds && point->y <= bounds
		&& point->z >= -bounds && point->z <= bounds;
}

void* __cdecl object_header_block_get(int32 object_index, const object_header_block_reference* reference)
{
	const object_header_datum* object_header = object_header_get(object_index);
	object_datum* object = object_get(object_index);

	ASSERT(reference->offset > 0);
	ASSERT(reference->size > 0);
	ASSERT(reference->offset + reference->size <= object_header->data_size);

	return (byte*)object + reference->offset;
}

void* __cdecl object_header_block_get_with_count(int32 object_index, const object_header_block_reference* reference, unsigned int element_size, int32* element_count)
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

const object_header_datum* __cdecl object_header_get(int32 object_index)
{
	const object_header_datum* result = DATUM_TRY_AND_GET(object_header_data, const object_header_datum, object_index);
	return result;
}

object_header_datum* __cdecl object_header_get_mutable(int32 object_index)
{
	object_header_datum* result = DATUM_TRY_AND_GET(object_header_data, object_header_datum, object_index);
	return result;
}

object_datum* __cdecl object_get(int32 object_index)
{
	object_datum* result = (object_datum*)object_get_and_verify_type(object_index, _object_mask_all);
	return result;
}

void* __cdecl object_get_and_verify_type(int32 object_index, uns32 valid_type_flags)
{
	//VASSERT(game_state_is_locked(), "someone is calling object_get when the game state is locked");

	object_datum* result = NULL;
	if (const object_header_datum* object_header = object_header_get(object_index))
	{
		object_datum* object = object_header->datum;
		if (TEST_BIT(valid_type_flags, object->object.object_identifier.get_type()))
		{
			result = object;
		}
		else
		{
			//c_string_builder("got an object type we didn't expect (expected one of 0x%08x but got #%d).",
			//	valid_type_flags,
			//	object->object.object_identifier.get_type()).get_string();
		}
	}
	return result;
}

e_object_type __cdecl object_get_type(int32 object_index)
{
	return INVOKE(0x0046DC70, object_get_type, object_index);
}

void __cdecl object_get_bounding_sphere(int32 object_index, real_point3d* center, real32* radius)
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
//.text:00B27210 ; public: void __cdecl t_message_queue<s_object_render_thread_message, 2048>::add_message(const s_object_render_thread_message*)
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
//.text:00B27630 ; real32 __cdecl attachment_get_primary_scale(int32, int32)
//.text:00B276D0 ; real32 __cdecl attachment_get_secondary_scale(int32, int32)
//.text:00B27770 ; void __cdecl attachments_delete(int32)
//.text:00B27820 ; void __cdecl attachments_model_state_changed(int32)
//.text:00B278A0 ; void __cdecl attachments_new(int32)
//.text:00B27A10 ; void __cdecl attachments_update(int32)
//.text:00B27BD0 ; 
//.text:00B27BF0 ; 
//.text:00B27C20 ; 
//.text:00B27C40 ; 
//.text:00B27C60 ; 
//.text:00B27C70 ; 
//.text:00B27C80 ; 

void c_object_identifier::clear()
{
	//INVOKE_CLASS_MEMBER(0x00B27CB0, c_object_identifier, clear);

	m_type = k_object_type_none;
	m_source = _object_source_none;
	m_origin_bsp_index = NONE;
	m_unique_id = NONE;
}

//.text:00B27CC0 ; 
//.text:00B27CE0 ; void __cdecl clear_all_object_render_data()

void c_object_identifier::clear_for_deletion()
{
	//INVOKE_CLASS_MEMBER(0x00B27D20, c_object_identifier, clear_for_deletion);

	m_source = _object_source_none;
	m_origin_bsp_index = NONE;
	m_unique_id = NONE;
}

//.text:00B27D30 ; public: void __cdecl c_static_flags_no_init<255>::clear_range(int32)
//.text:00B27D90 ; void __cdecl cluster_build_object_payload(int32, s_object_cluster_payload* payload)

int32 __cdecl cluster_get_first_collideable_object(int32* datum_index, s_cluster_reference cluster_reference)
{
	return INVOKE(0x00B27EB0, cluster_get_first_collideable_object, datum_index, cluster_reference);
}

int32 __cdecl cluster_get_first_collideable_object_and_payload(int32* datum_index, s_cluster_reference cluster_reference, const s_object_cluster_payload** payload)
{
	return INVOKE(0x00B27EE0, cluster_get_first_collideable_object_and_payload, datum_index, cluster_reference, payload);
}

//.text:00B27F10 ; int32 __cdecl cluster_get_first_noncollideable_object(int32*, s_cluster_reference)

int32 __cdecl cluster_get_first_noncollideable_object_and_payload(int32* datum_index, s_cluster_reference cluster_reference, const s_object_cluster_payload** payload)
{
	return INVOKE(0x00B27F40, cluster_get_first_noncollideable_object_and_payload, datum_index, cluster_reference, payload);
}

int32 __cdecl cluster_get_next_collideable_object(int32* datum_index)
{
	return INVOKE(0x00B27F70, cluster_get_next_collideable_object, datum_index);
}

int32 __cdecl cluster_get_next_collideable_object_and_payload(int32* datum_index, const s_object_cluster_payload** payload)
{
	return INVOKE(0x00B27FA0, cluster_get_next_collideable_object_and_payload, datum_index, payload);
}

//.text:00B27FD0 ; int32 __cdecl cluster_get_next_noncollideable_object(int32*)

int32 __cdecl cluster_get_next_noncollideable_object_and_payload(int32* datum_index, const s_object_cluster_payload** payload)
{
	return INVOKE(0x00B28000, cluster_get_next_noncollideable_object_and_payload, datum_index, payload);
}

//.text:00B28030 ; 
//.text:00B28040 ; real32 __cdecl compute_all_quiet() // probably needs to be a real64 for actual use
//.text:00B28100 ; real32 __cdecl compute_electrical_power() // probably needs to be a real64 for actual use
//.text:00B28110 ; real32 __cdecl compute_holiday() // probably needs to be a real64 for actual use
//.text:00B281E0 ; real32 __cdecl compute_time_hours() // probably needs to be a real64 for actual use
//.text:00B28240 ; real32 __cdecl compute_time_minutes() // probably needs to be a real64 for actual use
//.text:00B28290 ; real32 __cdecl compute_time_seconds() // probably needs to be a real64 for actual use

void c_object_identifier::create_dynamic(e_object_type type)
{
	//INVOKE_CLASS_MEMBER(0x00B282E0, c_object_identifier, create_dynamic, type);

	m_type = type;
	m_source = _object_source_dynamic;
	m_origin_bsp_index = NONE;
	m_unique_id = ++object_globals->object_identifier_salt;
}

void c_object_identifier::create_from_parent(e_object_type type)
{
	//INVOKE_CLASS_MEMBER(0x00B28320, c_object_identifier, create_from_parent, type);

	m_type = type;
	m_source = _object_source_parent;
	m_origin_bsp_index = NONE;
	m_unique_id = ++object_globals->object_identifier_salt;
}

void c_object_identifier::create_from_scenario(e_object_type type, int32 unique_id)
{
	//INVOKE_CLASS_MEMBER(0x00B28360, c_object_identifier, create_from_scenario, type, unique_id);

	m_type = type;
	m_origin_bsp_index = NONE;
	m_source = _object_source_editor;
	m_unique_id = unique_id;
}

void c_object_identifier::create_from_sky(e_object_type type, int32 unique_id)
{
	//INVOKE_CLASS_MEMBER(0x00B28380, c_object_identifier, create_from_sky, type, unique_id);

	m_type = type;
	m_origin_bsp_index = NONE;
	m_source = _object_source_sky;
	m_unique_id = unique_id;
}

void c_object_identifier::create_from_structure(e_object_type type, int16 origin_bsp_index, int32 unique_id)
{
	//INVOKE_CLASS_MEMBER(0x00B283A0, c_object_identifier, create_from_structure, type, origin_bsp_index, unique_id);

	m_type = type;
	m_origin_bsp_index = origin_bsp_index;
	m_source = _object_source_structure_object;
	m_unique_id = unique_id;
}

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
//.text:00B28610 ; public: void __cdecl t_message_container<s_object_render_thread_message, 2048>::filter_messages(bool(__cdecl*const)(s_object_render_thread_message*, uns32), uns32)
//.text:00B286A0 ; bool __cdecl filter_object_messages(s_object_render_thread_message*, uns32)

int32 __cdecl find_first_predicted_object_recursive(int32 object_index)
{
	return INVOKE(0x00B286C0, find_first_predicted_object_recursive, object_index);
}

int32 c_object_identifier::find_object_index() const
{
	return INVOKE_CLASS_MEMBER(0x00B28740, c_object_identifier, find_object_index);

	// $IMPLEMENT
}

s_scenario_object* c_object_identifier::find_scenario_object(int32* tag_block_index) const
{
	//return INVOKE_CLASS_MEMBER(0x00B28800, c_object_identifier, find_scenario_object, tag_block_index);

	return find_scenario_object_from_scenario(global_scenario, tag_block_index);
}

s_scenario_object* c_object_identifier::find_scenario_object_from_scenario(struct scenario* scenario, int32* tag_block_index) const
{
	return INVOKE_CLASS_MEMBER(0x00B28820, c_object_identifier, find_scenario_object_from_scenario, scenario, tag_block_index);

	// $IMPLEMENT
}

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
//.text:00B28B50 ; void __cdecl game_state_after_load_objects_setup_structure_bsp_fake_lightprobes(int32)

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
//.text:00B28DF0 ; real_orientation* __cdecl get_node_orientation_scratchpad_for_model(const render_model_definition*, int32)

int32 c_object_identifier::get_unique_id_direct() const
{
	//return INVOKE_CLASS_MEMBER(0x00B28E60, c_object_identifier, get_unique_id_direct);

	return m_unique_id;
}

//.text:00B28E70 ; similar to `objects_compact_memory_pool`
//.text:00B28ED0 ; void __cdecl handle_object_render_message(s_object_render_thread_message*, int32)
//.text:00B28F50 ; 
//.text:00B28F70 ; 

void __cdecl hs_object_definition_predict_all(int32 definition_index)
{
	INVOKE(0x00B28F90, hs_object_definition_predict_all, definition_index);
}

void __cdecl hs_object_definition_predict_low(int32 definition_index)
{
	INVOKE(0x00B28FA0, hs_object_definition_predict_low, definition_index);
}

//.text:00B28FB0 ; void __cdecl internal_object_compute_animated_node_orientations(int32, real32, const render_model_definition*, c_animation_manager*, const c_static_flags<256>*, int32, real_orientation*, bool)
//.text:00B29080 ; int16 __cdecl internal_object_get_markers_by_string_id(int32, int32, object_marker*, int16, bool)
//.text:00B29260 ; 
//.text:00B29280 ; 
//.text:00B292D0 ; 

bool c_object_identifier::is_equal(const c_object_identifier* other) const
{
	return INVOKE_CLASS_MEMBER(0x00B292E0, c_object_identifier, is_equal, other);

	// $IMPLEMENT
}

//.text:00B29330 ; 
//.text:00B29350 ; 
//.text:00B29370 ; 
//.text:00B29390 ; 
//.text:00B293B0 ; 

int32 __cdecl object_accepted_local_physics_object_get(int32 object_index)
{
	return INVOKE(0x00B293E0, object_accepted_local_physics_object_get, object_index);
}

void __cdecl object_activate(int32 object_index)
{
	INVOKE(0x00B29440, object_activate, object_index);
}

//.text:00B294C0 ; void __cdecl object_add_to_list(int32*, int32)

void __cdecl object_adjust_garbage_timer(int32 object_index, int32 time)
{
	INVOKE(0x00B29500, object_adjust_garbage_timer, object_index, time);
}

//.text:00B29540 ; void __cdecl object_adjust_node_matrices(int32, int32, real_matrix4x3*)
//.text:00B295B0 ; void __cdecl object_adjust_node_orientations_for_facing_change(int32, const real_vector3d*, const real_vector3d*)
//.text:00B296D0 ; void __cdecl object_adjust_placement(object_placement_data*)
//.text:00B29720 ; void __cdecl object_align_marker_to_matrix(int32, const object_marker*, const real_matrix4x3*)

void __cdecl object_animation_callback(const s_animation_event_data* event_data, int32 user_param)
{
	INVOKE(0x00B29900, object_animation_callback, event_data, user_param);
}

//.text:00B29B50 ; void __cdecl object_apply_acceleration(int32, int32, const real_point3d*, const real_vector3d*, const real_vector3d*)
//.text:00B29D40 ; void __cdecl object_apply_function_overlay_node_orientations(int32, const render_model_definition*, const c_animation_manager*, const c_static_flags<256>*, int32, real_orientation*)
//.text:00B29F20 ; void __cdecl object_attach_gamestate_entity(int32 object_index, int32 gamestate_entity_index)
//.text:00B29F60 ; void __cdecl object_attach_to_marker(int32, int32, int32, int32)
//.text:00B29FD0 ; void __cdecl object_attach_to_marker_immediate(int32, int32, int32, int32)
//.text:00B2A250 ; void __cdecl object_attach_to_node(int32, int32, int16)
//.text:00B2A2B0 ; void __cdecl object_attach_to_node_immediate(int32, int32, int16)
//.text:00B2A700 ; real32 __cdecl object_bouding_sphere_within_tolerance(const real_point3d*, real32, const real_point3d*, real32, real32)

bool __cdecl object_can_be_melee_instant_killed(int32 object_index)
{
	return INVOKE(0x00B2A780, object_can_be_melee_instant_killed, object_index);
}

bool __cdecl object_can_interpolate(int32 object_index)
{
	return INVOKE(0x00B2A7E0, object_can_interpolate, object_index);
}

//.text:00B2A820 ; void __cdecl object_choose_initial_permutation(int32, uns16, const s_model_customization_region_permutation*, int32)

void __cdecl object_choose_variant(int32 object_index, int32 name)
{
	INVOKE(0x00B2A960, object_choose_variant, object_index, name);
}

void __cdecl object_cinematic_collision(int32 object_index, bool enable)
{
	INVOKE(0x00B2A9C0, object_cinematic_collision, object_index, enable);
}

void __cdecl object_cinematic_lod(int32 object_index, bool enable)
{
	INVOKE(0x00B2AA20, object_cinematic_lod, object_index, enable);
}

void __cdecl object_cinematic_visibility(int32 object_index, bool enable)
{
	INVOKE(0x00B2AA70, object_cinematic_visibility, object_index, enable);
}

void __cdecl object_clear_sync_action(int32 object_index)
{
	INVOKE(0x00B2AAC0, object_clear_sync_action, object_index);
}

//.text:00B2AB10 ; bool __cdecl object_compute_bounding_sphere(int32)
//.text:00B2AD00 ; void __cdecl object_compute_bounding_sphere_recursive(int32, const real_point3d*, real32*)
//.text:00B2AEC0 ; bool __cdecl object_compute_change_colors(int32)
//.text:00B2B120 ; bool __cdecl object_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B2B830 ; void __cdecl object_compute_instance_bitfield_visible(int32)
//.text:00B2B8E0 ; void __cdecl object_compute_node_matrices(int32)
//.text:00B2B970 ; void __cdecl object_compute_node_matrices_non_recursive(int32)
//.text:00B2C240 ; void __cdecl object_compute_origin_matrix(int32, const real_point3d*, const real_vector3d*, const real_vector3d*, real32, bool, const real_matrix4x3*, bool, real_matrix4x3*)
//.text:00B2C3D0 ; const real_matrix4x3* __cdecl object_compute_render_time_node_matrices(int32, int32, const uns8*, const real_matrix4x3*)

void __cdecl object_connect_lights(int32 object_index, bool disconnect_this_object, bool reconnect_this_object)
{
	INVOKE(0x00B2C790, object_connect_lights, object_index, disconnect_this_object, reconnect_this_object);

	//object_connect_lights_recursive(object_index, disconnect_this_object, reconnect_this_object, false, false);
}

void __cdecl object_connect_lights_recursive(int32 object_index, bool disconnect_this_object, bool reconnect_this_object, bool a3, bool a4)
{
	INVOKE(0x00B2C7B0, object_connect_lights_recursive, object_index, disconnect_this_object, reconnect_this_object, a3, a4);
}

int32 __cdecl object_count(int32 type_flags, uns8 header_mask)
{
	return INVOKE(0x00B2C8E0, object_count, type_flags, header_mask);
}

void __cdecl object_create_attachments(int32 object_index)
{
	INVOKE(0x00B2C940, object_create_attachments, object_index);
}

void __cdecl object_create_children(int32 object_index)
{
	INVOKE(0x00B2C980, object_create_children, object_index);
}

bool __cdecl object_custom_animations_hold_on_last_frame()
{
	return INVOKE(0x00B2CBA0, object_custom_animations_hold_on_last_frame);
}

//.text:00B2CBC0 ; bool __cdecl object_custom_animations_prevent_lipsync_head_movement()

//void __cdecl object_deactivate(int32 object_index)
//{
//	INVOKE(0x00B2CBE0, object_deactivate, object_index);
//
//	//object_deactivate(object_index, false);
//}
//
//void __cdecl object_deactivate(int32 object_index, bool a2)
//{
//	INVOKE(0x00B2CC00, object_deactivate, object_index, a2);
//}

//.text:00B2CCE0 ; bool __cdecl object_definition_predict(int32, bool)
//.text:00B2CCF0 ; bool __cdecl object_definition_predict_all(int32)

bool __cdecl object_definition_predict_low(int32 object_definition_index)
{
	return INVOKE(0x00B2CD00, object_definition_predict_low, object_definition_index);
}

void __cdecl object_delete(int32 object_index)
{
	INVOKE(0x00B2CD10, object_delete, object_index);
}

void __cdecl object_delete_all_multiplayer_cinematic_objects()
{
	INVOKE(0x00B2CE90, object_delete_all_multiplayer_cinematic_objects);
}

void __cdecl object_delete_attachments(int32 object_index)
{
	INVOKE(0x00B2CF20, object_delete_attachments, object_index);
}

void __cdecl object_delete_immediately(int32 object_index)
{
	INVOKE(0x00B2CF40, object_delete_immediately, object_index);
}

void __cdecl object_delete_recursive(int32 object_index, bool deactivate)
{
	INVOKE(0x00B2CF70, object_delete_recursive, object_index, deactivate);
}

void __cdecl object_destroy_instance_group(int32 object_index, int32 instance_group_index)
{
	INVOKE(0x00B2D0B0, object_destroy_instance_group, object_index, instance_group_index);
}

void __cdecl object_detach(int32 object_index)
{
	INVOKE(0x00B2D180, object_detach, object_index);
}

void __cdecl object_detach_from_node(int32 object_index, const real_matrix4x3* node)
{
	INVOKE(0x00B2D1D0, object_detach_from_node, object_index, node);
}

void __cdecl object_detach_gamestate_entity(int32 object_index, int32 gamestate_index)
{
	INVOKE(0x00B2D270, object_detach_gamestate_entity, object_index, gamestate_index);
}

void __cdecl object_detach_immediate(int32 object_index)
{
	INVOKE(0x00B2D2C0, object_detach_immediate, object_index);
}

void __cdecl object_detach_internal(int32 object_index, bool compute_bounding_sphere)
{
	INVOKE(0x00B2D460, object_detach_internal, object_index, compute_bounding_sphere);
}

void __cdecl object_detach_predicted_objects(int32 object_index)
{
	INVOKE(0x00B2D4E0, object_detach_predicted_objects, object_index);
}

void __cdecl object_disconnect_from_map(int32 object_index, bool broadphase_remove_object)
{
	INVOKE(0x00B2D560, object_disconnect_from_map, object_index, broadphase_remove_object);
}

void __cdecl object_disconnect_from_physics(int32 object_index)
{
	INVOKE(0x00B2D600, object_disconnect_from_physics, object_index);
}

bool __cdecl object_find_initial_location(int32 object_index, s_location* location)
{
	return INVOKE(0x00B2D660, object_find_initial_location, object_index, location);
}

void __cdecl object_find_structure_bsp_fake_lightprobe_index(int32 structure_bsp_index, int32 object_index)
{
	INVOKE(0x00B2D6E0, object_find_structure_bsp_fake_lightprobe_index, structure_bsp_index, object_index);
}

bool __cdecl object_force_inside_bsp(int32 object_index, const real_point3d* position, int32 ignore_object_index)
{
	return INVOKE(0x00B2D7D0, object_force_inside_bsp, object_index, position, ignore_object_index);
}

void __cdecl object_freeze_node_orientations(int32 object_index, bool a2)
{
	INVOKE(0x00B2D9B0, object_freeze_node_orientations, object_index, a2);
}

bool __cdecl object_function_get_function_value(int32 object_index, const s_object_function_definition* function, int32 object_definition_index, real32* out_function_magnitude, bool* deterministic)
{
	return INVOKE(0x00B2DA20, object_function_get_function_value, object_index, function, object_definition_index, out_function_magnitude, deterministic);
}

int32 __cdecl object_get_attachment_marker_name(int32 object_index, int16 attachment_index)
{
	return INVOKE(0x00B2DCB0, object_get_attachment_marker_name, object_index, attachment_index);
}

bool __cdecl object_get_base_change_color_by_index(int32 object_index, int32 change_color_index, real_rgb_color* color)
{
	return INVOKE(0x00B2DD10, object_get_base_change_color_by_index, object_index, change_color_index, color);
}

real_point3d* __cdecl object_get_center_of_mass(int32 object_index, real_point3d* center)
{
	return INVOKE(0x00B2DD90, object_get_center_of_mass, object_index, center);
}

bool __cdecl object_get_change_color(int32 object_index, int32 change_color_index, real_rgb_color* change_color)
{
	return INVOKE(0x00B2DE50, object_get_change_color, object_index, change_color_index, change_color);
}

void __cdecl object_get_closest_point_and_normal(int32 object_index, const real_point3d* origin, real_point3d* closest_point, real_vector3d* normal)
{
	INVOKE(0x00B2DEF0, object_get_closest_point_and_normal, object_index, origin, closest_point, normal);
}

void __cdecl object_get_damage_owner(int32 object_index, s_damage_owner* owner)
{
	INVOKE(0x00B2DF80, object_get_damage_owner, object_index, owner);
}

bool __cdecl object_get_function_value(int32 object_index, int32 function_name, int32 object_definition_index, real32* out_function_magnitude)
{
	return INVOKE(0x00B2E030, object_get_function_value, object_index, function_name, object_definition_index, out_function_magnitude);
}

//.text:00B2E1A0 ; real32 __cdecl object_get_function_value_simple(int32, int32, int32)
//.text:00B2E1E0 ; int16 __cdecl object_get_local_markers_by_string_id(int32, int32, object_marker*, int16)

bool __cdecl object_get_localized_velocities(int32 object_index, real_vector3d* translational_velocity, real_vector3d* angular_velocity, real_vector3d* local_translational_velocity, real_vector3d* local_angular_velocity)
{
	return INVOKE(0x00B2E200, object_get_localized_velocities, object_index, translational_velocity, angular_velocity, local_translational_velocity, local_angular_velocity);
}

s_location* __cdecl object_get_location(int32 object_index, s_location* location)
{
	return INVOKE(0x00B2E2F0, object_get_location, object_index, location);
}

//.text:00B2E330 ; int32 __cdecl object_get_marker_object(int32)

int16 __cdecl object_get_markers_by_string_id(int32 object_index, string_id marker_name, object_marker* markers, int16 maximum_marker_count)
{
	return INVOKE(0x00B2E3C0, object_get_markers_by_string_id, object_index, marker_name, markers, maximum_marker_count);
}

real_matrix4x3* __cdecl object_get_node_matrix(int32 object_index, int16 node_index)
{
	return INVOKE(0x00B2E450, object_get_node_matrix, object_index, node_index);
}

void __cdecl object_get_orientation(int32 object_index, real_vector3d* forward, real_vector3d* up)
{
	INVOKE(0x00B2E490, object_get_orientation, object_index, forward, up);
}

real_point3d* __cdecl object_get_origin(int32 object_index, real_point3d* origin)
{
	if (!object_get(object_index))
		return origin;

	return INVOKE(0x00B2E5A0, object_get_origin, object_index, origin);
}

int32 __cdecl object_get_root_object(int32 object_index)
{
	return INVOKE(0x00B2E8F0, object_get_root_object, object_index);
}

int32 __cdecl object_get_ultimate_parent(int32 object_index)
{
	return INVOKE(0x00B2EAB0, object_get_ultimate_parent, object_index);
}

int32 __cdecl object_get_variant_index(int32 object_index)
{
	return INVOKE(0x00B2EB00, object_get_variant_index, object_index);
}

void __cdecl object_get_velocities(int32 object_index, real_vector3d* linear_velocity, real_vector3d* angular_velocity)
{
	INVOKE(0x00B2EB30, object_get_velocities, object_index, linear_velocity, angular_velocity);
}

real_matrix4x3* __cdecl object_get_world_matrix(int32 object_index, real_matrix4x3* matrix)
{
	return INVOKE(0x00B2EC60, object_get_world_matrix, object_index, matrix);
}

real_matrix4x3* __cdecl object_get_world_matrix_interpolated(int32 object_index, real_matrix4x3* out_matrix)
{
	return INVOKE(0x00B2ECE0, object_get_world_matrix_interpolated, object_index, out_matrix);
}

bool __cdecl object_has_animation_manager(int32 object_index)
{
	return INVOKE(0x00B2EDF0, object_has_animation_manager, object_index);
}

bool __cdecl object_has_multiplayer_properties(int32 object_index)
{
	return INVOKE(0x00B2EE30, object_has_multiplayer_properties, object_index);
}

bool __cdecl object_header_block_allocate(int32 object_index, int16 block_reference_offset, int16 size, int16 alignment_bits)
{
	return INVOKE(0x00B2EE70, object_header_block_allocate, object_index, block_reference_offset, size, alignment_bits);
}

void __cdecl object_header_delete(int32 object_index)
{
	return INVOKE(0x00B2EF90, object_header_delete, object_index);
}

int32 __cdecl object_header_new(int16 size)
{
	return INVOKE(0x00B2EFF0, object_header_new, size);
}

int32 __cdecl object_index_from_name_index(int16 name_index)
{
	return INVOKE(0x00B2F0A0, object_index_from_name_index, name_index);
}

int32 __cdecl object_index_from_scenario_object_index(e_object_type object_type, int32 object_index)
{
	return INVOKE(0x00B2F0E0, object_index_from_scenario_object_index, object_type, object_index);
}

bool __cdecl object_is_at_rest(int32 object_index)
{
	return INVOKE(0x00B2F120, object_is_at_rest, object_index);
}

bool __cdecl object_is_being_deleted(int32 object_index)
{
	return INVOKE(0x00B2F160, object_is_being_deleted, object_index);
}

bool __cdecl object_is_connected_to_map(int32 object_index)
{
	return INVOKE(0x00B2F190, object_is_connected_to_map, object_index);
}

bool __cdecl object_is_hidden(int32 object_index)
{
	return INVOKE(0x00B2F1D0, object_is_hidden, object_index);
}

bool __cdecl object_is_hidden_internal(const object_header_datum* object_header, const object_datum* object)
{
	return INVOKE(0x00B2F220, object_is_hidden_internal, object_header, object);
}

bool __cdecl object_is_hidden_non_recursive(int32 object_index)
{
	return INVOKE(0x00B2F240, object_is_hidden_non_recursive, object_index);
}

//.text:00B2F280 ; bool __cdecl sub_B2F280(int32 object_index, int32 render_model_index);

bool __cdecl object_is_multiplayer_cinematic_object(int32 object_index)
{
	return INVOKE(0x00B2F300, object_is_multiplayer_cinematic_object, object_index);
}

bool __cdecl object_is_or_contains_player(int32 object_index)
{
	return INVOKE(0x00B2F340, object_is_or_contains_player, object_index);
}

bool __cdecl object_is_running_sync_action(int32 object_index)
{
	return INVOKE(0x00B2F430, object_is_running_sync_action, object_index);
}

bool __cdecl object_is_vehicle(int32 object_index)
{
	return INVOKE(0x00B2F470, object_is_vehicle, object_index);
}

//.text:00B2F4B0 ; protected: void __cdecl c_object_iterator_base::object_iterator_begin_internal(uns32, uns32, uns32, int32)

e_object_type c_object_identifier::get_type() const
{
	return m_type;
}

int32 c_object_iterator_base::get_index()
{
	return m_iterator.index;
}

void c_object_iterator_base::object_iterator_begin_internal(uns32 type_flags, uns32 header_flags, uns32 iteration_match_flags, int32 next_absolute_index)
{
	return INVOKE_CLASS_MEMBER(0x00B2F4B0, c_object_iterator_base, object_iterator_begin_internal, type_flags, header_flags, iteration_match_flags, next_absolute_index);
}

bool c_object_iterator_base::object_iterator_next_internal()
{
	return INVOKE_CLASS_MEMBER(0x00B2F500, c_object_iterator_base, object_iterator_next_internal);
}

bool c_object_iterator_base::object_iterator_next_with_match_flags_internal()
{
	return INVOKE_CLASS_MEMBER(0x00B2F5A0, c_object_iterator_base, object_iterator_next_with_match_flags_internal);
}

object_datum* c_object_iterator_base::get_datum_internal()
{
	return m_object;
}

int32 __cdecl object_list_children_by_definition(int32 object_index, int32 definition_index)
{
	return INVOKE(0x00B2F830, object_list_children_by_definition, object_index, definition_index);
}

bool __cdecl object_load_scenario_placement_matrices(int32 object_index)
{
	return INVOKE(0x00B2F890, object_load_scenario_placement_matrices, object_index);
}

int32 __cdecl object_local_physics_object_get(int32 object_index)
{
	return INVOKE(0x00B2FBD0, object_local_physics_object_get, object_index);
}

int32 __cdecl object_lookup_variant_index_from_name(int32 object_index, int32 name)
{
	return INVOKE(0x00B2FC20, object_lookup_variant_index_from_name, object_index, name);
}

//void __cdecl object_make_noise(int32 object_index, e_sound_type sound_type, e_ai_sound_volume ai_sound_volume)
void __cdecl object_make_noise(int32 object_index, int32 sound_type, int32 ai_sound_volume)
{
	INVOKE(0x00B2FC70, object_make_noise, object_index, sound_type, ai_sound_volume);
}

//.text:00B2FCE0 ; bool __cdecl object_mark(int32)
//.text:00B2FD20 ; bool __cdecl object_mark_get_user_data(int32, int32*)
//.text:00B2FD60 ; bool __cdecl object_mark_with_user_data(int32, int32)

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

//.text:00B2FE20 ; void __cdecl object_model_state_changed(int32, int32, const char*)

void __cdecl object_move(int32 object_index)
{
	INVOKE(0x00B2FE50, object_move, object_index);
}

void __cdecl object_move_position(int32 object_index, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up, const s_location* location)
{
	INVOKE(0x00B30050, object_move_position, object_index, position, forward, up, location);

	//ASSERT(object_get(object_index)->object.parent_object_index == NONE);
	//object_set_position_internal(object_index, position, forward, up, location, false, true, false, false);
	//object_set_requires_motion(object_index);
}

void __cdecl object_name_list_allocate()
{
	INVOKE(0x00B301F0, object_name_list_allocate);
}

void __cdecl object_name_list_clear()
{
	INVOKE(0x00B30250, object_name_list_clear);
}

void __cdecl object_name_list_delete(int32 object_index)
{
	INVOKE(0x00B30280, object_name_list_delete, object_index);
}

int32 __cdecl object_name_list_lookup(int16 name_index)
{
	return INVOKE(0x00B30310, object_name_list_lookup, name_index);
}

void __cdecl object_name_list_new(int32 name, int16 name_index)
{
	INVOKE(0x00B30350, object_name_list_new, name, name_index);
}

bool __cdecl object_needs_rigid_body_update(int32 object_index)
{
	return INVOKE(0x00B303A0, object_needs_rigid_body_update, object_index);
}

int32 __cdecl object_new(object_placement_data* data)
{
	return INVOKE(0x00B30440, object_new, data);

	//if (!TEST_BIT(data->flags, 4) && data->definition_index != NONE)
	//	object_type_adjust_placement(data);
	//
	//if ((data->definition_index != NONE && object_definition_can_be_placed(data->definition_index, data->model_variant_index)) || data->definition_index == NONE)
	//	return NONE;
	//
	//struct object_definition* object_definition = TAG_GET(OBJECT_TAG, struct object_definition, data->definition_index);
	//object_type_definition* type_definition = object_type_definition_get(object_definition->object.type);
	//
	//s_model_definition* model_definition = NULL;
	//if (object_definition->object.model.index != NONE)
	//	model_definition = TAG_GET(MODEL_TAG, s_model_definition, object_definition->object.model.index);
	//
	//if (object_should_have_havok_component(NONE, data->definition_index))
	//	havok_memory_garbage_collect();
	//
	//int32 object_index = object_header_new(type_definition->datum_size);
	//if (object_index == NONE)
	//	return object_index;
	//
	//bool v53 = false;
	//bool v60 = false;
	//bool v63 = object_definition->object.multiplayer_object.count > 0;
	//bool v64 = false;
	//
	//object_header_datum* object_header = object_header_get_mutable(object_index);
	//object_datum* object = object_header->datum;
	//
	//object->object.flags.set(_object_being_created_bit, true);
	//object->definition_index = data->definition_index;
	//object->object.map_variant_index = NONE;
	//object->object.next_recycling_object_index = NONE;
	//object->object.recycling_time = NONE;
	//object->object.parent_recycling_group = NONE;
	//object->object.next_recycling_group_member = NONE;
	//object->object.scenery_air_probe_index = NONE;
	//object->object.air_probe_index = NONE;
	//
	//object->object.object_identifier = data->object_identifier;
	//int32 scenario_datum_index = data->scenario_datum_index;
	//if (data->object_identifier.m_source == _object_source_none)
	//{
	//	object->object.object_identifier.create_dynamic(object_definition->object.type);
	//	scenario_datum_index = NONE;
	//}
	//
	//object->object.scenario_datum_index = scenario_datum_index;
	//object->object.position = data->position;
	//object->object.forward = data->forward;
	//object->object.up = data->up;
	//object->object.transitional_velocity = data->linear_velocity;
	//object->object.angular_velocity = data->translational_velocity;
	//object->object.scale = data->scale;
	//object->object.flags.set(_object_mirrored_bit, TEST_BIT(data->flags, 0));
	//object->object.flags.set(_object_uses_collidable_list_bit, model_definition && model_definition->collision_model.index != NONE);
	//object->object.flags.set(_object_is_prt_and_lightmapped_bit, false);
	//object->object.flags.set(_object_created_with_parent_bit, object->object.object_identifier.m_source == _object_source_parent);
	//
	//if (object_definition->object.model.index != NONE)
	//	object->object.flags.set(_object_render_model_has_instances_bit, render_model_has_instances(model_definition->render_model.index));
	//
	//object_header->cluster_index = NONE;
	//object->object.location = { .cluster_reference = { .bsp_index = NONE, .cluster_index = NONE } };
	//object->object.first_cluster_reference_index = NONE;
	//object->object.clusters_touched_on_connection = 0;
	//object->object.bsp_placement_policy = data->bsp_placement_policy;
	//object->object.parent_object_index = NONE;
	//object->object.next_object_index = NONE;
	//object->object.first_child_object_index = NONE;
	//object->object.first_widget_index = NONE;
	//object->object.name_index = NONE;
	//object->object.damaged_explosion_timer = NONE;
	//object->object.body_damage_delay_ticks = NONE;
	//object->object.shield_impact_decay_timer = NONE;
	//
	//if (TEST_FLAG(object->object.flags, _object_hidden_bit))
	//{
	//	object->object.flags.set(_object_hidden_bit, false);
	//	const object_header_datum* ultimate_parent_object = object_header_get(object_get_ultimate_parent(object_index));
	//	if (TEST_FLAG(ultimate_parent_object->flags, _object_header_connected_to_map_bit))
	//		object_connect_lights_recursive(object_index, false, true, false, false);
	//	object_update_collision_culling(object_index);
	//}
	//
	//object->object.damage_owner = data->damage_owner;
	//object->object.structure_bsp_fake_lightprobe_index = NONE;
	//object->object.havok_component_index = NONE;
	//object->object.physics_flags = 0;
	//object->object.variant_index = NONE;
	//
	//object->object.physics_flags = TEST_FLAG(object_definition->object.flags, _object_does_not_collide_with_camera_bit) ? FLAG(21) : 0;
	//SET_BIT(object->object.physics_flags, 22, TEST_FLAG(object_definition->object.secondary_flags, _object_does_not_affect_projectile_aiming_bit));
	//SET_BIT(object->object.physics_flags, 9, TEST_BIT(data->flags, 3));
	//
	//object->object.in_water_ticks = 32768;
	//object->object.created_at_rest = TEST_BIT(data->flags, 8);// BYTE1(data->flags) & 1;
	//object->object.simulation_object_glue_index = NONE;
	//object->object.owner_team_index = NONE;
	//object->object.simulation_flags = 0;
	//object->object.child_variant_index = NONE;
	//object->object.destroyed_constraints = data->destroyed_constraints;
	//object->object.loosened_constraints = data->loosened_constraints;
	//
	//object_clear_sync_action(object_index);
	//
	//SET_BIT(object->object.simulation_flags, 1, data->multiplayer_cinematic_object);
	//
	//// $IMPLEMENT
	//
	//return object_index;
}

int32 __cdecl object_new_by_name(int16 name_index, bool displace_previous_object, bool multiplayer_cinematic_object)
{
	return INVOKE(0x00B30E60, object_new_by_name, name_index, displace_previous_object, multiplayer_cinematic_object);
}

//.text:00B30FD0 ; int32 __cdecl object_new_from_scenario(e_object_type, int32, s_scenario_object*, s_tag_block*, bool)
//.text:00B31000 ; int32 __cdecl object_new_from_scenario_bypass_simulation_object_placement_test(e_object_type, int32, s_scenario_object*, s_tag_block*, int32, bool)
//.text:00B31030 ; 
//.text:00B310C0 ; int32 __cdecl object_new_from_scenario_internal(e_object_type, int16, const s_scenario_object*, s_tag_block*, int32, bool, bool, int32)

bool __cdecl object_node_orientations_frozen(int32 object_index)
{
	return INVOKE(0x00B31240, object_node_orientations_frozen, object_index);
}

void __cdecl object_notify_in_local_physics_object(int32 object_index, int32 local_physics_object_index)
{
	INVOKE(0x00B31270, object_notify_in_local_physics_object, object_index, local_physics_object_index);
}

//.text:00B312F0 ; void __cdecl object_offset_interpolation(int32, const real_vector3d*)
//.text:00B31380 ; bool __cdecl object_owns_object(int32, int32)

int32 __cdecl object_override_create(int32 object_index)
{
	return NONE;

	object_datum* object = object_get(object_index);
	int32 override_index = object_override_find(object_index);
	if (override_index == NONE)
	{
		ASSERT(!object->object.flags.test(_object_has_override_bit));

		for (int32 object_override_index = 0; object_override_index < k_maximum_object_override_count; object_override_index++)
		{
			s_object_override* override = object_override_get(object_override_index);
			if (!override->valid)
			{
				override_index = object_override_index;
				override->valid = true;
				override->object_index = object_index;
				override->shader_index = NONE;
				break;
			}
		}

		if (override_index == NONE)
		{
			s_object_override* override = object_override_get(0);
			ASSERT(override->valid);
			ASSERT(override->object_index != NONE);

			if (object_datum* override_object = object_get(override->object_index))
			{
				override_object->object.flags.set(_object_has_override_bit, false);
			}

			memmove_guarded(
				object_override_globals.overrides,
				object_override_globals.overrides + 1,
				sizeof(s_object_override) * k_maximum_object_override_count - 1,
				object_override_globals.overrides,
				sizeof(object_override_globals));

			override = object_override_get(k_maximum_object_override_count - 1);
			override->valid = true;
			override->object_index = object_index;
			override->shader_index = NONE;
			override_index = k_maximum_object_override_count - 1;
		}
		object->object.flags.set(_object_has_override_bit, true);
	}

	ASSERT(object->object.flags.test(_object_has_override_bit) == (override_index != NONE));
	return override_index;
}

int32 __cdecl object_override_find(int32 object_index)
{
	return NONE;

	object_datum* object = object_get(object_index);
	int32 override_index = NONE;

	if (object->object.flags.test(_object_has_override_bit))
	{
		for (int32 i = 0; i < k_maximum_object_override_count; i++)
		{
			const s_object_override* override = object_override_get(i);
			ASSERT(override_index >= 0 && override_index < k_maximum_object_override_count);
			if (override->valid && override->object_index == object_index)
			{
				override_index = i;
				break;
			}
		}
	}

	ASSERT(object->object.flags.test(_object_has_override_bit) == (override_index != NONE));
	return override_index;
}

s_object_override* __cdecl object_override_get(int32 override_index)
{
	ASSERT(override_index >= 0 && override_index < k_maximum_object_override_count);
	s_object_override* result = &object_override_globals.overrides[override_index];
	return result;
}

int32 __cdecl object_override_get_shader(int32 object_index)
{
	int32 override_index = object_override_find(object_index);
	if (override_index != NONE)
	{
		int32 shader_index = object_override_get(override_index)->shader_index;
		return shader_index;
	}
	return NONE;
}

void __cdecl object_override_set_shader(int32 object_index, int32 shader_index)
{
	int32 override_index = object_override_create(object_index);
	if (override_index != NONE)
	{
		object_override_get(override_index)->shader_index = shader_index;
	}
}

//.text:00B313E0 ; void __cdecl object_physics_dynamic_force(int32, bool)

void __cdecl object_place(int32 object_index, const s_scenario_object* scenario_object)
{
	INVOKE(0x00B31470, object_place, object_index, scenario_object);

	//object_datum* object = object_get(object_index);
	//struct object_definition* object_definition = TAG_GET(OBJECT_TAG, struct object_definition, object->definition_index);
	//if (object_definition->object.ai_properties.count > 0)
	//{
	//	bool non_flight_blocking = object_definition->object.ai_properties[0].ai_flags.test(_ai_properties_non_flight_blocking_bit);
	//	SET_BIT(object->object.object_ai_flags, 2, non_flight_blocking);
	//}
}

void __cdecl object_placement_data_copy_change_colors(object_placement_data* data, int32 object_index)
{
	INVOKE(0x00B314E0, object_placement_data_copy_change_colors, data, object_index);

	//object_datum* object = object_get(object_index);
	//int32 change_color_count = 0;
	//if (real_rgb_color* change_colors = OBJECT_HEADER_BLOCK_GET_WITH_COUNT(object_index, real_rgb_color, &object->object.change_colors, &change_color_count))
	//{
	//	ASSERT(change_color_count % 2 == 0);
	//	for (int32 change_color_index = 0; change_color_index < change_color_count / 2; change_color_index++)
	//	{
	//		data->change_color_overrides[change_color_index] = change_colors[change_color_index];
	//		data->change_color_override_mask.set(change_color_index, true);
	//	}
	//}
}

void __cdecl object_placement_data_new(object_placement_data* data, int32 definition_index, int32 creator_object_index, const s_damage_owner* damage_owner)
{
	//INVOKE(0x00B31590, object_placement_data_new, data, definition_index, creator_object_index, damage_owner);

	csmemset(data, 0, sizeof(object_placement_data));
	
	data->definition_index = definition_index;
	data->model_variant_index = 0;
	data->flags = 0;
	data->forward = *global_forward3d;
	data->up = *global_up3d;
	data->ai_state_type = NONE;
	data->scale = 1.0f;
	data->change_color_override_mask.clear();
	data->bsp_placement_policy = 0;
	
	if (object_datum* owner_object = object_get(creator_object_index))
	{
		data->owner_object_index = creator_object_index;
		data->owner_player_index = NONE;
		data->owner_team_index = NONE;
	
		SET_BIT(data->flags, 5, owner_object->object.physics_flags.test(_object_in_local_physics_bit));
	
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

	if (creator_object_index != NONE)
		data->multiplayer_cinematic_object = object_is_multiplayer_cinematic_object(creator_object_index);

	data->parent_object_index = NONE;
	data->parent_marker = _string_id_invalid;
	data->child_marker = _string_id_invalid;
	
	for (s_model_customization_region_permutation& permutation : data->model_customization_overrides)
	{
		permutation.region_name = NONE;
		permutation.permutation_name = NONE;
	}
	data->model_customization_override_count = 0;
}

void __cdecl object_placement_data_set_location(object_placement_data* data, const struct s_location* location)
{
	INVOKE(0x00B31750, object_placement_data_set_location, data, location);

	//ASSERT(data != NULL);
	//ASSERT(location != NULL);
	//
	//data->location = *location;
	//data->location_set = true;
}

void __cdecl object_postprocess_node_matrices(int32 object_index)
{
	INVOKE(0x00B31770, object_postprocess_node_matrices, object_index);
}

void __cdecl object_pre_delete_recursive(int32 object_index, bool a2)
{
	INVOKE(0x00B31810, object_pre_delete_recursive, object_index, a2);
}

bool __cdecl object_predict(int32 object_index, bool low)
{
	return INVOKE(0x00B31890, object_predict, object_index, low);
}

bool __cdecl object_predict_all(int32 object_index)
{
	//return INVOKE(0x00B31910, object_predict_all, object_index);

	return object_predict(object_index, false);
}

bool __cdecl object_predict_low(int32 object_index)
{
	//return INVOKE(0x00B31930, object_predict_low, object_index);

	return object_predict(object_index, true);
}

void object_prepare_axis_vectors(int32 object_index, real_vector3d* forward, real_vector3d* up)
{
	INVOKE(0x00B31950, object_prepare_axis_vectors, object_index, forward, up);
}

//.text:00B31B80 ; void __cdecl object_queue_render_thread_message(int32, e_object_render_thread_message_type, int16)

void __cdecl object_reconnect_to_map(int32 object_index, bool a2, const s_location* location)
{
	INVOKE(0x00B31BD0, object_reconnect_to_map, object_index, a2, location);
}

void __cdecl object_reconnect_to_physics(int32 object_index)
{
	INVOKE(0x00B31E20, object_reconnect_to_physics, object_index);
}

void __cdecl object_register_scenario_object(int32 object_index)
{
	INVOKE(0x00B31E80, object_register_scenario_object, object_index);
}

void __cdecl object_reinitialize_from_placement(int32 object_index, object_placement_data* data, bool at_rest)
{
	INVOKE(0x00B31EF0, object_reinitialize_from_placement, object_index, data, at_rest);
}

void __cdecl object_remove_from_list(int32* object_list, int32 object_index)
{
	INVOKE(0x00B320A0, object_remove_from_list, object_list, object_index);
}

void __cdecl object_remove_pending_messages(int32 object_index)
{
	INVOKE(0x00B32100, object_remove_pending_messages, object_index);
}

void __cdecl object_render_debug(int32 object_index)
{
	//INVOKE(0x00B32130, object_render_debug, object_index);

	if (TEST_BIT(_object_mask_sound_scenery, object_get_type(object_index)) && !debug_objects_sound_spheres)
	{
		return;
	}

	object_render_debug_internal(object_index);
}

//.text:00B32140 ; bool __cdecl object_report_creation_failure(int32, const char*)

void __cdecl object_reset(int32 object_index)
{
	INVOKE(0x00B32160, object_reset, object_index);
}

void __cdecl object_reset_interpolation(int32 object_index)
{
	INVOKE(0x00B321A0, object_reset_interpolation, object_index);
}

void __cdecl object_respond_to_physics(int32 object_index)
{
	INVOKE(0x00B321F0, object_respond_to_physics, object_index);
}

void __cdecl object_resurrect(int32 object_index)
{
	INVOKE(0x00B32400, object_resurrect, object_index);
}

//.text:00B324A0 ; void __cdecl object_reverse_compute_node_orientations_from_matrices(int32, const c_static_flags<256>*)
//.text:00B32540 ; void __cdecl object_reverse_compute_node_orientations_from_matrices(int32, const c_static_flags<256>*, const render_model_definition*, int32, real_orientation*)

void __cdecl object_set_always_active(int32 object_index, bool always_active)
{
	INVOKE(0x00B32730, object_set_always_active, object_index, always_active);
}

void __cdecl object_set_at_rest(int32 object_index, bool at_rest)
{
	INVOKE(0x00B327F0, object_set_at_rest, object_index, at_rest);
}

bool __cdecl object_set_base_change_color_by_index(int32 object_index, int32 color_index, const real_rgb_color* color)
{
	return INVOKE(0x00B328F0, object_set_base_change_color_by_index, object_index, color_index, color);
}

void __cdecl object_set_body_vitality(int32 object_index, real32 body_vitality, int16 body_stun_ticks)
{
	INVOKE(0x00B329A0, object_set_body_vitality, object_index, body_vitality, body_stun_ticks);
}

void __cdecl object_set_custom_animation_speed(int32 object_index, real32 custom_animation_speed)
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

void __cdecl object_set_damage_owner(int32 object_index, const s_damage_owner* damage_owner, bool a3)
{
	INVOKE(0x00B32AD0, object_set_damage_owner, object_index, damage_owner, a3);
}

void __cdecl object_set_desired_velocities(int32 object_index, const real_vector3d* transitional_velocity, const real_vector3d* angular_velocity, bool a4)
{
	INVOKE(0x00B32B80, object_set_desired_velocities, object_index, transitional_velocity, angular_velocity, a4);
}

void __cdecl object_set_garbage(int32 object_index, bool a2, int32 collection_ticks)
{
	INVOKE(0x00B32C20, object_set_garbage, object_index, a2, collection_ticks);
}

void __cdecl object_set_hidden(int32 object_index, bool hidden)
{
	INVOKE(0x00B32D50, object_set_hidden, object_index, hidden);
}

void __cdecl object_set_in_limbo(int32 object_index, bool deactivate)
{
	INVOKE(0x00B32E20, object_set_in_limbo, object_index, deactivate);
}

void __cdecl object_set_infinite_shield_stun(int32 object_index)
{
	INVOKE(0x00B32EE0, object_set_infinite_shield_stun, object_index);
}

void __cdecl object_set_initial_change_colors(int32 object_index, c_flags<int8, uns8, 5> active_change_colors, const real_rgb_color* change_colors)
{
	INVOKE(0x00B32F20, object_set_initial_change_colors, object_index, active_change_colors, change_colors);
}

void __cdecl object_set_invalid_for_recycling(int32 object_index, bool invalid)
{
	INVOKE(0x00B33290, object_set_invalid_for_recycling, object_index, invalid);
}

bool __cdecl object_set_model_state(int32 object_index, int32 a2, int32 a3, bool a4)
{
	return INVOKE(0x00B332F0, object_set_model_state, object_index, a3, a3, a4);
}

void __cdecl object_set_model_state_property(int32 object_index, int32 a2, int32 a3, bool a4, bool a5)
{
	INVOKE(0x00B333F0, object_set_model_state_property, object_index, a3, a3, a4, a5);
}

void __cdecl object_set_model_state_property_per_region(int32 object_index, int32 a2, int32 a3, int16 a4, bool a5)
{
	INVOKE(0x00B33410, object_set_model_state_property_per_region, object_index, a3, a3, a4, a5);
}

void __cdecl object_set_object_index_for_name_index(int16 name_index, int32 object_index)
{
	INVOKE(0x00B334F0, object_set_object_index_for_name_index, name_index, object_index);
}

void __cdecl object_set_position(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location)
{
	//INVOKE(0x00B33530, object_set_position, object_index, desired_position, desired_forward, desired_up, location);

	object_set_position_direct(object_index, desired_position, desired_forward, desired_up, location, false);
}

void __cdecl object_set_position_direct(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location, bool in_editor)
{
	INVOKE(0x00B33550, object_set_position_direct, object_index, desired_position, desired_forward, desired_up, location, in_editor);
}

void __cdecl object_set_position_in_editor(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location, bool at_rest)
{
	//INVOKE(0x00B33600, object_set_position_in_editor, object_index, desired_position, desired_forward, desired_up, location, at_rest);
	
	object_set_position_direct(object_index, desired_position, desired_forward, desired_up, location, true);
	object_load_scenario_placement_matrices(object_index);
	if (at_rest && TEST_BIT(_object_mask_editor_placeable_objects, object_get_type(object_index)))
		object_set_at_rest(object_index, false);
}

void __cdecl object_set_position_in_sandbox_editor(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location)
{
	//INVOKE(0x00B33670, object_set_position_in_sandbox_editor, object_index, desired_position, desired_forward, desired_up, location);

	object_set_position_direct(object_index, desired_position, desired_forward, desired_up, location, true);
}

bool __cdecl object_set_position_internal(int32 object_index, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up, const s_location* location, bool compute_node_matrices, bool set_havok_object_position, bool in_editor, bool disconnected)
{
	return INVOKE(0x00B33690, object_set_position_internal, object_index, position, forward, up, location, compute_node_matrices, set_havok_object_position, in_editor, disconnected);

	//bool result = true;
	//
	//object_datum* object = object_get(object_index);
	//
	//bool connected_to_map = object->object.flags.test(_object_connected_to_map_bit);
	//if (connected_to_map)
	//	object_disconnect_from_map(object_index, false);
	//
	//if (position)
	//{
	//	if (object->object.parent_object_index == NONE && !point_in_bounds(position, 32768.0f))
	//		result = false;
	//
	//	if (result)
	//	{
	//		object->object.position = *position;
	//		simulation_action_object_update(object_index, _simulation_object_update_position);
	//	}
	//}
	//
	//if (forward)
	//{
	//	object->object.forward = *forward;
	//	object->object.up = *up;
	//	simulation_action_object_update(object_index, _simulation_object_update_forward_and_up);
	//}
	//
	//havok_object_set_position(object_index, position == NULL, false, false);
	//
	//if (connected_to_map)
	//{
	//	object_reconnect_to_map(object_index, false, location);
	//
	//	if (object->object.flags.test(_object_uses_collidable_list_bit))
	//		object_broadphase_update_object(object_index);
	//}
	//
	//object_header_datum* object_header = object_header_get_mutable(object_index);
	//if (object_header->flags.test(_object_header_child_bit))
	//{
	//	int32 ultimate_parent_index = object_get_ultimate_parent(object_index);
	//	object_header = object_header_get_mutable(ultimate_parent_index);
	//}
	//
	//if (object_header->flags.test(_object_header_active_bit))
	//{
	//	object_wake(object_index);
	//	object_header->flags.set(_object_header_awake_bit, true);
	//}
	//
	//recursive_wake_children_awoken_by_movement(object_index);
	//
	//return result;
}

void __cdecl object_set_region_permutation_direct(int32 object_index, int32 region_name, int32 permutation_name)
{
	INVOKE(0x00B33830, object_set_region_permutation_direct, object_index, region_name, permutation_name);
}

//.text:00B33960 ; void __cdecl object_set_region_state(int32, int32, const uns8*)
//.text:00B339E0 ; 

void __cdecl object_set_requires_motion(int32 object_index)
{
	INVOKE(0x00B33B50, object_set_requires_motion, object_index);
}

//.text:00B33BC0 ; void __cdecl object_set_scale(int32, real32, real32)
//.text:00B33C90 ; void __cdecl object_set_scale_fast(int32, real32, real32)
//.text:00B33D50 ; void __cdecl object_set_scale_internal(int32, real32, real32, bool)
//.text:00B33E20 ; void __cdecl object_set_scenario_permutation(int32, s_scenario_object_permutation*)

void __cdecl object_set_shadowless(int32 object_index, bool shadowless)
{
	INVOKE(0x00B33E30, object_set_shadowless, object_index, shadowless);
}

//.text:00B33E90 ; void __cdecl object_set_shield_stun(int32, int32)
//.text:00B33EE0 ; void __cdecl object_set_shield_vitality(int32, real32, bool, int16, bool)
//.text:00B33F80 ; void __cdecl object_set_sync_action(int32, int32, int32)

void __cdecl object_set_variant_direct(int32 object_index, int32 variant_name)
{
	INVOKE(0x00B33FC0, object_set_variant_direct, object_index, variant_name);
}

void __cdecl object_set_velocities(int32 object_index, const real_vector3d* linear_velocity, const real_vector3d* angular_velocity)
{
	INVOKE(0x00B34040, object_set_velocities, object_index, linear_velocity, angular_velocity);
}

//.text:00B34130 ; void __cdecl object_set_velocities_direct(int32, const real_vector3d*, const real_vector3d*)
//.text:00B341E0 ; void __cdecl object_set_velocities_internal(int32, const real_vector3d*, const real_vector3d*, bool)
//.text:00B34280 ; void __cdecl object_set_vision_mode_render_default(int32, bool)
//.text:00B342D0 ; bool __cdecl object_should_be_active(int32, const s_game_cluster_bit_vectors*)
//.text:00B34380 ; bool __cdecl object_should_be_deleted_when_deactivated(int32)
//.text:00B343D0 ; bool __cdecl object_start_interpolation(int32, real32)

void* __cdecl object_try_and_get_and_verify_type(int32 object_index, uns32 object_type_mask)
{
	return INVOKE(0x00B34490, object_try_and_get_and_verify_type, object_index, object_type_mask);
}

//.text:00B344E0 ; s_multiplayer_object_properties* __cdecl object_try_and_get_multiplayer(int32 object_index)

void* __cdecl object_try_and_get_unsafe_and_verify_type(int32 object_index, uns32 object_type_mask)
{
	return INVOKE(0x00B34540, object_try_and_get_unsafe_and_verify_type, object_index, object_type_mask);
}

bool __cdecl object_unmarked(int32 object_index)
{
	return INVOKE(0x00B34590, object_unmarked, object_index);
}

void __cdecl object_unregister_scenario_object(int32 object_index)
{
	INVOKE(0x00B345C0, object_unregister_scenario_object, object_index);
}

bool __cdecl object_update(int32 object_index)
{
	return INVOKE(0x00B34630, object_update, object_index);
}

void __cdecl object_update_collision_culling(int32 object_index)
{
	INVOKE(0x00B347F0, object_update_collision_culling, object_index);
}

//.text:00B34870 ; object_update_collision_?

void __cdecl object_update_visibility_culling(int32 object_index)
{
	INVOKE(0x00B348A0, object_update_visibility_culling, object_index);
}

bool __cdecl object_visible_to_any_player(int32 object_index)
{
	return INVOKE(0x00B34930, object_visible_to_any_player, object_index);
}

void __cdecl object_wake(int32 object_index)
{
	INVOKE(0x00B34D00, object_wake, object_index);
}

void __cdecl objects_activation(const s_game_cluster_bit_vectors* a1, const s_game_cluster_bit_vectors* a2)
{
	INVOKE(0x00B34D60, objects_activation, a1, a2);
}

//.text:00B35150 ; bool __cdecl objects_activation_find_scenario_reference_in_currently_active_clusters(const s_game_cluster_bit_vectors*, const s_game_cluster_bit_vectors*, const s_scenario_object_reference*)

bool __cdecl objects_can_connect_to_map()
{
	return INVOKE(0x00B35260, objects_can_connect_to_map);
}

int32 __cdecl objects_compact_memory_pool()
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

void __cdecl objects_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00B35530, objects_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl objects_enable_warthog_chaingun_light(bool enable_warthog_chaingun_light)
{
	INVOKE(0x00B35710, objects_enable_warthog_chaingun_light, enable_warthog_chaingun_light);

	//object_globals->warthog_chaingun_light_disabled = !enable_warthog_chaingun_light;
}

void __cdecl objects_garbage_collection()
{
	INVOKE(0x00B35740, objects_garbage_collection);
}

int32 __cdecl objects_get_active_garbage_count()
{
	return INVOKE(0x00B35790, objects_get_active_garbage_count);
}

render_lighting* __cdecl objects_get_cinematic_lighting()
{
	return INVOKE(0x00B357B0, objects_get_cinematic_lighting);
}

int32 __cdecl objects_get_contiguous_free_memory_size()
{
	return INVOKE(0x00B357D0, objects_get_contiguous_free_memory_size);
}

int32 __cdecl objects_get_first_garbage_object()
{
	return INVOKE(0x00B357F0, objects_get_first_garbage_object);
}

int32 __cdecl objects_get_free_object_header_count()
{
	return INVOKE(0x00B35810, objects_get_free_object_header_count);
}

int32 __cdecl objects_get_next_garbage_object(int32 object_index)
{
	return INVOKE(0x00B35830, objects_get_next_garbage_object, object_index);
}

void __cdecl objects_handle_deleted_object(int32 object_index)
{
	INVOKE(0x00B35870, objects_handle_deleted_object, object_index);
}

void __cdecl objects_handle_deleted_player(int32 object_index)
{
	INVOKE(0x00B35910, objects_handle_deleted_player, object_index);
}

//.text:00B35990 ; int32 __cdecl objects_in_clusters_by_indices(uns32, bool, c_flags<e_object_collision_cull_flag, uns16, 13>, int32, const s_cluster_reference*, int32, int32*)

int32 __cdecl objects_in_sphere(uns32 class_flags, uns32 type_flags, const s_location* location, const real_point3d* center, real32 radius, int32* object_indices, int32 maximum_count)
{
	return INVOKE(0x00B35B60, objects_in_sphere, class_flags, type_flags, location, center, radius, object_indices, maximum_count);
}

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

void __cdecl objects_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00B36310, objects_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

bool __cdecl objects_is_warthog_chaingun_light_enabled()
{
	return INVOKE(0x00B36480, objects_is_warthog_chaingun_light_enabled);

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

void __cdecl objects_setup_structure_bsp_fake_lightprobes(int32 structure_bsp_index)
{
	INVOKE(0x00B367D0, objects_setup_structure_bsp_fake_lightprobes, structure_bsp_index);
}

void __cdecl objects_update()
{
	INVOKE(0x00B36840, objects_update);

	//PROFILER(object_update)
	//{
	//	PROFILER(objects_update)
	//	{
	//		int32* object_early_movers = NULL;
	//		int32 object_early_movers_count = 0;
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
	//		for (int32 object_early_mover_index = 0; object_early_mover_index < object_early_movers_count; object_early_mover_index++)
	//		{
	//			int32 object_index = object_early_movers[object_early_mover_index];
	//			object_header_datum* object_header = object_header_get(object_index);
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
	//			int32 object_update_absolute_index = object_header_iter.get_absolute_index();
	//			object_header_datum* object_header = object_header_iter.get_datum();
	//			//ASSERT(location_valid(&object_header->datum->object.location) == scenario_location_valid(&object_header->datum->object.location));
	//
	//			if (object_header->flags.test(_object_header_active_bit) &&
	//				object_header->flags.test(_object_header_awake_bit) &&
	//				!object_header->flags.test(_object_header_post_update_bit))
	//			{
	//				object_datum* object = object_get(object_header_iter.get_index());
	//				ASSERT(object->object.parent_object_index == NONE);
	//				ASSERT(object->object.next_object_index == NONE);
	//
	//				if (object->object.flags.test(_object_in_limbo_bit))
	//				{
	//					item_datum* item = ITEM_GET(object_header_iter.get_index());
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

void __cdecl objects_update_header_callback(int32 object_index, uns32 datum_handle)
{
	INVOKE(0x00B369A0, objects_update_header_callback, object_index, datum_handle);
}

//.text:00B369E0 ; 
//.text:00B36A30 ; 

void __cdecl recursive_wake_children_awoken_by_movement(int32 object_index)
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

void __cdecl reset_object_cached_render_state(int32 object_index)
{
	INVOKE(0x00B36E20, reset_object_cached_render_state, object_index);
}

void __cdecl reset_object_render_message_queue()
{
	INVOKE(0x00B36E90, reset_object_render_message_queue);
}

//.text:00B36EE0 ; void __cdecl resursive_add_child_objects_to_list(int32, int32, int32)

bool __cdecl sample_object_lighting_from_probes(int32 object_index)
{
	return INVOKE(0x00B36F50, sample_object_lighting_from_probes, object_index);
}

void __cdecl scripted_object_function_set(int32 object_function_index, real32 object_function_value)
{
	INVOKE(0x00B371F0, scripted_object_function_set, object_function_index, object_function_value);
}

//.text:00B373D0 ; void __cdecl set_memory_pool_address(void*)

void object_debug_teleport(int32 object_index, const real_point3d* position)
{
	if (object_datum* object = object_get(object_index))
	{
		havok_can_modify_state_allow();

		if (object->object.flags.test(_object_in_limbo_bit))
		{
			object_set_in_limbo(object_index, false);
		}

		object_set_position_internal(object_index, position, NULL, NULL, NULL, false, true, false, true);

		havok_can_modify_state_disallow();
	}
	else
	{
		event(_event_warning, "Failed to get a valid object in %s.", __FUNCTION__);
	}
}

void object_get_debug_name(int32 object_index, bool full_name, c_static_string<256>* name)
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
	const char* group_tag_name = tag_group_get_name(group_tag);
	const char* tag_name = tag_get_name(object->definition_index);

	if (!full_name)
	{
		tag_name = tag_name_strip_path(tag_get_name(object->definition_index));
	}

	name->append_print("%s.%s|n", tag_name, group_tag_name);

	if (object->object.variant_index == NONE)
	{
		name->append("[default]|n");
	}
	else
	{
		struct object_definition* object_definition = TAG_GET(OBJECT_TAG, struct object_definition, object->definition_index);

		s_model_definition* model_definition = NULL;
		if (object_definition->object.model.index != NONE)
		{
			model_definition = object_definition->object.model.cast_to<s_model_definition>();
		}

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

void object_render_debug_internal(int32 object_index)
{
	const object_header_datum* object_header = object_header_get(object_index);
	object_datum* object = object_get(object_index);

	struct object_definition* object_definition = TAG_GET(OBJECT_TAG, struct object_definition, object->definition_index);

	c_static_string<4096> string;

	if (debug_objects_indices)
	{
		string.append_print("%d (%#x)|n", object_index, object_index);
	}

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
		object_get_debug_name(object_index, debug_objects_names_full, &name);
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
			real32 function_magnitude = 0.0f;
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
		real_vector3d linear_velocity{};

		object_get_world_matrix(object_index, &matrix);
		object_get_velocities(object_index, &linear_velocity, NULL);

		render_debug_matrix(true, &matrix, object->object.bounding_sphere_radius);
		render_debug_vector(true, &matrix.position, &linear_velocity,1.0f, global_real_argb_yellow);
	}

	if (debug_objects_origin)
	{
		real32 a1 = 0.1f;
		real32 a2 = 0.4f;
		real32 seconds = game_ticks_to_seconds(real32(game_time_get()));
		real32 angle = (seconds * TWO_PI) / 3.0f;
		real32 cos_angle = cosine(angle);
		real32 radius = (((cos_angle + 1.0f) * a2) / 2) + a1;

		real_point3d origin{};
		object_get_origin(object_index, &origin);
		render_debug_sphere(true, &origin, radius, global_real_argb_purple);
	}

	if (debug_objects_root_node)
	{
		real_matrix4x3* root_node_matrix = object_get_node_matrix(object_index, 0);
		render_debug_matrix(true, root_node_matrix, object->object.bounding_sphere_radius);
	}

	if (debug_objects_root_node_print)
	{
		real_matrix4x3* root_node_matrix = object_get_node_matrix(object_index, 0);

		string.append_print("Position: %.2f,%.2f,%.2f\r", root_node_matrix->position.x, root_node_matrix->position.y, root_node_matrix->position.z);
		string.append_print("Forward: %.2f,%.2f,%.2f\r", root_node_matrix->forward.i, root_node_matrix->forward.j, root_node_matrix->forward.k);
	}

	if (debug_objects_bounding_spheres)
	{
		int32 parent_object_index = object_get_ultimate_parent(object_index);
		const object_header_datum* parent_object_header = object_header_get(parent_object_index);
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
		point_from_line3d(&object->object.bounding_sphere_center, (real_vector3d*)&object_definition->object.dynamic_light_sphere_offset, 1.0f, &point);
		render_debug_sphere(true, &point, object_definition->object.dynamic_light_sphere_radius + object->object.scale, global_real_argb_black);
	}

	if (debug_objects_model_targets)
	{
		s_model_definition* model_definition = NULL;
		if (object_definition->object.model.index != NONE)
		{
			model_definition = object_definition->object.model.cast_to<s_model_definition>();
		}

		if (model_definition)
		{
			for (s_model_target& target : model_definition->targets)
			{
				object_marker markers[2]{};
				int16 marker_count = object_get_markers_by_string_id(object_index, target.marker_name.get_value(), markers, NUMBEROF(markers));
				switch (marker_count)
				{
				case 1:
				{
					render_debug_vector(true, &markers[0].matrix.position, &markers[0].matrix.forward, target.size, global_real_argb_darkgreen);

					if (target.cone_angle <= 3.1414928f)
					{
						render_debug_cone_outline(true, &markers[0].matrix.position, &markers[0].matrix.forward, target.size, target.cone_angle, global_real_argb_darkgreen);
					}
					else
					{
						render_debug_sphere(true, &markers[0].matrix.position, target.size, global_real_argb_darkgreen);
					}
				}
				break;
				case 2:
				{
					real_vector3d height{};
					vector_from_points3d(&markers[0].matrix.position, &markers[1].matrix.position, &height);
					render_debug_pill(true, &markers[0].matrix.position, &height, target.size, global_real_argb_darkgreen);
				}
				break;
				}
			}
		}
	}

	collision_model_instance instance{};
	if (debug_objects_collision_models && collision_model_instance_new(&instance, object_index))
	{
		render_debug_collision_model(&instance);
	}

	if (debug_objects_early_movers && object_definition->object.flags.test(_object_early_mover_bit))
	{
		const char* early_mover_string = "early mover";
		if (object_definition->object.flags.test(_object_early_mover_localized_physics_bit))
		{
			early_mover_string = "early mover + localized physics";
		}

		real_matrix4x3* root_node_matrix = object_get_node_matrix(object_index, 0);
		render_debug_string_at_point(&root_node_matrix->position, early_mover_string, global_real_argb_darkgreen);
	}

	//real32 object_cpu_times[2];
	//if (debug_objects_profile_times && object_profile_query_object_instance_cpu_times(object_index, &object_cpu_times))
	//{
	//
	//}

	if (object->object.havok_component_index == NONE)
	{
		s_physics_model_instance instance{};
		if (physics_model_instance_new(&instance, object_index) && debug_objects_physics_models)
		{
			render_debug_physics_model(&instance, global_real_argb_black);
		}
	}
	else if (restricted_region_locked_for_current_thread(k_thread_main))
	{
		c_havok_component* havok_component = DATUM_TRY_AND_GET(g_havok_component_data, c_havok_component, object->object.havok_component_index);
		havok_component->render_debug(
			debug_objects_water_physics,
			debug_objects_physics_models,
			debug_objects_expensive_physics,
			debug_objects_contact_points,
			debug_objects_constraints,
			debug_objects_vehicle_physics,
			debug_objects_mass);
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

	if (debug_objects_skeletons)
	{

	}
}

const char* object_describe(int32 object_index)
{
	return watch_object_describe(object_index);
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

