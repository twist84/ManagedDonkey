#include "motor/mover.hpp"

//.text:00BA65A0 ; void __cdecl mover_delete(long)
//.text:00BA6600 ; void __cdecl mover_fix_transform_from_physics(long, real_matrix4x3*)
//.text:00BA6650 ; void __cdecl mover_fix_transform_to_physics(long, real_matrix4x3*)

//e_motor_program __cdecl mover_get_motor_program(long motor_index)
long __cdecl mover_get_motor_program(long motor_index)
{
	return INVOKE(0x00BA66A0, mover_get_motor_program, motor_index);
}

//.text:00BA66E0 ; c_character_physics_component::e_mode __cdecl mover_get_physics_mode(long)
//.text:00BA6720 ; void __cdecl mover_handle_deleted_object(long, long)
//.text:00BA67A0 ; bool __cdecl mover_has_variable_camera_up(long)
//.text:00BA67F0 ; bool __cdecl mover_is_in_entering_state(long)
//.text:00BA6850 ; void __cdecl mover_move(long)
//.text:00BA68E0 ; bool __cdecl mover_new(long, object_placement_data*, bool*)
//.text:00BA6A60 ; bool __cdecl mover_post_update(long)
//.text:00BA6A70 ; void __cdecl mover_preprocess_node_orientations(long, c_static_flags<255> const*, long, real_orientation*)
//.text:00BA6AD0 ; void __cdecl mover_preprocess_root_node_matrix(long, real_matrix4x3*)
//.text:00BA6B20 ; void __cdecl mover_render_debug(long)
//.text:00BA6B30 ; void __cdecl mover_render_debug_internal(long)
//.text:00BA6B40 ; bool __cdecl mover_scripting_set_program(long, short)
//.text:00BA6BB0 ; bool __cdecl mover_set_motor_program(long, e_motor_program)
//.text:00BA6C00 ; bool __cdecl mover_update(long)

