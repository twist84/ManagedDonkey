#pragma once

#include "cseries/cseries.hpp"

struct c_allocation_base;
struct s_memory_pool
{
	tag signature;
	char name[32];
	c_allocation_base* allocator;
	long size;
	long free_size;
	dword offset_to_data;
	dword first_block_handle;
	dword last_block_handle;
	bool reference_tracking;
	bool allocation_from_anywhere_in_pool;
	bool verification_enabled;
	bool __unknown3F_bool;
	long callback_list;

	// block?
	byte __data[0x10];
};
static_assert(sizeof(s_memory_pool) == 0x44 + 0x10);
