#include "objects/object_scripting.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/thread_local.hpp"

//.text:00B95750 ; 
//.text:00B95770 ; 
//.text:00B95780 ; void __cdecl __tls_set_g_object_scripting_state_allocator(void*)
//.text:00B957A0 ; 
//.text:00B957E0 ; 
//.text:00B95810 ; 
//.text:00B95840 ; int32 __cdecl object_at_marker(int32, int32)
//.text:00B95920 ; void __cdecl object_scripting_clear_all_cinematic_function_variables(int32)
//.text:00B95990 ; void __cdecl object_scripting_clear_cinematic_function_variable(int32, int32)

void __cdecl object_scripting_dispose()
{
	INVOKE(0x00B959F0, object_scripting_dispose);
}

void __cdecl object_scripting_dispose_from_old_map()
{
	INVOKE(0x00B95A10, object_scripting_dispose_from_old_map);
}

//.text:00B95A30 ; void __cdecl object_scripting_dynamic_simulation_disable(int32, bool)
//.text:00B95AA0 ; bool __cdecl object_scripting_get_cinematic_function_variable(int32, int32, real32*)
//.text:00B95B00 ; bool __cdecl object_scripting_get_cinematic_function_variable(int32, int32, real32*, int32*)
//.text:00B95BF0 ; real64 __cdecl object_scripting_get_health(int32) // real32 __cdecl object_scripting_get_health(int32)
//.text:00B95C40 ; int32 __cdecl object_scripting_get_parent(int32)
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
//.text:00B95E10 ; function sets `object->object.brittle_collision_damage_timer`
//.text:00B95E90 ; void __cdecl object_scripting_set_deleted_when_deactivated(int32)
//.text:00B95ED0 ; void __cdecl object_scripting_set_function_variable(int32, int32, real32, real32)
//.text:00B96030 ; function sets `object->object.environment_interpenetration_ticks`
//.text:00B960D0 ; void __cdecl object_scripting_set_phantom_power(int32, bool)
//.text:00B96120 ; void __cdecl object_scripting_set_physics(int32, bool)
//.text:00B96170 ; void __cdecl object_scripting_set_shield_effect(int32, real32, real32)
//.text:00B96270 ; void __cdecl object_scripting_set_velocity2(int32, real32, real32, real32)
//.text:00B963A0 ; void __cdecl object_scripting_set_velocity(int32, real32)
//.text:00B964C0 ; void __cdecl object_scripting_wake_physics(int32)
//.text:00B964E0 ; void __cdecl objects_scripting_attach(int32, int32, int32, int32)
//.text:00B96570 ; void __cdecl objects_scripting_detach(int32, int32)
//.text:00B965C0 ; void __cdecl objects_scripting_set_scale(int32, real32, int16)
//.text:00B96670 ; 

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

