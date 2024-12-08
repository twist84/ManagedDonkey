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

//.text:00BAD3B0 ; void __cdecl biped_adjust_aiming(long, real_euler_angles2d*, real*, real*)
//.text:00BADC30 ; void __cdecl biped_build_axes_from_desired_facing(real_vector3d const*, real_vector3d const*, real_vector3d const*, real_vector3d*)
//.text:00BAE000 ; double __cdecl biped_calculate_crouch_delta(long)

bool __cdecl biped_calculate_crouch_velocity_delta(long biped_index, real* crouch_velocity_delta)
{
	return INVOKE(0x00BAE130, biped_calculate_crouch_velocity_delta, biped_index, crouch_velocity_delta);
}

//.text:00BAE1F0 ; bool __cdecl biped_can_melee_target_unit(long, long)
//.text:00BAE380 ; void __cdecl biped_clear_pathfinding_cache(long)

void __cdecl biped_falling_damage(long biped_index)
{
	//INVOKE(0x00BAE400, biped_falling_damage, biped_index);

	biped_datum* biped = biped_get(biped_index);
	struct biped_definition* biped_definition = (struct biped_definition*)tag_get(BIPED_TAG, biped->definition_index);

	void* motor_state = object_header_block_get(biped_index, &biped->mover.motor_state);
	REFERENCE_DECLARE(offset_pointer(motor_state, 4), byte_flags, motor_state_flags);

	s_game_globals_falling_damage& falling_damage = scenario_get_game_globals()->falling_damage[0];

	bool immune_to_falling_damage = 
		TEST_BIT(biped->unit.flags, 8) ||
		biped_definition->biped.flags.test(_biped_definition_flags_immune_to_falling_damage) || 
		TEST_BIT(biped->object.damage_flags, 8);

	real gravity_scale = biped_definition->biped.physics.ground_physics.gravity_scale;
	if (gravity_scale <= 0.0f)
		gravity_scale = 1.0f;

	if (!game_is_multiplayer() && biped->object.parent_object_index == NONE)
	{
		real_vector3d linear_velocity{};
		object_get_velocities(biped_index, &linear_velocity, NULL);

		real k = linear_velocity.k;
		real crouch_velocity_delta = 0.0f;
		if (TEST_BIT(motor_state_flags, 0) && biped_calculate_crouch_velocity_delta(biped_index, &crouch_velocity_delta))
			k -= crouch_velocity_delta;

		real_vector3d linear_local_space_velocity{};
		real_vector3d angular_local_space_velocity{};
		if (object_get_early_mover_local_space_velocity(biped_index, &linear_local_space_velocity, &angular_local_space_velocity, false, false))
			k -= linear_local_space_velocity.k;

		ASSERT(!game_is_predicted());

		if ((!cheat.jetpack /*&& !game_skull_is_active(45)*/ || biped->unit.player_index == NONE) &&
			biped->biped.physics.get_mode() == c_character_physics_component::e_mode::_mode_ground &&
			-(falling_damage.runtime_maximum_falling_velocity * gravity_scale) > k)
		{
			if (!immune_to_falling_damage && !TEST_BIT(biped->object.damage_flags, 2))
			{
				SET_BIT(biped->biped.flags, 5, true);
				s_damage_data damage_data{};
				damage_data.material_type = c_global_material_type();
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
				generate_event(_event_warning, "WARNING: %s reached terminal velocity outside world (%f %f %f) and was erased",
					object_describe(biped_index),
					biped->object.position.x,
					biped->object.position.y,
					biped->object.position.z);

				object_delete(biped_index);
			}
		}
	}
}

//.text:00BAE600 ; void __cdecl biped_get_control_vectors(long, bool, real_vector3d*, real_vector3d*, real_vector3d*)
//.text:00BAE8D0 ; double __cdecl biped_get_player_specific_scale(long)
//.text:00BAE9D0 ; void __cdecl biped_ground_plane_fix_transform_from_physics(long, real_matrix4x3*)
//.text:00BAED40 ; void __cdecl biped_ground_plane_fix_transform_to_physics(long, real_matrix4x3*)
//.text:00BAF0C0 ; void __cdecl biped_initialize_character_physics_move_input(long, s_character_physics_move_input_datum*, bool, bool, bool, bool, bool)
//.text:00BAF280 ; void __cdecl biped_initialize_character_physics_update_input(long, s_character_physics_update_input_datum*, bool, bool, real, bool, bool)
//.text:00BAF990 ; void __cdecl biped_move_calculate_controlled_by_aiming(long, real_vector3d const*, real_vector3d const*, real_vector3d const*, bool, real_vector3d*, short*)
//.text:00BB01B0 ; void __cdecl biped_physics_update_crouch(long, s_character_physics_update_output_datum*, bool*)
//.text:00BB0790 ; void __cdecl biped_set_forward_vector(long, real_vector3d const*)
//.text:00BB08A0 ; void __cdecl biped_snap_facing_to_desired_facing(long)
//.text:00BB08E0 ; void __cdecl biped_update_desired_facing(long, bool, short*, real_vector3d*, bool*)
//.text:00BB0B90 ; void __cdecl biped_update_pathfinding_cache(long, s_character_physics_move_output_datum const*)
//.text:00BB0D60 ; 
//.text:00BB0D70 ; public: class c_animation_id __cdecl c_animation_manager::get_active_animation_id() const
//.text:00BB0D90 ; 
//.text:00BB0E10 ; bool __cdecl motor_system_biped_move(long, bool, bool, bool, bool, s_character_physics_move_output_datum*)
//.text:00BB1890 ; void __cdecl motor_system_biped_physics_postupdate_moving(long, s_character_physics_update_input_datum const*, s_character_physics_update_output_datum*, bool*)
//.text:00BB1A50 ; bool __cdecl motor_system_biped_physics_preupdate_moving(long, s_character_physics_update_input_datum*, s_character_physics_update_output_datum*, bool, bool, real, bool, bool)

