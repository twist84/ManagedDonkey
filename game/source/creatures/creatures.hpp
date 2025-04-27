#pragma once

#include "cseries/cseries.hpp"
#include "motor/mover.hpp"
#include "objects/objects.hpp"
#include "physics/character_physics.hpp"

struct s_creature_control_data
{
	long animation_mode;
	long animation_state;
	uint16 control_flags;
	char turning_speed;
	uint8 pad;
	real_vector3d throttle;
	real_vector3d forward;
	real_vector3d up;
};
static_assert(sizeof(s_creature_control_data) == 0x30);

struct _creature_datum
{
	uint16 flags;
	short team_index;
	long ai_state_type;
	object_header_block_reference ai_state_storage;
	s_creature_control_data control_data;
	real_vector3d turning_velocity;
	real32 bank;
	c_character_physics_component physics;
	char attached_damage_timer;
	char attached_cooldown_timer;
	short destroy_after_death_timer;
	short lifetime_ticks;
};
static_assert(sizeof(_creature_datum) == 0xD4);

struct creature_datum
{
	long definition_index;
	_object_datum object;
	_mover_datum mover;
	_creature_datum creature;
};
static_assert(sizeof(creature_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_mover_datum) + sizeof(_creature_datum));

extern void __cdecl creature_get_head_position(long creature_index, real_point3d* head_position);

