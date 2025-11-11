#pragma once

#include "ai/sector.hpp"

class c_ai_point3d;

extern c_sector_ref __cdecl sector_from_point(const real_point3d* point, c_ai_point3d* position_ref);

