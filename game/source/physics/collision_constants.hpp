#pragma once

#include "cseries/cseries.hpp"

// Bungie was kind enough to expose the following in `collision_debug_render`

// #TODO: confirm the following bits
enum e_collision_test_flag
{
	_collision_test_structure_bit = 0,
	_collision_test_water_bit,

	_collision_test_unknown2_bit,

	_collision_test_instanced_geometry_bit,
	_collision_test_render_only_bsps_bit,
	_collision_test_ignore_child_objects_bit,
	_collision_test_ignore_nonpathfindable_objects_bit,

	_collision_test_unknown7_bit,

	_collision_test_ignore_cinematic_objects_bit,
	_collision_test_ignore_dead_bipeds_bit,

	_collision_test_unknown10_bit,

	_collision_test_front_facing_surfaces_bit,
	_collision_test_back_facing_surfaces_bit,
	_collision_test_ignore_two_sided_surfaces_bit,
	_collision_test_ignore_invisible_surfaces_bit,
	_collision_test_ignore_breakable_surfaces_bit,
	_collision_test_allow_early_out_bit,
	_collision_test_try_to_keep_location_valid_bit,

	_collision_test_unknown18_bit,
	_collision_test_unknown19_bit,
	_collision_test_unknown20_bit,
	_collision_test_unknown21_bit,

	k_collision_test_flags
};
static_assert(MASK(k_collision_test_flags) == ~0xFFC00000);

enum e_collision_test_objects_flag
{
	_collision_test_objects_bit = 0,
	_collision_test_objects_biped_bit,          // _object_type_biped
	_collision_test_objects_vehicle_bit,        // _object_type_vehicle
	_collision_test_objects_weapon_bit,         // _object_type_weapon
	_collision_test_objects_equipment_bit,      // _object_type_equipment
	_collision_test_objects_arg_device_bit,     // _object_type_arg_device
	_collision_test_objects_terminal_bit,       // _object_type_terminal
	_collision_test_objects_projectile_bit,     // _object_type_projectile
	_collision_test_objects_scenery_bit,        // _object_type_scenery
	_collision_test_objects_machine_bit,        // _object_type_machine
	_collision_test_objects_control_bit,        // _object_type_control
	_collision_test_objects_sound_scenery_bit,  // _object_type_sound_scenery
	_collision_test_objects_crate_bit,          // _object_type_crate
	_collision_test_objects_creature_bit,       // _object_type_creature
	_collision_test_objects_giant_bit,          // _object_type_giant
	_collision_test_objects_effect_scenery_bit, // _object_type_effect_scenery

	k_collision_test_objects_flags
};
static_assert(MASK(k_collision_test_objects_flags) == ~0xFFFF0000);

struct s_collision_test_flags
{
	c_flags<e_collision_test_flag, dword, k_collision_test_flags> collision_flags;
	c_flags<e_collision_test_objects_flag, dword, k_collision_test_objects_flags> object_flags;
};
static_assert(sizeof(s_collision_test_flags) == 0x8);

extern s_collision_test_flags& _collision_test_flags_unknown_471A840;
extern s_collision_test_flags& k_perception_collision_flags;
extern s_collision_test_flags& _collision_test_footstep_effect_flags;
extern s_collision_test_flags& _collision_test_fix_position_flags;
extern s_collision_test_flags& _collision_test_flags_unknown_471A860;
extern s_collision_test_flags& _collision_test_for_line_of_sight_obstruction_flags;
extern s_collision_test_flags& _collision_test_for_damage_flags;
extern s_collision_test_flags& _collision_test_flags_unknown_471A878;
extern s_collision_test_flags& _collision_test_flags_unknown_471A880;
extern s_collision_test_flags& _collision_test_flags_unknown_471A888;
extern s_collision_test_flags& _collision_test_pathfinding_flags;
extern s_collision_test_flags& _collision_test_flags_unknown_471A898;
extern s_collision_test_flags& k_movement_collision_flags;
extern s_collision_test_flags& _collision_test_environment_flags;
extern s_collision_test_flags& _collision_test_flags_unknown_471A8B0;
extern s_collision_test_flags& _collision_test_flags_unknown_471A8B8;
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

