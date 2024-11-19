#pragma once

#include "objects/objects.hpp"

struct _mover_datum
{
	object_header_block_reference motor_tasks;
	object_header_block_reference motor_state;
	object_header_block_reference action_state_storage;
};
static_assert(sizeof(_mover_datum) == 0xC);

struct mover_datum
{
	long definition_index;
	_object_datum object;
	_mover_datum mover;
};
static_assert(sizeof(mover_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_mover_datum));

extern long __cdecl mover_get_motor_program(long motor_index);

