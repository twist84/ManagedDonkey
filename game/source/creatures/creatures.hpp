#pragma once

#include "cseries/cseries.hpp"
#include "motor/mover.hpp"
#include "objects/objects.hpp"
#include "physics/character_physics.hpp"

struct s_creature_control_data
{
	int32 animation_mode;
	int32 animation_state;
	uns16 control_flags;
	int8 turning_speed;
	byte pad;
	real_vector3d throttle;
	real_vector3d forward;
	real_vector3d up;
};
COMPILE_ASSERT(sizeof(s_creature_control_data) == 0x30);

struct _creature_datum
{
	uns16 flags;
	int16 team_index;
	int32 ai_state_type;
	object_header_block_reference ai_state_storage;
	s_creature_control_data control_data;
	real_vector3d turning_velocity;
	real32 bank;
	c_character_physics_component physics;
	int8 attached_damage_timer;
	int8 attached_cooldown_timer;
	int16 destroy_after_death_timer;
	int16 lifetime_ticks;
};
COMPILE_ASSERT(sizeof(_creature_datum) == 0xD4);

struct creature_datum
{
	int32 definition_index;
	_object_datum object;
	_mover_datum mover;
	_creature_datum creature;
};
COMPILE_ASSERT(sizeof(creature_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_mover_datum) + sizeof(_creature_datum));

extern void __cdecl creature_get_head_position(int32 creature_index, real_point3d* head_position);

