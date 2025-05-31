#include "creatures/creatures.hpp"

//.text:00BB3D30 ; void __cdecl creature_accelerate(int32, const real_vector3d*)
//.text:00BB3DD0 ; void __cdecl creature_cause_attached_damage(int32)
//.text:00BB3F20 ; bool __cdecl creature_compute_function_value(int32, int32, int32, bool, real32*, bool*, bool*)
//.text:00BB3F90 ; void __cdecl creature_control(int32, const s_creature_control_data*)
//.text:00BB4000 ; void __cdecl creature_control_clear(s_creature_control_data*)
//.text:00BB4050 ; void __cdecl creature_control_verify(const s_creature_control_data*)
//.text:00BB4060 ; void __cdecl creature_damage_aftermath_apply(int32, const s_damage_aftermath_result_data*)
//.text:00BB40B0 ; void __cdecl creature_damage_aftermath_compute(int32, int32, const s_damage_data*, const s_damage_aftermath_result_data*)
//.text:00BB41B0 ; void __cdecl creature_died(int32)
//.text:00BB41F0 ; void __cdecl creature_affected_by_shape_phantom_this_tick(int32)
//.text:00BB4220 ; void __cdecl creature_get_center_of_mass(int32, real_point3d*)
//.text:00BB4260 ; void __cdecl creature_get_facing_vector(int32, real_vector3d*)

void __cdecl creature_get_head_position(int32 creature_index, real_point3d* head_position)
{
	INVOKE(0x00BB4280, creature_get_head_position, creature_index, head_position);
}

//.text:00BB4300 ; bool __cdecl creature_is_busy(int32)
//.text:00BB4340 ; void __cdecl creature_kill(int32)
//.text:00BB4380 ; void __cdecl creature_move(int32)
//.text:00BB4390 ; bool __cdecl creature_new(int32, object_placement_data*, bool*)
//.text:00BB4550 ; void __cdecl creature_render_debug(int32)
//.text:00BB4560 ; void __cdecl creature_set_actively_controlled(int32, bool)
//.text:00BB45C0 ; bool __cdecl creature_try_to_impact(int32)
//.text:00BB4B40 ; bool __cdecl creature_update(int32)
//.text:00BB4D50 ; 
//.text:00BB4D60 ; 
//.text:00BB4D90 ; 

