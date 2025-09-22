#include "ai/cl_engine.hpp"

#include "scenario/scenario.hpp"
#include "scenario/scenario_definitions.hpp"

//.text:014423A0 ; 
//.text:014423C0 ; 
//.text:014423D0 ; void __cdecl __tls_set_g_command_script_allocator(void*)
//.text:01442400 ; bool __cdecl actor_allow_vignette(int32, int32, int16, bool*)
//.text:01442490 ; real32 __cdecl actor_can_fill_role(int32, int32, int16, const mission_dialogue_line*, s_role_set*, int16, int32*, int16*, bool*)
//.text:01442720 ; 
//.text:01442760 ; int32 __cdecl command_script_new(int32, int32, int32)
//.text:01442910 ; void __cdecl cs_abort_on_alert2(int32, int32, bool)
//.text:01442950 ; void __cdecl cs_abort_on_alert(int32, bool)
//.text:01442990 ; void __cdecl cs_abort_on_combat_status2(int32, int32, int16)
//.text:014429D0 ; void __cdecl cs_abort_on_combat_status(int32, int16)
//.text:01442A10 ; void __cdecl cs_abort_on_damage2(int32, int32, bool)
//.text:01442A50 ; void __cdecl cs_abort_on_damage(int32, bool)
//.text:01442A90 ; void __cdecl cs_abort_on_vehicle_exit2(int32, int32, bool)
//.text:01442AD0 ; void __cdecl cs_abort_on_vehicle_exit(int32, bool)
//.text:01442B10 ; void __cdecl cs_abort_thread(int32)
//.text:01442B50 ; void __cdecl cs_actor_execute_command_script(int32, int32, int16*, c_behavior_state*)
//.text:01442C40 ; void __cdecl cs_actor_handle_command_script_attach(int32, int32)
//.text:01442C50 ; void __cdecl cs_actor_interrupt_command_script(int32, int32)
//.text:01442C70 ; void __cdecl cs_actor_update(int32)
//.text:01442E70 ; void __cdecl cs_apply_persistent_state(int32)
//.text:01443340 ; bool __cdecl cs_atom_executing(int32, int32, int16)
//.text:01443400 ; int32 __cdecl cs_attach_command_script(int32, int16)
//.text:01443420 ; int32 __cdecl cs_attach_command_script(int32, int16, int32)

bool __cdecl cs_blocked(int32 thread_index)
{
	return INVOKE(0x01443510, cs_blocked, thread_index);
}

//.text:01443680 ; bool __cdecl cs_cast(int32, int32, bool, int16, int32*, int16)
//.text:014439F0 ; bool __cdecl cs_cast_internal(int32, bool, int16, const mission_dialogue_line**, int16, int16, s_role_set*, int16*, int16, int32, int32*, int16*)
//.text:01443BF0 ; void __cdecl cs_command_clear(cs_command*)
//.text:01443C20 ; int16 __cdecl cs_command_end(int32, int32, cs_command*, s_atom_control*, int32)
//.text:01443C60 ; int16 __cdecl cs_command_start(int32, int32, cs_command*, s_atom_control*, int32)
//.text:01443D70 ; int16 __cdecl cs_command_update(int32, int32, cs_command*, s_atom_control*, int32)
//.text:01443DF0 ; int16 __cdecl cs_control_clear(int32, int32, cs_command*, s_atom_control*, int32)
//.text:01443E20 ; int16 __cdecl cs_control_query(int32, int32, cs_command*, s_atom_control*, int32)
//.text:01443E30 ; void __cdecl cs_create_for_new_map()

void __cdecl cs_dispose()
{
	INVOKE(0x01443E40, cs_dispose);
}

void __cdecl cs_dispose_from_old_map()
{
	INVOKE(0x01443E50, cs_dispose_from_old_map);
}

//.text:01443E70 ; bool __cdecl cs_get_crouch_modifier(int32, real32*)

cs_point_set* __cdecl cs_get_point_set(int32 index)
{
	//return INVOKE(0x01443EE0, cs_get_point_set, index);

	return &global_scenario_get()->cs_script_data[0].point_sets[index];
}

//.text:01443F10 ; void __cdecl cs_global_script_context_clear()
//.text:01443F30 ; void __cdecl cs_handle_deleted_object(int32, int32)

void __cdecl cs_handle_thread_delete(int32 deleted_thread_index)
{
	INVOKE(0x01444060, cs_handle_thread_delete, deleted_thread_index);
}

//.text:014440D0 ; int16 __cdecl cs_helper(int32, int16, int16(__cdecl*)(int32, int32, cs_command*, s_atom_control*, int32), int32)
//.text:01444280 ; int32 __cdecl cs_index_from_thread(int32, int32, bool)

void __cdecl cs_initialize()
{
	INVOKE(0x01444330, cs_initialize);
}

//.text:01444370 ; cs_command* __cdecl cs_initialize_command(int32, int16, bool)

void __cdecl cs_initialize_for_new_map()
{
	INVOKE(0x01444400, cs_initialize_for_new_map);
}

void __cdecl cs_initialize_for_new_structure_bsp(int32 activating_structure_bsp_mask)
{
	INVOKE(0x01444420, cs_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:01444430 ; int32 __cdecl cs_queue_command_script(int32, int16)
//.text:01444450 ; int32 __cdecl cs_queue_command_script(int32, int16, int32)
//.text:014444E0 ; bool __cdecl cs_release(int32, int32)
//.text:01444540 ; bool __cdecl cs_release(int32, int32, int32)
//.text:01444580 ; void __cdecl cs_remove_all(int32)
//.text:01444730 ; void __cdecl cs_remove_command_script(int32, int32)
//.text:014448A0 ; int32 __cdecl cs_reserve(int32, int32, bool, int16)
//.text:01444950 ; void __cdecl cs_reset_cast(int32)
//.text:014449C0 ; int32 __cdecl cs_role(int32, int16)

cs_script_data* __cdecl cs_scenario_get_script_data(struct scenario* scenario)
{
	//return INVOKE(0x01444A50, cs_scenario_get_script_data, scenario);

	return &scenario->cs_script_data[0];
}

//.text:01444A60 ; void __cdecl cs_set_cleanup_script(int32, int16)

void __cdecl cs_setup_global_script_context(int32 thread_index)
{
	INVOKE(0x01444AD0, cs_setup_global_script_context, thread_index);
}

//.text:01444B70 ; int32 __cdecl cs_stack_command_script(int32, int16)
//.text:01444C00 ; int32 __cdecl cs_stack_command_script(int32, int16, int32)
//.text:01444C90 ; 
//.text:01444CC0 ; 
//.text:01444CF0 ; bool __cdecl point_ref_get_position_and_orientation(int32, real_point3d*, real_vector3d*, real_vector3d*)
//.text:01444D80 ; 
//.text:01444DC0 ; bool __cdecl valid_point_ref(int32)
//.text:01444E30 ; bool __cdecl valid_point_set(int32)

