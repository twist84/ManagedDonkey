#include "physics/point_physics.hpp"

void __cdecl point_physics_dispose()
{
	INVOKE(0x00695C70, point_physics_dispose);
}

void __cdecl point_physics_dispose_from_old_map()
{
	INVOKE(0x00695C80, point_physics_dispose_from_old_map);
}

void __cdecl point_physics_initialize()
{
	INVOKE(0x00695C90, point_physics_initialize);
}

void __cdecl point_physics_initialize_for_new_map()
{
	INVOKE(0x00695CA0, point_physics_initialize_for_new_map);
}

//.text:00695D10 ; uns32 __cdecl point_physics_update(int32, const point_physics_definition*, s_location*, int16, real_point3d*, real_vector3d*, const real_vector3d*, real_vector3d*, c_global_material_type*, real32, real32)

