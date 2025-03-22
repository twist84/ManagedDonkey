#include "physics/impacts.hpp"

#include "memory/thread_local.hpp"
#include "physics/havok_component.hpp"

//.text:00696950 ; 
//.text:00696970 ; 
//.text:00696990 ; 
//.text:006969B0 ; 
//.text:006969C0 ; 
//.text:006969D0 ; 
//.text:006969E0 ; 

void __cdecl __tls_set_g_impact_array_data_allocator(void* new_address)
{
	INVOKE(0x00696A00, __tls_set_g_impact_array_data_allocator, new_address);

	//TLS_DATA_GET_VALUE_REFERENCE(g_impact_array_data);
	//data_set_new_base_address(&g_impact_array_data.m_data_array, (s_data_array*)new_address);
}

void __cdecl __tls_set_g_impact_data_allocator(void* new_address)
{
	INVOKE(0x00696A30, __tls_set_g_impact_data_allocator, new_address);

	//TLS_DATA_GET_VALUE_REFERENCE(g_impact_data);
	//data_set_new_base_address(&g_impact_data.m_data_array, (s_data_array*)new_address);
}

void __cdecl __tls_set_g_impact_globals_allocator(void* new_address)
{
	INVOKE(0x00696A60, __tls_set_g_impact_globals_allocator, new_address);

	//TLS_DATA_GET_VALUE_REFERENCE(g_impact_globals);
	//g_impact_globals = (impact_globals*)new_address;
}

//.text:00696A80 ; public: void c_impact::add_reference(long)
//.text:00696A90 ; 
//.text:00696AD0 ; 
//.text:00696B10 ; 
//.text:00696B50 ; 
//.text:00696B70 ; 
//.text:00696B90 ; public: void c_impact::calculate_effect_parameters(long, long, long, real_point3d*, real_vector3d*, real_vector3d*, real_vector3d*, real*, real*, bool)
//.text:00697390 ; private: void c_impact::calculate_impact_matrix(long, real_matrix3x3*)
//.text:00697620 ; private: void c_impact::calculate_rigid_body_indexes(long, long*, long*)
//.text:006977C0 ; public: static real __cdecl c_impact::calculate_score(real_point3d const*, c_impact::e_states)
//.text:00697870 ; public: static void __cdecl c_impact::calculate_water_surface_vector(real_point3d*, real_vector3d*)
//.text:006978F0 ; public: static bool __cdecl c_impact::can_impulse_state_interupt_looping_state(long, real, long, real)
//.text:00697920 ; private: bool c_impact::currently_playing_looping_sound(long, long)
//.text:006979D0 ; 
//.text:00697A00 ; 
//.text:00697A30 ; 
//.text:00697A60 ; public: void c_impact::disconnect_from_physics()
//.text:00697A80 ; 
//.text:00697AB0 ; 
//.text:00697AE0 ; 
//.text:00697B10 ; 
//.text:00697B20 ; 
//.text:00697B30 ; 
//.text:00697B50 ; 
//.text:00697B60 ; public: c_global_material_type c_impact::get_global_material_index_a() const
//.text:00697B80 ; public: c_global_material_type c_impact::get_global_material_index_b() const
//.text:00697BA0 ; real __cdecl get_impact_maximum_collision_magnitude(e_impact_sound_scale_type)
//.text:00697BE0 ; real __cdecl get_impact_minimum_collision_magnitude(e_impact_sound_scale_type)
//.text:00697C20 ; real __cdecl get_impact_minimum_collision_pre_simulation_angular_velocity_magnitude(e_impact_sound_scale_type)
//.text:00697C90 ; real __cdecl get_impact_minimum_collision_pre_simulation_linear_velocity_magnitude(e_impact_sound_scale_type)
//.text:00697CA0 ; e_impact_sound_scale_type __cdecl get_impact_sound_scale_type(long, long, long)
//.text:00697DF0 ; private: static real __cdecl c_impact::get_impact_state_score_scale(c_impact::e_states)
//.text:00697E20 ; 
//.text:00697E30 ; private: void c_impact::get_material_effects(real_point3d const*, c_global_material_type, c_global_material_type, c_impact::e_states, s_material_effects_result*)
//.text:00697F10 ; private: static void __cdecl c_impact::get_material_effects(long, long, real_vector3d const*, e_sweetener_size, bool, real_point3d const*, c_global_material_type, c_global_material_type, c_impact::e_states, s_material_effects_result*)
//.text:00698010 ; public: real_vector3d const* c_impact::get_normal() const
//.text:00698020 ; public: long c_impact::get_object_index_a() const
//.text:00698030 ; public: long c_impact::get_object_index_b() const
//.text:00698040 ; 
//.text:00698070 ; public: long c_impact::get_reference_count() const
//.text:00698080 ; public: real c_impact::get_score() const
//.text:00698090 ; public: c_impact::e_states c_impact::get_state() const
//.text:006980A0 ; public: c_impact::e_type c_impact::get_type() const
//.text:006980B0 ; public: long c_impact::get_vehicle_friction_point_index() const
//.text:006980C0 ; public: bool c_impact::get_vehicle_friction_point_sound_only() const
//.text:006980D0 ; public: real_point3d const* c_impact::get_world_space_position() const
//.text:006980E0 ; private: void c_impact::halt_managed_effects(bool, bool)

void __cdecl impact_array_add_impact(long impact_array_index, long impact_datum_index)
{
	INVOKE(0x00698180, impact_array_add_impact, impact_array_index, impact_datum_index);
}

void __cdecl impact_array_delete(long impact_array_index)
{
	INVOKE(0x006981C0, impact_array_delete, impact_array_index);
}

long __cdecl impact_array_impact_count(long impact_array_index)
{
	return INVOKE(0x006981F0, impact_array_impact_count, impact_array_index);
}

long __cdecl impact_array_impact_get(long impact_array_index, long impact_array_impact_index)
{
	return INVOKE(0x00698220, impact_array_impact_get, impact_array_index, impact_array_impact_index);
}

long __cdecl impact_array_new(long object_index)
{
	return INVOKE(0x00698260, impact_array_new, object_index);
}

void __cdecl impact_array_remove_impact(long impact_array_index, long impact_datum_index)
{
	INVOKE(0x006982B0, impact_array_remove_impact, impact_array_index, impact_datum_index);
}

void __cdecl impact_delete(long impact_datum_index)
{
	INVOKE(0x00698310, impact_delete, impact_datum_index);
}

long __cdecl impact_new(c_impact::s_contact_description const* contact_description, c_impact::e_states state)
{
	return INVOKE(0x006984F0, impact_new, contact_description, state);
}

void __cdecl impacts_create_constraint_impacts_for_havok_component(long object_index)
{
	INVOKE(0x00698780, impacts_create_constraint_impacts_for_havok_component, object_index);
}

void __cdecl impacts_create_orphaned_impact(long object_index, c_havok_contact_point* contact_point, bool create_new_impacts)
{
	INVOKE(0x00698A10, impacts_create_orphaned_impact, object_index, contact_point, create_new_impacts);
}

void __cdecl impacts_create_orphaned_impacts()
{
	INVOKE(0x00698C90, impacts_create_orphaned_impacts);
}

void __cdecl impacts_disconnect_from_physics()
{
	INVOKE(0x00698DD0, impacts_disconnect_from_physics);
}

bool __cdecl impacts_disconnected_from_physics()
{
	return INVOKE(0x00698E90, impacts_disconnected_from_physics);
}

void __cdecl impacts_dispose()
{
	INVOKE(0x00698EB0, impacts_dispose);
}

void __cdecl impacts_dispose_from_old_map()
{
	INVOKE(0x00698EF0, impacts_dispose_from_old_map);
}

void __cdecl impacts_dispose_global_allocations()
{
	INVOKE(0x00698F20, impacts_dispose_global_allocations);
}

void __cdecl impacts_flush()
{
	INVOKE(0x00698F30, impacts_flush);
}

long __cdecl impacts_get_highest_score_datum_index()
{
	return INVOKE(0x00698F60, impacts_get_highest_score_datum_index);
}

void __cdecl impacts_initialize()
{
	INVOKE(0x00698FB0, impacts_initialize);
}

void __cdecl impacts_initialize_for_new_map()
{
	INVOKE(0x00699050, impacts_initialize_for_new_map);
}

void __cdecl impacts_initialize_global_allocations()
{
	INVOKE(0x006990B0, impacts_initialize_global_allocations);
}

void __cdecl impacts_load_from_game_state()
{
	INVOKE(0x006990C0, impacts_load_from_game_state);
}

void __cdecl impacts_merge()
{
	INVOKE(0x00699130, impacts_merge);
}

void __cdecl impacts_notify_object_deleted(long object_index)
{
	INVOKE(0x00699460, impacts_notify_object_deleted, object_index);
}

void __cdecl impacts_pop_highest_score_datum_index()
{
	INVOKE(0x00699500, impacts_pop_highest_score_datum_index);
}

void __cdecl impacts_quicksort()
{
	INVOKE(0x00699530, impacts_quicksort);
}

void __cdecl impacts_reconnect_to_physics()
{
	INVOKE(0x00699600, impacts_reconnect_to_physics);
}

bool __cdecl impacts_score_compare(long impact_index_a, long impact_index_b, void const* ignored)
{
	return INVOKE(0x006996C0, impacts_score_compare, impact_index_a, impact_index_b, ignored);
}

void __cdecl impacts_update()
{
	INVOKE(0x00699710, impacts_update);
}

//.text:00699A30 ; public: static bool __cdecl c_impact::included_in_impact_material(long, c_global_material_type, c_global_material_type, c_global_material_type, c_global_material_type)
//.text:00699A60 ; public: static bool __cdecl c_impact::included_in_impact_space(real_point3d const*, real_vector3d const*, real_point3d const*, real_vector3d const*, real*)
//.text:00699B10 ; public: static bool __cdecl c_impact::included_in_impact_type(long, long, long, long, c_global_material_type, c_global_material_type, c_global_material_type, c_global_material_type, long, long, bool, bool, long, long)
//.text:00699BB0 ; public: bool c_impact::includes_point(c_impact::s_contact_description const*, bool)
//.text:00699C70 ; public: static void __cdecl c_impact::initialize_contact_description(c_impact::s_contact_description*, c_impact::e_type, bool, long, long, c_global_material_type, long, long, c_global_material_type, real_point3d const*, real_vector3d const*, long, s_physics_model_constraint_reference const*)
//.text:00699D00 ; private: static bool __cdecl c_impact::is_b_dominant_body(long, long)
//.text:00699D90 ; public: bool c_impact::is_between_pair_of_dynamic_rigid_bodies() const
//.text:00699DA0 ; public: bool c_impact::is_managing_effect() const
//.text:00699DD0 ; 
//.text:00699DF0 ; public: static bool __cdecl c_impact::passes_the_character_test(long, long, real_point3d const*, real_vector3d const*)
//.text:00699F10 ; public: void c_impact::queue_collision(real)
//.text:00699F90 ; public: bool c_impact::recently_played_impulse_effect() const
//.text:00699FD0 ; public: void c_havok_component::remove_impact(long)
//.text:0069A080 ; public: void c_impact::remove_reference(long, bool)
//.text:0069A100 ; 
//.text:0069A140 ; 
//.text:0069A180 ; 
//.text:0069A1C0 ; 
//.text:0069A1D0 ; 
//.text:0069A1F0 ; public: void c_impact::update(long)
//.text:0069A990 ; public: void c_impact::update_effects(long, long, long)
//.text:0069B470 ; public: void c_impact::update_looping_sound(s_sound_location*, real*) const
//.text:0069B730 ; private: void c_impact::update_rigid_body_space_positions(real_point3d const*, long, long)
//.text:0069B830 ; public: void c_impact::update_score()
//.text:0069B890 ; public: void c_impact::update_state(long, long, long)
//.text:0069BF10 ; public: void c_impact::update_vehicle_impact(long, c_impact::s_contact_description const*, real_vector3d const*, real, bool)
//.text:0069BF70 ; public: void c_impact::update_vehicle_impact_sound_only(long)
//.text:0069C0C0 ; public: void c_impact::update_water_impact(long, real, bool)

