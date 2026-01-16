#pragma once

#include "cseries/cseries.hpp"

// Bungie was kind enough to expose the following in `collision_debug_render`

enum e_collision_test_flag
{
	_collision_test_structure_bit = 0,
	_collision_test_media_bit,
	_collision_test_soft_ceilings_bit,
	_collision_test_instanced_geometry_bit,
	_collision_test_render_only_bsps_bit,
	_collision_test_ignore_child_objects_bit,
	_collision_test_ignore_nonpathfindable_objects_bit,
	_collision_test_ignore_non_flight_blocking_bit,
	_collision_test_ignore_cinematic_objects_bit,
	_collision_test_ignore_dead_bipeds_bit,
	_collision_test_ignore_geometry_that_ignores_aoe_bit,
	_collision_test_front_facing_surfaces_bit,
	_collision_test_back_facing_surfaces_bit,
	_collision_test_ignore_two_sided_surfaces_bit,
	_collision_test_ignore_invisible_surfaces_bit,
	_collision_test_ignore_breakable_surfaces_bit,
	_collision_test_allow_early_out_bit,
	_collision_test_try_to_keep_location_valid_bit,
	_collision_test_ignore_non_prt_objects_bit,
	_collision_test_ignore_non_collision_model_physics_bit,
	_collision_test_ignore_non_camera_camera_collision_bit,
	_collision_test_ignore_invisible_to_projectile_aiming_bit,

	k_collision_test_flags_count
};
COMPILE_ASSERT(MASK(k_collision_test_flags_count) == ~0xFFC00000);

enum e_collision_test_objects_flag
{
	_collision_test_objects_bit = 0,
	_collision_test_objects_first_type_bit,
	_collision_test_objects_bipeds_bit = _collision_test_objects_first_type_bit,         // _object_type_biped
	_collision_test_objects_vehicles_bit,       // _object_type_vehicle
	_collision_test_objects_weapons_bit,        // _object_type_weapon
	_collision_test_objects_equipment_bit,      // _object_type_equipment
	_collision_test_objects_arg_device_bit,     // _object_type_arg_device
	_collision_test_objects_terminals_bit,      // _object_type_terminal
	_collision_test_objects_projectiles_bit,    // _object_type_projectile
	_collision_test_objects_scenery_bit,        // _object_type_scenery
	_collision_test_objects_machines_bit,       // _object_type_machine
	_collision_test_objects_controls_bit,       // _object_type_control
	_collision_test_objects_sound_scenery_bit,  // _object_type_sound_scenery
	_collision_test_objects_crates_bit,         // _object_type_crate
	_collision_test_objects_creatures_bit,      // _object_type_creature
	_collision_test_objects_giants_bit,         // _object_type_giant
	_collision_test_objects_effect_scenery_bit, // _object_type_effect_scenery
	_collision_test_objects_last_type_bit = _collision_test_objects_effect_scenery_bit,

	k_collision_test_objects_flags_count
};
COMPILE_ASSERT(MASK(k_collision_test_objects_flags_count) == ~0xFFFF0000);

struct s_collision_test_flags
{
	c_flags<e_collision_test_flag, uns32, k_collision_test_flags_count> collision_flags;
	c_flags<e_collision_test_objects_flag, uns32, k_collision_test_objects_flags_count> object_flags;
};
COMPILE_ASSERT(sizeof(s_collision_test_flags) == 0x8);

extern s_collision_test_flags& k_trivial_obstruction_collision_flags;
extern s_collision_test_flags& k_perception_collision_flags;
extern s_collision_test_flags& _collision_test_footstep_effect_flags;
extern s_collision_test_flags& _collision_test_fix_position_flags;
extern s_collision_test_flags& _collision_test_for_sound_transmission_flags;
extern s_collision_test_flags& _collision_test_for_line_of_sight_obstruction_flags;
extern s_collision_test_flags& _collision_test_for_damage_flags;
extern s_collision_test_flags& _collision_test_for_editor_flags;
extern s_collision_test_flags& _collision_test_pathfinding_structure_only_flags;
extern s_collision_test_flags& k_communication_collision_flags;
extern s_collision_test_flags& _collision_test_pathfinding_flags;
extern s_collision_test_flags& _collision_test_ragdoll_bring_inside_world_flags;
extern s_collision_test_flags& k_movement_collision_flags;
extern s_collision_test_flags& _collision_test_environment_flags;
extern s_collision_test_flags& _collision_test_for_biped_sight_position_offset_flags;
extern s_collision_test_flags& _collision_test_for_projectiles_fat_flags;
extern s_collision_test_flags& _collision_test_for_melee_obstruction_flags;
extern s_collision_test_flags& _collision_test_objects_all_types_flags;
extern s_collision_test_flags& _collision_test_for_line_of_sight_flags;
extern s_collision_test_flags& _collision_test_objects_sight_blocking_flags;
extern s_collision_test_flags& _collision_test_for_unit_access_obstruction_flags;
extern s_collision_test_flags& _collision_test_for_vehicles_flags;
extern s_collision_test_flags& k_perception_early_out_collision_flags;
extern s_collision_test_flags& _collision_test_for_projectiles_flags;
extern s_collision_test_flags& _collision_test_for_damage_obstruction_flags;
extern s_collision_test_flags& _collision_test_unit_enter_seat_flags;
extern s_collision_test_flags& _collision_test_empty_flags;
extern s_collision_test_flags& _collision_test_structure_geometry_flags;

