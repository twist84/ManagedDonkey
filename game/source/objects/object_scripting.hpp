#pragma once

#include "cseries/cseries.hpp"

struct s_object_scripting_state
{
	enum
	{
		k_maximum_varaibles = 32
	};

	struct s_variable
	{
		c_string_id function_variable_string;
		real32 old_value;
		real32 new_value;
		int32 new_value_set_time;
		int32 interpolation_time;
	};
	static_assert(sizeof(s_variable) == 0x14);

	s_variable variables[k_maximum_varaibles];
	int32 variable_owner_indexes[k_maximum_varaibles];
	bool map_initialized;
};
static_assert(sizeof(s_object_scripting_state) == 0x304);

extern void __cdecl object_scripting_dispose();
extern void __cdecl object_scripting_dispose_from_old_map();
extern void __cdecl object_scripting_initialize();
extern void __cdecl object_scripting_initialize_for_new_map();

extern void object_scripting_copy_player_appearance(int32 object_index, int32 player_num);

