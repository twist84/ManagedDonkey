#include "motor/mover.hpp"

//.text:00BA65A0 ; void __cdecl mover_delete(int32)
//.text:00BA6600 ; void __cdecl mover_fix_transform_from_physics(int32, real_matrix4x3*)
//.text:00BA6650 ; void __cdecl mover_fix_transform_to_physics(int32, real_matrix4x3*)

//e_motor_program __cdecl mover_get_motor_program(int32 motor_index)
int32 __cdecl mover_get_motor_program(int32 motor_index)
{
	return INVOKE(0x00BA66A0, mover_get_motor_program, motor_index);
}

//.text:00BA66E0 ; c_character_physics_component::e_mode __cdecl mover_get_physics_mode(int32)
//.text:00BA6720 ; void __cdecl mover_handle_deleted_object(int32, int32)
//.text:00BA67A0 ; bool __cdecl mover_has_variable_camera_up(int32)
//.text:00BA67F0 ; bool __cdecl mover_is_in_entering_state(int32)
//.text:00BA6850 ; void __cdecl mover_move(int32)
//.text:00BA68E0 ; bool __cdecl mover_new(int32, object_placement_data*, bool*)
//.text:00BA6A60 ; bool __cdecl mover_post_update(int32)
//.text:00BA6A70 ; void __cdecl mover_preprocess_node_orientations(int32, c_static_flags<255> const*, int32, real_orientation*)
//.text:00BA6AD0 ; void __cdecl mover_preprocess_root_node_matrix(int32, real_matrix4x3*)
//.text:00BA6B20 ; void __cdecl mover_render_debug(int32)
//.text:00BA6B30 ; void __cdecl mover_render_debug_internal(int32)
//.text:00BA6B40 ; bool __cdecl mover_scripting_set_program(int32, int16)
//.text:00BA6BB0 ; bool __cdecl mover_set_motor_program(int32, e_motor_program)
//.text:00BA6C00 ; bool __cdecl mover_update(int32)

