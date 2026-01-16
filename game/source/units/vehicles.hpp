#pragma once

#include "ai/sector.hpp"
#include "units/units.hpp"
#include "units/vehicle_definitions.hpp"
#include "units/vehicle_type_component.hpp"

#define MAXIMUM_NUMBER_OF_SUSPENSION_ANIMATIONS 32

struct _vehicle_datum
{
	uns16 flags;
	uns8 upside_down_on_ground_ticks;
	uns8 airborne_ticks;
	uns8 upending_type;
	uns8 upending_ticks;
	uns8 on_ground_ticks;
	byte pad7[0x5];
	uns16 slipping_wheels_bit_vector;
	real32 mean_antigrav_fraction;
	c_vehicle_type_component type_component;
	uns8 suspension_last_measurement[MAXIMUM_NUMBER_OF_SUSPENSION_ANIMATIONS];
	uns8 suspension[MAXIMUM_NUMBER_OF_SUSPENSION_ANIMATIONS];
	int32 last_physics_cache_update_time;
	int32 squad_index;
	int32 next_vehicle_index;
	int32 spawn_point_name;
	int32 bipeds_killed;
	uns32 disallowed_seats[2];
	int32 reserved_seats[128];
	int32 pathfinding_time;
	int16 pathfinding_structure_index;
	int32 pathfinding_surface_index;
	c_sector_ref pathfinding_sector;
	int32 pathfinding_instanced_geometry_index;
	int32 pathfinding_object_index;
	uns32 pathfinding_bsp_reference;
	real_point3d pathfinding_point;
	int32 last_known_animation_graph_index;
	c_animation_id steering_animation_id;
	c_animation_id roll_animation_id;
	c_animation_id throttle_animation_id;
	c_animation_id velocity_animation_id;
	c_animation_id acceleration_animation_id[3];
	real32 driver_force_position;
	real32 driver_force_scale;
	real_vector3d auto_aiming_vector;
	real_point3d last_soft_ceiling_update_position;
	object_header_block_reference vehicle_physics_cache;
};
COMPILE_ASSERT(sizeof(_vehicle_datum) == 0x340);
COMPILE_ASSERT(0x7 == OFFSETOF(_vehicle_datum, pad7));

struct vehicle_datum
{
	int32 definition_index;
	_object_datum object;
	_mover_datum mover;
	_unit_datum unit;
	_vehicle_datum vehicle;
};
COMPILE_ASSERT(sizeof(vehicle_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_mover_datum) + sizeof(_unit_datum) + sizeof(_vehicle_datum));

extern bool __cdecl vehicle_about_to_detonate_near_any_player(int32* out_vehicle_index);
extern bool __cdecl vehicle_can_be_teleported(int32 vehicle_index);
extern void __cdecl vehicle_get_seat_position(int32 vehicle_index, int16 seat_index, real_point3d* seat_position);
extern e_vehicle_type __cdecl vehicle_get_type(int32 vehicle_index);
extern bool __cdecl vehicle_moving_near_any_player(int32* out_vehicle_index);
extern void __cdecl vehicle_render_debug(int32 vehicle_index);

