#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "physics/character_physics.hpp"
#include "units/units.hpp"

struct _biped_datum
{
	word_flags flags;
	short pathfinding_structure_index;
	long last_pathfinding_attempt_time;
	long pathfinding_surface_index;
	c_sector_ref pathfinding_sector;
	long pathfinding_instanced_geometry_index;
	long pathfinding_object_index;
	dword pathfinding_bsp_reference;
	real_point3d pathfinding_point;
	real_vector3d pathfinding_ground_collision_normal;
	real_point3d pathfinding_ground_collision_point;
	long last_falling_communication_time;
	long bump_object_index;
	char bump_ticks;
	char jump_control_ticks;
	byte stun_ticks;
	char last_known_speed_scale;
	real lean;
	real camera_offset_z;
	real camera_offset_y;
	real camera_height;
	long jump_time;
	long land_time;
	short current_gate_type;
	short current_gate_index;
	long current_gate_start_time;
	real_vector3d max_analog_movement_controller;
	real_vector3d next_analog_movement_controller;
	long analog_movement_controller_age;
	real_vector3d first_person_camera_offset;
	c_character_physics_component physics;
	object_header_block_reference simulation_interpolation;
	object_header_block_reference last_node_matrices_storage;
	real_vector3d root_offset;
	real_vector3d ground_fit_normal;
	real pivot_on_foot_scale;
	real pivot_on_foot_scale_boost;
	real_point3d pivot_point;
	real_vector2d pivot_fixup;
	real_matrix4x3 left_foot_target;
	real_matrix4x3 right_foot_target;
	real_point3d leap_destination;
	real_vector3d leap_destination_normal;
	real_matrix4x3 root_matrix_history[3];
	real landing_recovery_offset;
	real pendulum_scale;
	real_vector3d pendulum_vector;
	real gravity_scale;

	// linked, armor related
	long  customized_area_object_indices[10];
	short customized_area_indices[10];

	long character_definition_index;
	short death_squad_index;
	byte airborne_intentional_ticks;
	char ai_combat_status;
};
static_assert(sizeof(_biped_datum) == 0x2C8);

struct biped_datum
{
	long definition_index;
	_object_datum object;
	_mover_datum mover;
	_unit_datum unit;
	_biped_datum biped;
};
static_assert(sizeof(biped_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_mover_datum) + sizeof(_unit_datum) + sizeof(_biped_datum));

extern bool debug_objects_physics_control_node;
extern bool debug_objects_biped_autoaim_pills;
extern bool debug_objects_ground_plane;
extern bool debug_objects_movement_mode;
extern bool debug_biped_throttle;
extern bool debug_objects_unit_pathfinding_surface;
extern bool debug_objects_pendulum;

extern void __cdecl biped_bumped_object(long biped_index, long object_index, real_vector3d const* old_velocity);
extern void __cdecl biped_get_autoaim_pill(long biped_index, real_point3d* base, real_vector3d* height, real* autoaim_width);
extern void __cdecl biped_get_physics_pill(long biped_index, real_point3d* position, real* height, real* radius);
extern void __cdecl biped_get_sentinel_animation_node_position_and_velocity(long biped_index, real_point3d* position, real_vector3d* velocity);
extern void __cdecl biped_get_sight_position(long biped_index, short estimate_mode, bool offset_camera, real_point3d const* estimated_body_position, real_vector3d const* a5, real_vector3d const* desired_facing_vector, real_vector3d const* desired_gun_offset, real_point3d* camera_position);
extern bool __cdecl biped_in_airborne_state(long biped_index);
extern void __cdecl biped_scripting_ragdoll(long biped_index);
extern void __cdecl biped_render_debug(long biped_index);
extern bool __cdecl biped_update(long biped_index);
extern void __cdecl biped_update_camera(long biped_index);
extern void __cdecl biped_update_jetpack(long biped_index);
extern void __cdecl biped_update_keyframed_rigid_bodies(long biped_index);
extern void __cdecl biped_update_kill_volumes(long biped_index);
extern void __cdecl biped_update_pendulum(long biped_index);
extern void __cdecl biped_update_root_matrix_history(long biped_index);
extern bool __cdecl biped_update_soft_ceilings(long biped_index);
extern bool __cdecl biped_update_stun(long biped_index);
extern bool __cdecl biped_update_with_parent(long biped_index, long parent_index);
extern bool __cdecl biped_update_without_parent(long biped_index);

