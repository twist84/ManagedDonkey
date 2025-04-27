#include "ai/formations.hpp"

//.text:014C4F90 ; 
//.text:014C4FB0 ; 
//.text:014C4FC0 ; void __cdecl __tls_set_g_formation_data_allocator(void*)
//.text:014C4FF0 ; 
//.text:014C5000 ; 
//.text:014C5040 ; 
//.text:014C5070 ; int16 __cdecl formation_add_member(int32, int16)
//.text:014C5150 ; void __cdecl formation_delete(int32)
//.text:014C5180 ; bool __cdecl formation_fit_position_to_pathfinding(real_point3d*)
//.text:014C5290 ; 
//.text:014C5930 ; bool __cdecl formation_get_reservation_position(int32, real32, real_point3d const*, real_vector3d const*, int16, real_point3d*)
//.text:014C5A80 ; int32 __cdecl formation_new(int32)
//.text:014C5C20 ; int16 __cdecl formation_primitive_generate_positions(int32, int16, real32, real_point3d const*, real_vector3d const*, real_point3d**, int16)
//.text:014C5AC0 ; void __cdecl formation_point_get_position(s_formation_point const*, real32, real_point3d const*, real_vector3d const*, real_point3d*)
//.text:014C6180 ; 
//.text:014C6C20 ; void __cdecl formation_reservation_clear(int32, int16)
//.text:014C6C80 ; bool __cdecl formation_reservation_valid(int32, int16)
//.text:014C6CC0 ; void __cdecl formation_reset(int32, int32)
//.text:014C6D50 ; int16 __cdecl formation_select_primitive(int32, int16)
//.text:014C6EA0 ; int32 __cdecl formation_setup_spawn_formation(int32)

void __cdecl formations_dispose()
{
	INVOKE(0x014C6F00, formations_dispose);
}

void __cdecl formations_dispose_from_old_map()
{
	INVOKE(0x014C6F10, formations_dispose_from_old_map);
}

void __cdecl formations_initialize()
{
	INVOKE(0x014C6F30, formations_initialize);
}

void __cdecl formations_initialize_for_new_map()
{
	INVOKE(0x014C6F70, formations_initialize_for_new_map);
}

void __cdecl formations_update()
{
	INVOKE(0x014C6FD0, formations_update);
}

//.text:014C6FE0 ; 
//.text:014C7010 ; 

