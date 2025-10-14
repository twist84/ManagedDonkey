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

extern int32 __cdecl object_at_marker(int32 parent_object_index, int32 parent_marker_name_id);
extern void __cdecl object_scripting_clear_all_cinematic_function_variables(int32 object_index);
extern void __cdecl object_scripting_clear_cinematic_function_variable(int32 object_index, int32 function_variable_string);
extern void __cdecl object_scripting_dispose();
extern void __cdecl object_scripting_dispose_from_old_map();
extern void __cdecl object_scripting_dynamic_simulation_disable(int32 object_index, bool dynamic_simulation_disabled);
extern int32 __cdecl object_scripting_get_parent(int32 object_index);
extern void __cdecl object_scripting_initialize();
extern void __cdecl object_scripting_initialize_for_new_map();
extern void __cdecl object_scripting_set_collision_damage_armor_scale(int32 object_index, real32 scale);
extern void __cdecl object_scripting_set_deleted_when_deactivated(int32 object_index);
extern void __cdecl object_scripting_set_function_variable(int32 object_index, int32 function_variable_string, real32 value, real32 interpolation_time_in_ticks);
extern void __cdecl object_scripting_set_inertia_tensor_scale(int32 object_index, real32 scale);
extern void __cdecl object_scripting_set_phantom_power(int32 object_index, bool phantom_power_on);
extern void __cdecl object_scripting_set_physics(int32 object_index, bool physics_enabled);
extern void __cdecl object_scripting_set_shield_effect(int32 object_index, real32 shield_value, real32 shield_time);
extern void __cdecl object_scripting_set_velocity2(int32 object_index, real32 i, real32 j, real32 k);
extern void __cdecl object_scripting_set_velocity(int32 object_index, real32 i);
extern void __cdecl object_scripting_wake_physics(int32 object_index);
extern void __cdecl objects_scripting_attach(int32 parent_object_index, int32 parent_marker_name_id, int32 child_object_index, int32 child_marker_name_id);
extern void __cdecl objects_scripting_detach(int32 parent_object_index, int32 child_object_index);
extern void __cdecl objects_scripting_set_scale(int32 object_index, real32 scale, int16 interpolation_hs_tick_count);

extern void object_scripting_copy_player_appearance(int32 object_index, int32 player_num);

