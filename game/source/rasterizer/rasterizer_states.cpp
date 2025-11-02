#include "rasterizer/rasterizer_states.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x01910530, real32, g_shadow_generate_slope_depth_bias);// = 1.414f;
REFERENCE_DECLARE(0x01910534, real32, g_shadow_generate_depth_bias);// = 0.003f;
REFERENCE_DECLARE(0x01910538, real32, g_shadow_generate_dynamic_lights_slope_depth_bias);// = 1.414f;
REFERENCE_DECLARE(0x0191053C, real32, g_shadow_generate_dynamic_lights_depth_bias);// = 0.0008f;
REFERENCE_DECLARE(0x01910540, real32, g_shadow_apply_slope_depth_bias);// = -0.1f;
REFERENCE_DECLARE(0x01910544, real32, g_shadow_apply_depth_bias);// = -0.00000020; // -2.0e-7
REFERENCE_DECLARE(0x01910548, real32, g_debug_geometry_depth_bias);// = -0.0001;
REFERENCE_DECLARE(0x05106FA0, bool, g_use_motion_blur_alpha_blend_mode);// = false;

