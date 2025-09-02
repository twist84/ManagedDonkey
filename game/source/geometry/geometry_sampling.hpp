#pragma once

class c_geometry_sampler
{
public:
	static void __cdecl show_all_air_probes();

public:
	c_geometry_sampler();

	static real32& g_bounce_light_scale;
	static struct s_geometry_sample& m_default_lightprobe_sample;
};

