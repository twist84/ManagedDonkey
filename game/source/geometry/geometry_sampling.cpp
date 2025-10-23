#include "geometry/geometry_sampling.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "scenario/scenario.hpp"

// `c_geometry_sampler::g_bounce_light_scale` scales `s_geometry_sample::m_diffuse` 
// as part of a call to `sh_eval_directional_light` from within the `c_geometry_sampler::sample`
// - halo 3: 1.0f
// - halo reach: 1.5f
// - halo online: 1.5f
REFERENCE_DECLARE(0x018F2FFC, real32, c_geometry_sampler::g_bounce_light_scale) = 0.0f;
REFERENCE_DECLARE(0x018F3000, s_geometry_sample, c_geometry_sampler::m_default_lightprobe_sample);

// Halo Online uses the index for the name
//#define USE_INDEX

void __cdecl c_geometry_sampler::show_all_air_probes()
{
	//INVOKE(0x00754900, c_geometry_sampler::show_all_air_probes);

	struct scenario* scenario = global_scenario_get();
	if (scenario->airprobes.count)
	{
		real_matrix4x3 camera{};
		c_player_view::get_player_render_camera_orientation(&camera);
		for (int32 airprobe_index = 0; airprobe_index < scenario->airprobes.count; ++airprobe_index)
		{
			s_scenario_airprobe_info* airprobe = TAG_BLOCK_GET_ELEMENT(&scenario->airprobes, airprobe_index, s_scenario_airprobe_info);
			real32 distance = distance3d(&camera.position, &airprobe->position);
			if (distance < 50.0f)
			{
#if defined(USE_INDEX)
				char probe_name[256]{};
				csnzprintf(probe_name, sizeof(probe_name), "ap%d", airprobe_index);
#else
				const char* probe_name = string_id_get_string_const(airprobe->name);

				// if `probe_name` is null fallback to index for the name
				if (!probe_name)
				{
					char name_from_index[256]{};
					probe_name = csnzprintf(name_from_index, sizeof(name_from_index), "ap%d", airprobe_index);
				}
#endif

				char probe_info[256]{};
				csnzprintf(probe_info, sizeof(probe_info), "%s(%4.2f)", probe_name, distance);
				render_debug_string_at_point(&airprobe->position, probe_info, global_real_argb_aqua);
				render_debug_sphere(true, &airprobe->position, 0.5f, global_real_argb_aqua);
			}
		}
	}
}

