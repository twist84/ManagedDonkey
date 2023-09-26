#pragma once

#include "hs/hs_scenario_definitions.hpp"

struct
{
	struct s_unknown_struct
	{
		byte __data[0x14];
	};

	bool initialized;

	long compiled_source_size;
	char* compiled_source;

	char __padC[0xC];

	char const* error_message;
	long error_offset;
	char error_message_buffer[1024];

	bool __unknown420;
	bool __unknown421;
	bool illegal_block;
	bool illegal_variable_value_set;
	bool __unknown424;

	long some_reference_count;

	long current_script_index;
	long current_global_index;

	s_unknown_struct* __unknown434;
	long* __unknown438; // 1024
	long* __unknown43C; // 256
	short __unknown434_count;

} hs_compile_globals;
static_assert(sizeof(hs_compile_globals) == 0x444);

