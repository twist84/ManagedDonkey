#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"

struct impact_globals
{
	long highest_score_index;
	long sorted_indexes[32];
	bool disconnected_from_physics;
	long object_iteration_next_absolute_index;
};
static_assert(sizeof(impact_globals) == 0x8C);

struct impact_array_datum :
	s_datum_header
{
	short impact_indexes_count;
	long impact_indexes[32];
	long object_index;
};
static_assert(sizeof(impact_array_datum) == 0x88);

// $TODO: find a home
struct s_physics_model_constraint_reference
{
	short constraint_type;
	short constraint_type_index;
};
static_assert(sizeof(s_physics_model_constraint_reference) == 0x4);

struct c_impact :
	s_datum_header
{
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
		byte type;
		short explicit_pad0;
		long object_index_a;
		long rigid_body_index_a;
		c_global_material_type global_material_a;
		long object_index_b;
		long rigid_body_index_b;
		c_global_material_type global_material_b;
		real_point3d world_space_position;
		real_vector3d normal;
		long vehicle_friction_point_index;
		bool vehicle_friction_point_sound_only;
		byte explicit_pad1;
		short explicit_pad2;
		s_physics_model_constraint_reference constraint_reference;
	};
	static_assert(sizeof(s_contact_description) == 0x40);

	byte_flags m_flags;
	char m_score_sorted_order;
	e_type m_type;
	real m_score;
	short m_reference_count;
	char m_ticks_unreferenced;
	char m_sweetener_size;
	char m_state;
	char m_vehicle_friction_point_index;
	bool m_vehicle_friction_point_sound_only;
	bool m_vehicle_friction_point_breaking;
	long m_last_time_in_collision_state;
	char m_ticks_at_collision_state;
	bool m_queued_collision;
	real m_best_collision_scale;
	real m_queued_collision_scale;
	char m_ticks_at_impulse_effect_state;
	char m_looping_effect_state;
	char m_ticks_at_looping_effect_state;
	char m_desired_looping_effect_state;
	char m_ticks_desiring_looping_effect_state;
	long m_object_index_a;
	long m_object_index_b;
	c_global_material_type m_global_material_index_a;
	c_global_material_type m_global_material_index_b;
	long m_looping_sound_index_a;
	long m_looping_sound_definition_index_a;
	long m_looping_sound_index_b;
	long m_looping_sound_definition_index_b;
	long m_effect_index_a;
	long m_effect_secondary_index_a;
	long m_effect_index_b;
	real m_collision_magnitude;
	real m_grinding_magnitude;
	real m_grinding_scale;
	real m_rolling_scale;
	real_vector3d m_effect_direction;
	real_vector3d m_normal;
	real_point3d m_world_space_position;
	real_point3d m_rigid_body_space_position_a;
	real_point3d m_rigid_body_space_position_b;
	real m_last_pre_simulation_velocity_squared_maximum;
	char m_last_pre_simulation_velocity_squared_age;
	char m_ticks_at_looping_state;
	long m_last_collision_effect_game_time;
	real m_last_collision_effect_scale;
	s_physics_model_constraint_reference m_constraint_reference;
};
static_assert(sizeof(c_impact) == 0xB4);


extern void __cdecl impacts_update();

