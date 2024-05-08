#pragma once

#include "objects/objects.hpp"

struct motor_datum
{
	object_datum object;
	object_header_block_reference motor_tasks;
	object_header_block_reference motor_state;
	object_header_block_reference action_state_storage;
};
static_assert(sizeof(motor_datum) == sizeof(object_datum) + 0xC);

