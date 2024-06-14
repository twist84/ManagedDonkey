#pragma once

#include "objects/objects.hpp"

struct motor_data
{
	object_header_block_reference motor_tasks;
	object_header_block_reference motor_state;
	object_header_block_reference action_state_storage;
};
static_assert(sizeof(motor_data) == 0xC);

struct motor_datum
{
	object_data object;
	motor_data motor;
};
static_assert(sizeof(motor_datum) == sizeof(object_datum) + sizeof(motor_data));

