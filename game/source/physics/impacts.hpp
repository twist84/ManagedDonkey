#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"

struct impact_globals
{
	int32 highest_score_index;
	int32 sorted_indexes[32];
	bool disconnected_from_physics;
	int32 object_iteration_next_absolute_index;
};
static_assert(sizeof(impact_globals) == 0x8C);

struct impact_array_datum :
	s_datum_header
{
	int16 impact_indexes_count;
	int32 impact_indexes[32];
	int32 object_index;
};
static_assert(sizeof(impact_array_datum) == 0x88);

// $TODO find a home
struct s_physics_model_constraint_reference
{
	int16 constraint_type;
	int16 constraint_type_index;
};
static_assert(sizeof(s_physics_model_constraint_reference) == 0x4);

class c_impact :
	public s_datum_header
{
public:
	enum e_flags
	{
		_flag_active_bit = 0,
		_flag_havok_component_b_is_dominant_bit,

		k_flag_count
	};

	enum e_type
	{
		_type_friction = 0,
		_type_contact,
		_type_constraint,
		_type_water,

		k_type_count
	};

	enum e_states
	{
		_state_collision = 0,
		_state_contact,

		_first_looping_state,
		_state_rolling = _first_looping_state,
		_state_grinding,
		_last_looping_state = _state_grinding,

		k_state_count
	};

	struct s_contact_description
	{
		bool new_contact;
		uns8 type;
		int16 explicit_pad0;
		int32 object_index_a;
		int32 rigid_body_index_a;
		c_global_material_type global_material_a;
		int32 object_index_b;
		int32 rigid_body_index_b;
		c_global_material_type global_material_b;
		real_point3d world_space_position;
		real_vector3d normal;
		int32 vehicle_friction_point_index;
		bool vehicle_friction_point_sound_only;
		uns8 explicit_pad1;
		int16 explicit_pad2;
		s_physics_model_constraint_reference constraint_reference;
	};
	static_assert(sizeof(s_contact_description) == 0x40);

	uns8 m_flags;
	int8 m_score_sorted_order;
	e_type m_type;
	real32 m_score;
	int16 m_reference_count;
	int8 m_ticks_unreferenced;
	int8 m_sweetener_size;
	int8 m_state;
	int8 m_vehicle_friction_point_index;
	bool m_vehicle_friction_point_sound_only;
	bool m_vehicle_friction_point_breaking;
	int32 m_last_time_in_collision_state;
	int8 m_ticks_at_collision_state;
	bool m_queued_collision;
	real32 m_best_collision_scale;
	real32 m_queued_collision_scale;
	int8 m_ticks_at_impulse_effect_state;
	int8 m_looping_effect_state;
	int8 m_ticks_at_looping_effect_state;
	int8 m_desired_looping_effect_state;
	int8 m_ticks_desiring_looping_effect_state;
	int32 m_object_index_a;
	int32 m_object_index_b;
	c_global_material_type m_global_material_index_a;
	c_global_material_type m_global_material_index_b;
	int32 m_looping_sound_index_a;
	int32 m_looping_sound_definition_index_a;
	int32 m_looping_sound_index_b;
	int32 m_looping_sound_definition_index_b;
	int32 m_effect_index_a;
	int32 m_effect_secondary_index_a;
	int32 m_effect_index_b;
	real32 m_collision_magnitude;
	real32 m_grinding_magnitude;
	real32 m_grinding_scale;
	real32 m_rolling_scale;
	real_vector3d m_effect_direction;
	real_vector3d m_normal;
	real_point3d m_world_space_position;
	real_point3d m_rigid_body_space_position_a;
	real_point3d m_rigid_body_space_position_b;
	real32 m_last_pre_simulation_velocity_squared_maximum;
	int8 m_last_pre_simulation_velocity_squared_age;
	int8 m_ticks_at_looping_state;
	int32 m_last_collision_effect_game_time;
	real32 m_last_collision_effect_scale;
	s_physics_model_constraint_reference m_constraint_reference;
};
static_assert(sizeof(c_impact) == 0xB4);

class c_havok_contact_point;

extern void __cdecl __tls_set_g_impact_array_data_allocator(void* new_address);
extern void __cdecl __tls_set_g_impact_data_allocator(void* new_address);
extern void __cdecl __tls_set_g_impact_globals_allocator(void* new_address);
extern void __cdecl impact_array_add_impact(int32 impact_array_index, int32 impact_datum_index);
extern void __cdecl impact_array_delete(int32 impact_array_index);
extern int32 __cdecl impact_array_impact_count(int32 impact_array_index);
extern int32 __cdecl impact_array_impact_get(int32 impact_array_index, int32 impact_array_impact_index);
extern int32 __cdecl impact_array_new(int32 object_index);
extern void __cdecl impact_array_remove_impact(int32 impact_array_index, int32 impact_datum_index);
extern void __cdecl impact_delete(int32 impact_datum_index);
extern int32 __cdecl impact_new(const c_impact::s_contact_description* contact_description, c_impact::e_states state);
extern void __cdecl impacts_create_constraint_impacts_for_havok_component(int32 object_index);
extern void __cdecl impacts_create_orphaned_impact(int32 object_index, c_havok_contact_point* contact_point, bool create_new_impacts);
extern void __cdecl impacts_create_orphaned_impacts();
extern void __cdecl impacts_disconnect_from_physics();
extern bool __cdecl impacts_disconnected_from_physics();
extern void __cdecl impacts_dispose();
extern void __cdecl impacts_dispose_from_old_map();
extern void __cdecl impacts_dispose_global_allocations();
extern void __cdecl impacts_flush();
extern int32 __cdecl impacts_get_highest_score_datum_index();
extern void __cdecl impacts_initialize();
extern void __cdecl impacts_initialize_for_new_map();
extern void __cdecl impacts_initialize_global_allocations();
extern void __cdecl impacts_load_from_game_state();
extern void __cdecl impacts_merge();
extern void __cdecl impacts_notify_object_deleted(int32 object_index);
extern void __cdecl impacts_pop_highest_score_datum_index();
extern void __cdecl impacts_quicksort();
extern void __cdecl impacts_reconnect_to_physics();
extern bool __cdecl impacts_score_compare(int32 impact_index_a, int32 impact_index_b, const void* ignored);
extern void __cdecl impacts_update();

