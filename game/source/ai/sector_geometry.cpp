#include "ai/sector_geometry.hpp"

#include "cseries/cseries.hpp"

//.text:01464EA0 ; c_sector_ref __cdecl collision_surface_get_sector(int16, int32, int32, int32, uns32, const real_point3d*)
//.text:014650F0 ; c_sector_ref __cdecl giant_sector_from_point(const real_point3d*)
//.text:014651C0 ; bool __cdecl pathfinding_test_line(const real_point3d*, c_sector_ref, const real_point3d*, c_sector_ref, const path_error_info*, path_collision_result*)
//.text:014652D0 ; bool __cdecl pathfinding_test_pill2d(bool, const obstacles*, int16, int16, bool, const real_point3d*, c_sector_ref, c_sector_ref, const real_vector3d*, real32, real32, bool, bool, bool, const path_error_info*, pathfinding_test_pill2d_result*)
//.text:01465790 ; bool __cdecl pathfinding_test_ray2d(const real_point3d*, c_sector_ref, c_sector_ref, const vector3d*, real32, uns32, const path_error_info*, path_collision_result*)
//.text:01466030 ; bool __cdecl pathfinding_test_ray2d(const real_point3d*, c_sector_ref, c_sector_ref, const real_vector3d*, real32, path_collision_result*)
//.text:01466060 ; bool __cdecl pathfinding_test_ray2d(const real_point3d*, c_sector_ref, c_sector_ref, const real_vector3d*, real32, const path_error_info*, path_collision_result*)
//.text:014660E0 ; bool __cdecl pathfinding_test_ray2d_helper(pathfinding_data*, sector_link*, c_sector_ref, uns32, int32, const real_point3d*, const real_vector2d*, const real_plane2d*, c_sector_ref*, pathfinding_data**, bool, real32, real32, real_point2d*, real_point2d*, path_collision_result*)
//.text:014662D0 ; bool __cdecl pill2d_verify_collision_helper(const real_point3d*, c_sector_ref, const real_point3d*, c_sector_ref, const path_error_info*)
//.text:014663E0 ; 

c_sector_ref __cdecl sector_from_point(const real_point3d* point, c_ai_point3d* position_ref)
{
	return INVOKE(0x01466420, sector_from_point, point, position_ref);
}

//.text:014664D0 ; c_sector_ref __cdecl sector_from_point(const real_point3d*, const real_vector3d*, c_ai_point3d*, collision_result&)
//.text:01466570 ; class c_sector_ref __cdecl sector_from_point(const real_point3d*, const real_vector3d*, c_ai_point3d*, collision_result&, real32)
//.text:01466600 ; class c_sector_ref __cdecl sector_from_point(const real_point3d*, const real_vector3d*, c_ai_point3d*, collision_result&, real32, real32)
//.text:014666A0 ; bool __cdecl sector_from_point_and_sector(real_point3d*, c_sector_ref, const real_point3d*, c_sector_ref*)
//.text:014667C0 ; c_sector_ref __cdecl sector_from_vector(const real_point3d*, const real_vector3d*, c_ai_point3d*, int32, collision_result&)
//.text:014668C0 ; bool __cdecl vector_plane_intersection(const real_point3d*, const real_vector3d*, const real_plane3d*, real32*)

