#pragma once

#include "cseries/cseries.hpp"

extern bool soft_ceilings_disable;

extern void __cdecl scenario_soft_ceilings_render_debug(real_point3d const* point, bool render_soft_ceilings, bool render_soft_kill, bool render_slip_surfaces);

