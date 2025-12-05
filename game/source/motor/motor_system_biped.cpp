#include "motor/motor_system_biped.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/cheats.hpp"
#include "game/game.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "scenario/scenario.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "units/biped_definitions.hpp"
#include "units/bipeds.hpp"

HOOK_DECLARE(0x00BAE400, biped_falling_damage);

//.text:00BAD3B0 ; void __cdecl biped_adjust_aiming(int32, real_euler_angles2d*, real32*, real32*)
//.text:00BADC30 ; void __cdecl biped_build_axes_from_desired_facing(const real_vector3d*, const real_vector3d*, const real_vector3d*, real_vector3d*)
//.text:00BAE000 ; real64 __cdecl biped_calculate_crouch_delta(int32)

bool __cdecl biped_calculate_crouch_velocity_delta(int32 biped_index, real32* crouch_velocity_delta)
{
	return INVOKE(0x00BAE130, biped_calculate_crouch_velocity_delta, biped_index, crouch_velocity_delta);
}

//.text:00BAE1F0 ; bool __cdecl biped_can_melee_target_unit(int32, int32)
//.text:00BAE380 ; void __cdecl biped_clear_pathfinding_cache(int32)

void __cdecl biped_falling_damage(int32 biped_index)
{
	//INVOKE(0x00BAE400, biped_falling_damage, biped_index);

	biped_datum* biped = BIPED_GET(biped_index);
	struct biped_definition* biped_definition = TAG_GET(BIPED_TAG, struct biped_definition, biped->definition_index);

	void* motor_state = OBJECT_HEADER_BLOCK_GET(biped_index, void, &biped->mover.motor_state);
	REFERENCE_DECLARE(offset_pointer(motor_state, 4), uns8, motor_state_flags);

	s_game_globals_falling_damage& falling_damage = scenario_get_game_globals()->falling_damage[0];

	bool immune_to_falling_damage = 
		TEST_BIT(biped->unit.flags, 8) ||
		biped_definition->biped.flags.test(_biped_definition_flags_immune_to_falling_damage) || 
		biped->object.damage_flags.test(_object_cannot_take_damage_bit);

	real32 gravity_scale = biped_definition->biped.physics.ground.gravity_scale;
	if (gravity_scale <= 0.0f)
	{
		gravity_scale = 1.0f;
	}

	if (!game_is_multiplayer() && biped->object.parent_object_index == NONE)
	{
		real_vector3d linear_velocity{};
		object_get_velocities(biped_index, &linear_velocity, NULL);

		real32 k = linear_velocity.k;
		real32 crouch_velocity_delta = 0.0f;
		if (TEST_BIT(motor_state_flags, 0) && biped_calculate_crouch_velocity_delta(biped_index, &crouch_velocity_delta))
		{
			k -= crouch_velocity_delta;
		}

		real_vector3d linear_local_space_velocity{};
		real_vector3d angular_local_space_velocity{};
		if (object_get_early_mover_local_space_velocity(biped_index, &linear_local_space_velocity, &angular_local_space_velocity, false, false))
		{
			k -= linear_local_space_velocity.k;
		}

		ASSERT(!game_is_predicted());

		if ((!cheat.jetpack /*&& !game_skull_is_active(45)*/ || biped->unit.player_index == NONE) &&
			biped->biped.physics.get_mode() == c_character_physics_component::e_mode::_mode_ground &&
			-(falling_damage.runtime_maximum_falling_velocity * gravity_scale) > k)
		{
			if (!immune_to_falling_damage && !biped->object.damage_flags.test(_object_dead_bit))
			{
				biped->biped.flags.set(_biped_falling_to_his_death_bit, true);
				s_damage_data damage_data{};
				damage_data.global_material_type = c_global_material_type();
				damage_data_new(&damage_data, falling_damage.distance_damage.index);
				SET_BIT(damage_data.flags, 2, true);
				damage_data.damage_reporting_info.type = _damage_reporting_type_falling_damage;
				damage_compute_damage_owner_from_object_index(biped_index, &damage_data.damage_owner);
				object_cause_damage_simple(&damage_data, biped_index, 5);
			}

			if (!game_is_multiplayer() &&
				biped->object.flags.test(_object_outside_of_map_bit) &&
				biped->unit.player_index == NONE &&
				!simulation_query_object_is_predicted(biped_index))
			{
				event(_event_warning, "WARNING: %s reached terminal velocity outside world (%f %f %f) and was erased",
					object_describe(biped_index),
					biped->object.position.x,
					biped->object.position.y,
					biped->object.position.z);

				object_delete(biped_index);
			}
		}
	}
}

//.text:00BAE600 ; void __cdecl biped_get_control_vectors(int32, bool, real_vector3d*, real_vector3d*, real_vector3d*)
//.text:00BAE8D0 ; real64 __cdecl biped_get_player_specific_scale(int32)
//.text:00BAE9D0 ; void __cdecl biped_ground_plane_fix_transform_from_physics(int32, real_matrix4x3*)
//.text:00BAED40 ; void __cdecl biped_ground_plane_fix_transform_to_physics(int32, real_matrix4x3*)
//.text:00BAF0C0 ; void __cdecl biped_initialize_character_physics_move_input(int32, s_character_physics_move_input_datum*, bool, bool, bool, bool, bool)
//.text:00BAF280 ; void __cdecl biped_initialize_character_physics_update_input(int32, s_character_physics_update_input_datum*, bool, bool, real32, bool, bool)
//.text:00BAF990 ; void __cdecl biped_move_calculate_controlled_by_aiming(int32, const real_vector3d*, const real_vector3d*, const real_vector3d*, bool, real_vector3d*, int16*)
//.text:00BB01B0 ; void __cdecl biped_physics_update_crouch(int32, s_character_physics_update_output_datum*, bool*)
//.text:00BB0790 ; void __cdecl biped_set_forward_vector(int32, const real_vector3d*)
//.text:00BB08A0 ; void __cdecl biped_snap_facing_to_desired_facing(int32)
//.text:00BB08E0 ; void __cdecl biped_update_desired_facing(int32, bool, int16*, real_vector3d*, bool*)
//.text:00BB0B90 ; void __cdecl biped_update_pathfinding_cache(int32, const s_character_physics_move_output_datum*)
//.text:00BB0D60 ; 
//.text:00BB0D70 ; public: class c_animation_id __cdecl c_animation_manager::get_active_animation_id() const
//.text:00BB0D90 ; 
//.text:00BB0E10 ; bool __cdecl motor_system_biped_move(int32, bool, bool, bool, bool, s_character_physics_move_output_datum*)
//.text:00BB1890 ; void __cdecl motor_system_biped_physics_postupdate_moving(int32, const s_character_physics_update_input_datum*, s_character_physics_update_output_datum*, bool*)
//.text:00BB1A50 ; bool __cdecl motor_system_biped_physics_preupdate_moving(int32, s_character_physics_update_input_datum*, s_character_physics_update_output_datum*, bool, bool, real32, bool, bool)

