#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "physics/character_physics.hpp"
#include "units/units.hpp"

struct _biped_datum
{
	uns16 flags;
	int16 pathfinding_structure_index;
	int32 last_pathfinding_attempt_time;
	int32 pathfinding_surface_index;
	c_sector_ref pathfinding_sector;
	int32 pathfinding_instanced_geometry_index;
	int32 pathfinding_object_index;
	uns32 pathfinding_bsp_reference;
	real_point3d pathfinding_point;
	real_vector3d pathfinding_ground_collision_normal;
	real_point3d pathfinding_ground_collision_point;
	int32 last_falling_communication_time;
	int32 bump_object_index;
	int8 bump_ticks;
	int8 jump_control_ticks;
	uns8 stun_ticks;
	int8 last_known_speed_scale;
	real32 lean;
	real32 camera_offset_z;
	real32 camera_offset_y;
	real32 camera_height;
	int32 jump_time;
	int32 land_time;
	int16 current_gate_type;
	int16 current_gate_index;
	int32 current_gate_start_time;
	real_vector3d max_analog_movement_controller;
	real_vector3d next_analog_movement_controller;
	int32 analog_movement_controller_age;
	real_vector3d first_person_camera_offset;
	c_character_physics_component physics;
	object_header_block_reference simulation_interpolation;
	object_header_block_reference last_node_matrices_storage;
	real_vector3d root_offset;
	real_vector3d ground_fit_normal;
	real32 pivot_on_foot_scale;
	real32 pivot_on_foot_scale_boost;
	real_point3d pivot_point;
	real_vector2d pivot_fixup;
	real_matrix4x3 left_foot_target;
	real_matrix4x3 right_foot_target;
	real_point3d leap_destination;
	real_vector3d leap_destination_normal;
	real_matrix4x3 root_matrix_history[3];
	real32 landing_recovery_offset;
	real32 pendulum_scale;
	real_vector3d pendulum_vector;
	real32 gravity_scale;

	// linked, armor related
	int32  customized_area_object_indices[10];
	int16 customized_area_indices[10];

	int32 character_definition_index;
	int16 death_squad_index;
	uns8 airborne_intentional_ticks;
	int8 ai_combat_status;
};
static_assert(sizeof(_biped_datum) == 0x2C8);

struct biped_datum
{
	int32 definition_index;
	_object_datum object;
	_mover_datum mover;
	_unit_datum unit;
	_biped_datum biped;
};
static_assert(sizeof(biped_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_mover_datum) + sizeof(_unit_datum) + sizeof(_biped_datum));

extern bool debug_objects_physics_control_node;
extern bool debug_objects_biped_autoaim_pills;
extern bool debug_objects_ground_plane;
extern bool debug_objects_movement_mode;
extern bool debug_biped_throttle;
extern bool debug_objects_unit_pathfinding_surface;
extern bool debug_objects_pendulum;

extern void __cdecl biped_bumped_object(int32 biped_index, int32 object_index, const real_vector3d* old_velocity);
extern void __cdecl biped_get_autoaim_pill(int32 biped_index, real_point3d* base, real_vector3d* height, real32* autoaim_width);
extern void __cdecl biped_get_physics_pill(int32 biped_index, real_point3d* position, real32* height, real32* radius);
extern void __cdecl biped_get_sentinel_animation_node_position_and_velocity(int32 biped_index, real_point3d* position, real_vector3d* velocity);
extern void __cdecl biped_get_sight_position(int32 biped_index, int16 estimate_mode, bool offset_camera, const real_point3d* estimated_body_position, const real_vector3d* a5, const real_vector3d* desired_facing_vector, const real_vector3d* desired_gun_offset, real_point3d* camera_position);
extern bool __cdecl biped_in_airborne_state(int32 biped_index);
extern void __cdecl biped_scripting_ragdoll(int32 biped_index);
extern void __cdecl biped_render_debug(int32 biped_index);
extern bool __cdecl biped_update(int32 biped_index);
extern void __cdecl biped_update_camera(int32 biped_index);
extern void __cdecl biped_update_jetpack(int32 biped_index);
extern void __cdecl biped_update_keyframed_rigid_bodies(int32 biped_index);
extern void __cdecl biped_update_kill_volumes(int32 biped_index);
extern void __cdecl biped_update_pendulum(int32 biped_index);
extern void __cdecl biped_update_root_matrix_history(int32 biped_index);
extern bool __cdecl biped_update_soft_ceilings(int32 biped_index);
extern bool __cdecl biped_update_stun(int32 biped_index);
extern bool __cdecl biped_update_with_parent(int32 biped_index, int32 parent_index);
extern bool __cdecl biped_update_without_parent(int32 biped_index);

