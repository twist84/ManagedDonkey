#include "ai/cl_engine.hpp"

#include "scenario/scenario.hpp"
#include "scenario/scenario_definitions.hpp"

//.text:01442370 ; 
//.text:014423A0 ; 
//.text:014423C0 ; 
//.text:014423D0 ; void __cdecl __tls_set_g_command_script_allocator(void*)
//.text:01442400 ; bool __cdecl actor_allow_vignette(long, long, short, bool*)
//.text:01442490 ; real __cdecl actor_can_fill_role(long, long, short, mission_dialogue_line const*, s_role_set*, short, long*, short*, bool*)
//.text:01442720 ; 
//.text:01442760 ; long __cdecl command_script_new(long, long, long)
//.text:01442910 ; void __cdecl cs_abort_on_alert2(long, long, bool)
//.text:01442950 ; void __cdecl cs_abort_on_alert(long, bool)
//.text:01442990 ; void __cdecl cs_abort_on_combat_status2(long, long, short)
//.text:014429D0 ; void __cdecl cs_abort_on_combat_status(long, short)
//.text:01442A10 ; void __cdecl cs_abort_on_damage2(long, long, bool)
//.text:01442A50 ; void __cdecl cs_abort_on_damage(long, bool)
//.text:01442A90 ; void __cdecl cs_abort_on_vehicle_exit2(long, long, bool)
//.text:01442AD0 ; void __cdecl cs_abort_on_vehicle_exit(long, bool)
//.text:01442B10 ; void __cdecl cs_abort_thread(long)
//.text:01442B50 ; void __cdecl cs_actor_execute_command_script(long, long, short*, c_behavior_state*)
//.text:01442C40 ; void __cdecl cs_actor_handle_command_script_attach(long, long)
//.text:01442C50 ; void __cdecl cs_actor_interrupt_command_script(long, long)
//.text:01442C70 ; void __cdecl cs_actor_update(long)
//.text:01442E70 ; void __cdecl cs_apply_persistent_state(long)
//.text:01443340 ; bool __cdecl cs_atom_executing(long, long, short)
//.text:01443400 ; long __cdecl cs_attach_command_script(long, short)
//.text:01443420 ; long __cdecl cs_attach_command_script(long, short, long)
//.text:01443510 ; bool __cdecl cs_blocked(long)
//.text:01443680 ; bool __cdecl cs_cast(long, long, bool, short, long*, short)
//.text:014439F0 ; bool __cdecl cs_cast_internal(long, bool, short, mission_dialogue_line const**, short, short, s_role_set*, short*, short, long, long*, short*)
//.text:01443BF0 ; void __cdecl cs_command_clear(cs_command*)
//.text:01443C20 ; short __cdecl cs_command_end(long, long, cs_command*, s_atom_control*, long)
//.text:01443C60 ; short __cdecl cs_command_start(long, long, cs_command*, s_atom_control*, long)
//.text:01443D70 ; short __cdecl cs_command_update(long, long, cs_command*, s_atom_control*, long)
//.text:01443DF0 ; short __cdecl cs_control_clear(long, long, cs_command*, s_atom_control*, long)
//.text:01443E20 ; short __cdecl cs_control_query(long, long, cs_command*, s_atom_control*, long)
//.text:01443E30 ; 
//.text:01443E40 ; void __cdecl cs_dispose()
//.text:01443E50 ; void __cdecl cs_dispose_from_old_map()
//.text:01443E70 ; bool __cdecl cs_get_crouch_modifier(long, real*)

cs_point_set* __cdecl cs_get_point_set(long index)
{
	//return INVOKE(0x01443EE0, cs_get_point_set, index);

	return &global_scenario_get()->scripting_data[0].point_sets[index];
}

//.text:01443F10 ; void __cdecl cs_global_script_context_clear()
//.text:01443F30 ; void __cdecl cs_handle_deleted_object(long, long)
//.text:01444060 ; void __cdecl cs_handle_thread_delete(long)
//.text:014440D0 ; short __cdecl cs_helper(long, short, short(__cdecl*)(long, long, cs_command*, s_atom_control*, long), long)
//.text:01444280 ; long __cdecl cs_index_from_thread(long, long, bool)
//.text:01444330 ; void __cdecl cs_initialize()
//.text:01444370 ; cs_command* __cdecl cs_initialize_command(long, short, bool)
//.text:01444400 ; void __cdecl cs_initialize_for_new_map()
//.text:01444420 ; void __cdecl cs_initialize_for_new_structure_bsp(long)
//.text:01444430 ; long __cdecl cs_queue_command_script(long, short)
//.text:01444450 ; long __cdecl cs_queue_command_script(long, short, long)
//.text:014444E0 ; bool __cdecl cs_release(long, long)
//.text:01444540 ; bool __cdecl cs_release(long, long, long)
//.text:01444580 ; void __cdecl cs_remove_all(long)
//.text:01444730 ; void __cdecl cs_remove_command_script(long, long)
//.text:014448A0 ; long __cdecl cs_reserve(long, long, bool, short)
//.text:01444950 ; void __cdecl cs_reset_cast(long)
//.text:014449C0 ; long __cdecl cs_role(long, short)

cs_script_data* __cdecl cs_scenario_get_script_data(struct scenario* scenario)
{
	//return INVOKE(0x01444A50, cs_scenario_get_script_data, scenario);

	return &scenario->scripting_data[0];
}

//.text:01444A60 ; void __cdecl cs_set_cleanup_script(long, short)
//.text:01444AD0 ; void __cdecl cs_setup_global_script_context(long)
//.text:01444B70 ; long __cdecl cs_stack_command_script(long, short)
//.text:01444C00 ; long __cdecl cs_stack_command_script(long, short, long)
//.text:01444C90 ; 
//.text:01444CC0 ; 
//.text:01444CF0 ; bool __cdecl point_ref_get_position_and_orientation(long, real_point3d*, vector3d*, vector3d*)
//.text:01444D80 ; 
//.text:01444DC0 ; bool __cdecl valid_point_ref(long)
//.text:01444E30 ; bool __cdecl valid_point_set(long)

