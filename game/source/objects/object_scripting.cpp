#include "objects/object_scripting.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/thread_local.hpp"

//.text:00B95750 ; 
//.text:00B95770 ; 
//.text:00B95780 ; void __cdecl __tls_set_g_object_scripting_state_allocator(void*)
//.text:00B957A0 ; 
//.text:00B957E0 ; 
//.text:00B95810 ; 

int32 __cdecl object_at_marker(int32 parent_object_index, int32 parent_marker_name_id)
{
	return INVOKE(0x00B95840, object_at_marker, parent_object_index, parent_marker_name_id);
}

void __cdecl object_scripting_clear_all_cinematic_function_variables(int32 object_index)
{
	INVOKE(0x00B95920, object_scripting_clear_all_cinematic_function_variables, object_index);
}

void __cdecl object_scripting_clear_cinematic_function_variable(int32 object_index, int32 function_variable_string)
{
	INVOKE(0x00B95990, object_scripting_clear_cinematic_function_variable, object_index, function_variable_string);
}

void __cdecl object_scripting_dispose()
{
	INVOKE(0x00B959F0, object_scripting_dispose);
}

void __cdecl object_scripting_dispose_from_old_map()
{
	INVOKE(0x00B95A10, object_scripting_dispose_from_old_map);
}

void __cdecl object_scripting_dynamic_simulation_disable(int32 object_index, bool dynamic_simulation_disabled)
{
	INVOKE(0x00B95A30, object_scripting_dynamic_simulation_disable, object_index, dynamic_simulation_disabled);
}

//.text:00B95AA0 ; bool __cdecl object_scripting_get_cinematic_function_variable(int32, int32, real32*)
//.text:00B95B00 ; bool __cdecl object_scripting_get_cinematic_function_variable(int32, int32, real32*, int32*)
//.text:00B95BF0 ; real64 __cdecl object_scripting_get_health(int32) // real32 __cdecl object_scripting_get_health(int32)

int32 __cdecl object_scripting_get_parent(int32 object_index)
{
	return INVOKE(0x00B95C40, object_scripting_get_parent, object_index);
}

//.text:00B95C80 ; real64 __cdecl object_scripting_get_shield(int32) // real32 __cdecl object_scripting_get_shield(int32)

void __cdecl object_scripting_initialize()
{
	INVOKE(0x00B95CD0, object_scripting_initialize);
}

void __cdecl object_scripting_initialize_for_new_map()
{
	INVOKE(0x00B95D30, object_scripting_initialize_for_new_map);
}

//.text:00B95DA0 ; 
//.text:00B95DF0 ; 

void __cdecl object_scripting_set_collision_damage_armor_scale(int32 object_index, real32 scale)
{
	INVOKE(0x00B95E10, object_scripting_set_collision_damage_armor_scale, object_index, scale);
}

void __cdecl object_scripting_set_deleted_when_deactivated(int32 object_index)
{
	INVOKE(0x00B95E90, object_scripting_set_deleted_when_deactivated, object_index);
}

void __cdecl object_scripting_set_function_variable(int32 object_index, int32 function_variable_string, real32 value, real32 interpolation_time_in_ticks)
{
	INVOKE(0x00B95ED0, object_scripting_set_function_variable, object_index, function_variable_string, value, interpolation_time_in_ticks);
}

void __cdecl object_scripting_set_inertia_tensor_scale(int32 object_index, real32 scale)
{
	INVOKE(0x00B96030, object_scripting_set_inertia_tensor_scale, object_index, scale);
}

void __cdecl object_scripting_set_phantom_power(int32 object_index, bool phantom_power_on)
{
	INVOKE(0x00B960D0, object_scripting_set_phantom_power, object_index, phantom_power_on);
}

void __cdecl object_scripting_set_physics(int32 object_index, bool physics_enabled)
{
	INVOKE(0x00B96120, object_scripting_set_physics, object_index, physics_enabled);
}

void __cdecl object_scripting_set_shield_effect(int32 object_index, real32 shield_value, real32 shield_time)
{
	INVOKE(0x00B96170, object_scripting_set_shield_effect, object_index, shield_value, shield_time);
}

void __cdecl object_scripting_set_velocity2(int32 object_index, real32 i, real32 j, real32 k)
{
	INVOKE(0x00B96270, object_scripting_set_velocity2, object_index, i, j, k);
}

void __cdecl object_scripting_set_velocity(int32 object_index, real32 i)
{
	INVOKE(0x00B963A0, object_scripting_set_velocity, object_index, i);
}

void __cdecl object_scripting_wake_physics(int32 object_index)
{
	INVOKE(0x00B964C0, object_scripting_wake_physics, object_index);
}

void __cdecl objects_scripting_attach(int32 parent_object_index, int32 parent_marker_name_id, int32 child_object_index, int32 child_marker_name_id)
{
	INVOKE(0x00B964E0, objects_scripting_attach, parent_object_index, parent_marker_name_id, child_object_index, child_marker_name_id);
}

void __cdecl objects_scripting_detach(int32 parent_object_index, int32 child_object_index)
{
	INVOKE(0x00B96570, objects_scripting_detach, parent_object_index, child_object_index);
}

void __cdecl objects_scripting_set_scale(int32 object_index, real32 scale, int16 interpolation_hs_tick_count)
{
	INVOKE(0x00B965C0, objects_scripting_set_scale, object_index, scale, interpolation_hs_tick_count);
}

//.text:00B96670 ; tls

void object_scripting_copy_player_appearance(int32 object_index, int32 player_num)
{
	if (object_index != NONE)
	{
		int32 index = NONE;

		int32 player_count = 0;
		c_data_iterator<player_datum> player_iterator;
		player_iterator.begin(player_data);
		while (player_iterator.next())
		{
			if (player_count == player_num)
			{
				index = player_iterator.get_index();
			}
			player_count++;
		}

		if (index != NONE)
		{
			player_copy_object_appearance(index, object_index);
		}
		else
		{
			event(_event_warning, "couldn't copy appearance for player #%d (only found %d players)",
				player_num,
				player_count);
		}
	}
}

