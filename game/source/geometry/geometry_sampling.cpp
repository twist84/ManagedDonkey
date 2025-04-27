#include "geometry/geometry_sampling.hpp"

#include "cseries/cseries.hpp"

// g_bounce_light_scale scales `s_geometry_sample::__vector1BC` 
// as part of a call to `sh_eval_directional_light` from within the `c_geometry_sampler::sample`
// - halo 3: 1.0f
// - halo reach: 1.5f
// - halo online: 1.5f
REFERENCE_DECLARE(0x018F2FFC, real32, c_geometry_sampler::g_bounce_light_scale);
REFERENCE_DECLARE(0x018F3000, s_geometry_sample, c_geometry_sampler::m_default_lightprobe_sample);

c_geometry_sampler::c_geometry_sampler()
{
	// set `g_bounce_light_scale` to `0.0f` to "fix random colored lighting"
	c_geometry_sampler::g_bounce_light_scale = 0.0f;
}

c_geometry_sampler override_geometry_sampler = {};