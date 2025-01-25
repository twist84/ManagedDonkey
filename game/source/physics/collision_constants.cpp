#include "physics/collision_constants.hpp"

REFERENCE_DECLARE(0x0471A840, s_collision_test_flags, k_trivial_obstruction_collision_flags);
REFERENCE_DECLARE(0x0471A848, s_collision_test_flags, k_perception_collision_flags);
REFERENCE_DECLARE(0x0471A850, s_collision_test_flags, _collision_test_footstep_effect_flags);
REFERENCE_DECLARE(0x0471A858, s_collision_test_flags, _collision_test_fix_position_flags);
REFERENCE_DECLARE(0x0471A860, s_collision_test_flags, _collision_test_for_sound_transmission_flags);
REFERENCE_DECLARE(0x0471A868, s_collision_test_flags, _collision_test_for_line_of_sight_obstruction_flags);
REFERENCE_DECLARE(0x0471A870, s_collision_test_flags, _collision_test_for_damage_flags);
REFERENCE_DECLARE(0x0471A878, s_collision_test_flags, _collision_test_for_editor_flags);
REFERENCE_DECLARE(0x0471A880, s_collision_test_flags, _collision_test_pathfinding_structure_only_flags);
REFERENCE_DECLARE(0x0471A888, s_collision_test_flags, k_communication_collision_flags);
REFERENCE_DECLARE(0x0471A890, s_collision_test_flags, _collision_test_pathfinding_flags);
REFERENCE_DECLARE(0x0471A898, s_collision_test_flags, _collision_test_ragdoll_bring_inside_world_flags);
REFERENCE_DECLARE(0x0471A8A0, s_collision_test_flags, k_movement_collision_flags);
REFERENCE_DECLARE(0x0471A8A8, s_collision_test_flags, _collision_test_environment_flags);
REFERENCE_DECLARE(0x0471A8B0, s_collision_test_flags, _collision_test_for_biped_sight_position_offset_flags);
REFERENCE_DECLARE(0x0471A8B8, s_collision_test_flags, _collision_test_for_projectiles_fat_flags);
REFERENCE_DECLARE(0x0471A8C0, s_collision_test_flags, _collision_test_for_melee_obstruction_flags);
REFERENCE_DECLARE(0x0471A8C8, s_collision_test_flags, _collision_test_objects_all_types_flags);
REFERENCE_DECLARE(0x0471A8D0, s_collision_test_flags, _collision_test_for_line_of_sight_flags);
REFERENCE_DECLARE(0x0471A8D8, s_collision_test_flags, _collision_test_objects_sight_blocking_flags);
REFERENCE_DECLARE(0x0471A8E0, s_collision_test_flags, _collision_test_for_unit_access_obstruction_flags);
REFERENCE_DECLARE(0x0471A8E8, s_collision_test_flags, _collision_test_for_vehicles_flags);
REFERENCE_DECLARE(0x0471A8F0, s_collision_test_flags, k_perception_early_out_collision_flags);
REFERENCE_DECLARE(0x0471A8F8, s_collision_test_flags, _collision_test_for_projectiles_flags);
REFERENCE_DECLARE(0x0471A900, s_collision_test_flags, _collision_test_for_damage_obstruction_flags);
REFERENCE_DECLARE(0x0471A908, s_collision_test_flags, _collision_test_unit_enter_seat_flags);
REFERENCE_DECLARE(0x0471A910, s_collision_test_flags, _collision_test_empty_flags);
REFERENCE_DECLARE(0x0471A918, s_collision_test_flags, _collision_test_structure_geometry_flags);

//s_collision_test_flags collision_test_flags_build(dword, dword);
//s_collision_test_flags collision_test_flags_or(s_collision_test_flags, s_collision_test_flags);
//s_collision_test_flags collision_test_flags_and(s_collision_test_flags, s_collision_test_flags);
//s_collision_test_flags collision_test_flags_invert(s_collision_test_flags, s_collision_test_flags);

//.text:015A1690 ; void `dynamic initializer for '_collision_test_empty_flags''(void)
//.text:015A16B0 ; void `dynamic initializer for '_collision_test_environment_flags''(void)
//.text:015A16D0 ; void `dynamic initializer for '_collision_test_fix_position_flags''(void)
//.text:015A16F0 ; void `dynamic initializer for '_collision_test_footstep_effect_flags''(void)
//.text:015A1710 ; void `dynamic initializer for '_collision_test_for_biped_sight_position_offset_flags''(void)
//.text:015A1740 ; void `dynamic initializer for '_collision_test_for_damage_flags''(void)
//.text:015A1760 ; void `dynamic initializer for '_collision_test_for_damage_obstruction_flags''(void)
//.text:015A1780 ; void `dynamic initializer for '_collision_test_for_editor_flags''(void)
//.text:015A17B0 ; void `dynamic initializer for '_collision_test_for_line_of_sight_flags''(void)
//.text:015A17E0 ; void `dynamic initializer for '_collision_test_for_line_of_sight_obstruction_flags''(void)
//.text:015A1800 ; void `dynamic initializer for '_collision_test_for_melee_obstruction_flags''(void)
//.text:015A1820 ; void `dynamic initializer for '_collision_test_for_projectiles_fat_flags''(void)
//.text:015A1840 ; void `dynamic initializer for '_collision_test_for_projectiles_flags''(void)
//.text:015A1860 ; void `dynamic initializer for '_collision_test_for_sound_transmission_flags''(void)
//.text:015A1880 ; void `dynamic initializer for '_collision_test_for_unit_access_obstruction_flags''(void)
//.text:015A18B0 ; void `dynamic initializer for '_collision_test_for_vehicles_flags''(void)
//.text:015A18D0 ; void `dynamic initializer for '_collision_test_objects_all_types_flags''(void)
//.text:015A18F0 ; void `dynamic initializer for '_collision_test_objects_sight_blocking_flags''(void)
//.text:015A1910 ; void `dynamic initializer for '_collision_test_pathfinding_flags''(void)
//.text:015A1930 ; void `dynamic initializer for '_collision_test_pathfinding_structure_only_flags''(void)
//.text:015A1950 ; void `dynamic initializer for '_collision_test_ragdoll_bring_inside_world_flags''(void)
//.text:015A1980 ; void `dynamic initializer for '_collision_test_structure_geometry_flags''(void)
//.text:015A19A0 ; void `dynamic initializer for '_collision_test_unit_enter_seat_flags''(void)
//.text:015A19C0 ; void `dynamic initializer for 'k_communication_collision_flags''(void)
//.text:015A19E0 ; void `dynamic initializer for 'k_movement_collision_flags''(void)
//.text:015A1A00 ; void `dynamic initializer for 'k_perception_collision_flags''(void)
//.text:015A1A20 ; void `dynamic initializer for 'k_perception_early_out_collision_flags''(void)
//.text:015A1A40 ; void `dynamic initializer for 'k_trivial_obstruction_collision_flags''(void)

