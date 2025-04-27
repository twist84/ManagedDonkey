#include "ai/ai_reference_frame.hpp"

//.text:014516D0 ; real32 __cdecl ai_distance3d(c_ai_point3d const*, c_ai_point3d const*)
//.text:01451710 ; real32 __cdecl ai_distance3d(c_ai_point3d const*, real_point3d const*)
//.text:014517C0 ; real32 __cdecl ai_distance_squared3d(c_ai_point3d const*, c_ai_point3d const*)
//.text:01451890 ; real32 __cdecl ai_distance_squared3d(c_ai_point3d const*, real_point3d const*)
//.text:01451930 ; bool __cdecl ai_point3d_new(real_point3d const*, long, uint32, c_ai_point3d*)
//.text:014519C0 ; bool __cdecl ai_point_from_world_position(short, real_point3d const*, c_ai_point3d*)

real_point3d* __cdecl ai_point_get_position(c_ai_point3d const* ai_point, real_point3d* position)
{
    return INVOKE(0x01451A10, ai_point_get_position, ai_point, position);
}

//.text:01451A60 ; short __cdecl ai_reference_frame_create(long, short)
//.text:01451A70 ; short __cdecl ai_reference_frame_get_by_bsp_reference(long, uint32)
//.text:01451AD0 ; short __cdecl ai_reference_frame_get_by_identifier_and_node_index(c_object_identifier*, short)
//.text:01451B30 ; short __cdecl ai_reference_frame_get_by_node_index(long, short)
//.text:01451B70 ; void __cdecl ai_reference_frame_handle_deleted_object(long)
//.text:01451BD0 ; bool __cdecl ai_reference_frame_inverse_transform_point(short, real_point3d const*, real_point3d*)
//.text:01451CD0 ; bool __cdecl ai_reference_frame_inverse_transform_vector(short, real_vector3d const*, real_vector3d*)
//.text:01451DD0 ; bool __cdecl ai_reference_frame_transform_ai_point(short, c_ai_point3d const*, c_ai_point3d*)
//.text:01451E70 ; bool __cdecl ai_reference_frame_transform_point(short, real_point3d const*, real_point3d*)
//.text:01451F80 ; bool __cdecl ai_reference_frame_transform_vector(short, real_vector3d const*, real_vector3d*)
//.text:01452090 ; bool __cdecl ai_reference_frame_verify(short)
//.text:014520F0 ; bool __cdecl ai_reference_frames_isolated(short, short)

void __cdecl ai_refresh_reference_frames()
{
    INVOKE(0x01452180, ai_refresh_reference_frames);
}

