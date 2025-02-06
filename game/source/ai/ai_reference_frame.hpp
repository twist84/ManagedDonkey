#pragma once

#include "cseries/cseries.hpp"

struct s_ai_reference_frame_definition;

struct c_ai_point3d;
real_point3d* __cdecl ai_point_get_position(c_ai_point3d const* ai_point, real_point3d* position);
extern void __cdecl ai_refresh_reference_frames();

