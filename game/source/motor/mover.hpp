#pragma once

#include "objects/objects.hpp"

struct _motor_datum
{
	object_header_block_reference motor_tasks;
	object_header_block_reference motor_state;
	object_header_block_reference action_state_storage;
};
static_assert(sizeof(_motor_datum) == 0xC);

struct motor_datum
{
	_object_datum object;
	_motor_datum motor;
};
static_assert(sizeof(motor_datum) == sizeof(_object_datum) + sizeof(_motor_datum));

