#include "ai/ai_reference_frame.hpp"

//.text:014516D0 ; real32 __cdecl ai_distance3d(const c_ai_point3d*, const c_ai_point3d*)
//.text:01451710 ; real32 __cdecl ai_distance3d(const c_ai_point3d*, const real_point3d*)
//.text:014517C0 ; real32 __cdecl ai_distance_squared3d(const c_ai_point3d*, const c_ai_point3d*)
//.text:01451890 ; real32 __cdecl ai_distance_squared3d(const c_ai_point3d*, const real_point3d*)
//.text:01451930 ; bool __cdecl ai_point3d_new(const real_point3d*, int32, uns32, c_ai_point3d*)
//.text:014519C0 ; bool __cdecl ai_point_from_world_position(int16, const real_point3d*, c_ai_point3d*)

real_point3d* __cdecl ai_point_get_position(const c_ai_point3d* ai_point, real_point3d* position)
{
    return INVOKE(0x01451A10, ai_point_get_position, ai_point, position);
}

//.text:01451A60 ; int16 __cdecl ai_reference_frame_create(int32, int16)
//.text:01451A70 ; int16 __cdecl ai_reference_frame_get_by_bsp_reference(int32, uns32)
//.text:01451AD0 ; int16 __cdecl ai_reference_frame_get_by_identifier_and_node_index(c_object_identifier*, int16)
//.text:01451B30 ; int16 __cdecl ai_reference_frame_get_by_node_index(int32, int16)
//.text:01451B70 ; void __cdecl ai_reference_frame_handle_deleted_object(int32)
//.text:01451BD0 ; bool __cdecl ai_reference_frame_inverse_transform_point(int16, const real_point3d*, real_point3d*)
//.text:01451CD0 ; bool __cdecl ai_reference_frame_inverse_transform_vector(int16, const real_vector3d*, real_vector3d*)
//.text:01451DD0 ; bool __cdecl ai_reference_frame_transform_ai_point(int16, const c_ai_point3d*, c_ai_point3d*)
//.text:01451E70 ; bool __cdecl ai_reference_frame_transform_point(int16, const real_point3d*, real_point3d*)
//.text:01451F80 ; bool __cdecl ai_reference_frame_transform_vector(int16, const real_vector3d*, real_vector3d*)
//.text:01452090 ; bool __cdecl ai_reference_frame_verify(int16)
//.text:014520F0 ; bool __cdecl ai_reference_frames_isolated(int16, int16)

void __cdecl ai_refresh_reference_frames()
{
    INVOKE(0x01452180, ai_refresh_reference_frames);
}

