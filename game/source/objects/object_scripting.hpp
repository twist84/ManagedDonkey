#pragma once

#include "cseries/cseries.hpp"

struct s_object_scripting_state
{
	struct s_variable // #TODO: find the actual name
	{
		c_string_id name;
		real __unknown4;
		real __unknown8;
		long __unknownC;
		long __unknown10;
	};
	static_assert(sizeof(s_variable) == 0x14);

	s_variable variables[32];
	long object_indices[32];
	bool map_initialized;
};
static_assert(sizeof(s_object_scripting_state) == 0x304);

extern void __cdecl object_scripting_dispose();
extern void __cdecl object_scripting_dispose_from_old_map();
extern void __cdecl object_scripting_initialize();
extern void __cdecl object_scripting_initialize_for_new_map();

