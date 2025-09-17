#include "rasterizer/rasterizer_profile.hpp"

#include "math/color_math.hpp"
#include "render/views/render_view.hpp"

#include <d3dx9.h>

c_rasterizer_profile_globals g_rasterizer_profile_globals;

uns32 g_rasterizer_profile_pix_colors[k_rasterizer_profile_element_count]
{
	real_argb_color_to_pixel32(global_real_argb_black),     // total
	real_argb_color_to_pixel32(global_real_argb_grey),      // texaccum
	real_argb_color_to_pixel32(global_real_argb_red),       // static lighting
	real_argb_color_to_pixel32(global_real_argb_green),     // transparents
	real_argb_color_to_pixel32(global_real_argb_blue),      // lightmap shadow generate
	real_argb_color_to_pixel32(global_real_argb_yellow),    // lightmap shadow apply
	real_argb_color_to_pixel32(global_real_argb_cyan),      // dynamic light
	real_argb_color_to_pixel32(global_real_argb_magenta),   // dynamic light shadow generate
	real_argb_color_to_pixel32(global_real_argb_pink),      // effects
	real_argb_color_to_pixel32(global_real_argb_lightblue), // widgets
	real_argb_color_to_pixel32(global_real_argb_orange),    // game engine
	real_argb_color_to_pixel32(global_real_argb_purple),    // interface/hud
	real_argb_color_to_pixel32(global_real_argb_aqua),      // occlusions
	real_argb_color_to_pixel32(global_real_argb_darkgreen), // debug
	real_argb_color_to_pixel32(global_real_argb_salmon),    // distortions
	real_argb_color_to_pixel32(global_real_argb_red),       // patchy fog
	0,                                                      // water
};

const char* const k_rasterizer_profile_element_names[k_rasterizer_profile_element_count]
{
	"total",
	"texaccum",
	"static lighting",
	"transparents",
	"lightmap shadow generate",
	"lightmap shadow apply",
	"dynamic light",
	"dynamic light shadow generate",
	"effects",
	"widgets",
	"game engine",
	"interface/hud",
	"occlusions",
	"debug",
	"distortions",
	"patchy fog",
	"water"
};

const char* const k_rasterizer_profile_stall_names[k_rasterizer_profile_stall_count]
{
	"none",
	"primary overrun",
	"secondary overrun",
	"swap throttle",
	"block until idle",
	"block until not busy",
	"block on fence",
	"vertex shader release",
	"pixel shader release",
	"vertex buffer release",
	"vertex buffer lock",
	"index buffer release",
	"index buffer lock",
	"texture release",
	"texture lock",
	"command buffer release",
	"command buffer lock",
	"constant buffer release",
	"constant buffer lock"
};

void rasterizer_profile_initialize()
{
	g_rasterizer_profile_globals.initialize();
}

void rasterizer_profile_dispose()
{
}

void rasterizer_profile_initialize_for_new_map()
{
	g_rasterizer_profile_globals.initialize();
}

void rasterizer_profile_dispose_from_old_map()
{
}

void rasterizer_profile_frame_begin()
{
	g_rasterizer_profile_globals.increment_frame_reference();

	if (g_rasterizer_profile_globals.get_mode())
		*g_rasterizer_profile_globals.get_frame_valid(g_rasterizer_profile_globals.get_current_frame_reference()) = false;

	real32 element_elapsed_milliseconds[k_rasterizer_profile_element_count]{};
	for (int32 i = 0; i < k_rasterizer_profile_element_count; i++)
	{
		e_rasterizer_profile_elements profile_element_index = e_rasterizer_profile_elements(i);

		element_elapsed_milliseconds[profile_element_index] = rasterizer_profile_get_element_elapsed_milliseconds(profile_element_index);
		s_gpu_time_profile* element_profile = g_rasterizer_profile_globals.get_element_profile(profile_element_index, g_rasterizer_profile_globals.get_current_frame_reference());
		element_profile->stop_watch.reset();
		element_profile->stop_watch.stop();
	}

	for (int32 i = 0; i < k_rasterizer_profile_element_count; i++)
	{
		e_rasterizer_profile_stall_elements profile_element_index = e_rasterizer_profile_stall_elements(i);

		*g_rasterizer_profile_globals.get_element_block_time(profile_element_index, 1) = *g_rasterizer_profile_globals.get_element_block_time(profile_element_index, 0);
		*g_rasterizer_profile_globals.get_element_block_time(profile_element_index, 0) = 0.0;
	}

	*g_rasterizer_profile_globals.get_block_time_total(1) = *g_rasterizer_profile_globals.get_block_time_total(0);
	*g_rasterizer_profile_globals.get_block_time_total(0) = 0.0;
}

void rasterizer_profile_frame_end()
{
	//PIXAddNamedCounter(rasterizer_profile_get_element_elapsed_milliseconds(_rasterizer_profile_element_total), "GPU_work_ms");
}

void rasterizer_profile_update()
{
}

void rasterizer_profile_start_element_timer(e_rasterizer_profile_elements profile_element_index)
{
	g_rasterizer_profile_globals.toggle_element_timer(profile_element_index, (uns8)g_rasterizer_profile_globals.get_current_frame_reference(), true);
}

void rasterizer_profile_stop_element_timer(e_rasterizer_profile_elements profile_element_index)
{
	g_rasterizer_profile_globals.toggle_element_timer(profile_element_index, (uns8)g_rasterizer_profile_globals.get_current_frame_reference(), false);
}

void rasterizer_profile_set_mode(e_rasterizer_profile_modes mode)
{
	g_rasterizer_profile_globals.set_mode(mode);
}

real32 rasterizer_profile_get_element_elapsed_milliseconds(e_rasterizer_profile_elements profile_element_index)
{
	s_gpu_time_profile* element_profile = g_rasterizer_profile_globals.get_element_profile(profile_element_index, g_rasterizer_profile_globals.get_last_frame_reference());

	real32 element_elapsed_milliseconds = c_stop_watch::cycles_to_seconds(element_profile->cycles) * 1000.0f;
	if (profile_element_index == _rasterizer_profile_element_total)
	{
		// $TODO d3d device get counter
	}

	return element_elapsed_milliseconds;
}

real32 rasterizer_profile_get_element_elapsed_block_milliseconds(e_rasterizer_profile_stall_elements profile_element_index)
{
	return (real32)*g_rasterizer_profile_globals.get_element_block_time(profile_element_index, 1);
}

real32 rasterizer_profile_get_block_total_time()
{
	return (real32)*g_rasterizer_profile_globals.get_block_time_total(1);
}

const char* rasterizer_profile_get_stall_name(int32 profile_element_index)
{
	if (profile_element_index >= 0 && profile_element_index < k_rasterizer_profile_stall_count)
		return k_rasterizer_profile_stall_names[profile_element_index];

	return "invalid";
}

const char* rasterizer_profile_get_element_name(e_rasterizer_profile_elements profile_element_index)
{
	const char* result = NULL;
	switch (g_rasterizer_profile_globals.get_mode())
	{
	case _rasterizer_profile_mode_1:
	{
		if (profile_element_index >= 0 && profile_element_index < k_rasterizer_profile_element_count)
		{
			result = k_rasterizer_profile_element_names[profile_element_index];
		}
	}
	break;
	case _rasterizer_profile_mode_5:
	{
		if (profile_element_index >= 0 && profile_element_index < k_rasterizer_profile_element_count)
		{
			result = k_rasterizer_profile_stall_names[profile_element_index];
		}
	}
	break;
	default:
	{
		VASSERT(0, "unsupported mode");
	}
	break;
	}

	return NULL;
}

c_rasterizer_profile_globals::c_rasterizer_profile_globals() :
	m_element_profile(),
	m_element_block_time(),
	m_block_time_total(),
	m_frame_valid(),
	m_frame_reference(),
	m_mode()
{
}

c_rasterizer_profile_globals::~c_rasterizer_profile_globals()
{
}

void c_rasterizer_profile_globals::initialize()
{
}

s_gpu_time_profile* c_rasterizer_profile_globals::get_element_profile(e_rasterizer_profile_elements profile_element_index, int32 frame_reference)
{
	ASSERT(profile_element_index >= 0 && profile_element_index < k_rasterizer_profile_element_count);
	ASSERT(frame_reference >= 0 && frame_reference < k_max_cpu_frames_ahead_of_gpu);

	return &g_rasterizer_profile_globals.m_element_profile[profile_element_index][frame_reference];
}

real64* c_rasterizer_profile_globals::get_element_block_time(e_rasterizer_profile_stall_elements profile_element_index, int32 block_buffer_index)
{
	ASSERT(profile_element_index >= 0 && profile_element_index < k_rasterizer_profile_element_count);
	ASSERT(block_buffer_index >= 0 && block_buffer_index < k_maximum_number_of_block_time_buffers);

	return &g_rasterizer_profile_globals.m_element_block_time[profile_element_index][block_buffer_index];
}

real64* c_rasterizer_profile_globals::get_block_time_total(int32 block_buffer_index)
{
	ASSERT(block_buffer_index >= 0 && block_buffer_index < k_maximum_number_of_block_time_buffers);

	return &m_block_time_total[block_buffer_index];
}

bool* c_rasterizer_profile_globals::get_frame_valid(int32 index)
{
	ASSERT(index >= 0 && index < k_max_cpu_frames_ahead_of_gpu);

	return &m_frame_valid[index];
}

int32 c_rasterizer_profile_globals::get_last_frame_reference()
{
	bool last_frame_valid = false;
	int32 last_frame_reference = 0;

	for (int32 frame_reference = m_frame_reference - 1 + k_max_cpu_frames_ahead_of_gpu;
		frame_reference > m_frame_reference - 1;
		frame_reference--)
	{
		last_frame_reference = frame_reference % k_max_cpu_frames_ahead_of_gpu;
		if (m_frame_valid[frame_reference % 4])
		{
			last_frame_valid = true;
			break;
		}
	}

	if (!last_frame_valid)
	{
		event(_event_warning, "couldn't find valid last frame index for gpu profile");
	}

	return last_frame_reference;
}

int32 c_rasterizer_profile_globals::get_current_frame_reference()
{
	return m_frame_reference % k_max_cpu_frames_ahead_of_gpu;
}

void c_rasterizer_profile_globals::increment_frame_reference()
{
	m_frame_reference = (m_frame_reference + 1) % k_max_cpu_frames_ahead_of_gpu;
}

e_rasterizer_profile_modes c_rasterizer_profile_globals::get_mode()
{
	return e_rasterizer_profile_modes();
}

void c_rasterizer_profile_globals::set_mode(e_rasterizer_profile_modes mode)
{
	ASSERT(mode >= 0 && mode < k_rasterizer_profile_mode_count);

	m_mode = mode;
}

void c_rasterizer_profile_globals::toggle_element_timer(e_rasterizer_profile_elements profile_element_index, uns8 frame_reference, bool start)
{
	//if (get_mode())
	//{
	//	ASSERT(profile_element_index >= 0 && profile_element_index < k_rasterizer_profile_element_count);
	//
	//	uns8 start_flag = start ? 0x8000 : 0;
	//
	//	ASSERT(c_rasterizer::get_device() != NULL);
	//	c_rasterizer::get_device()->InsertCallback(
	//		0,
	//		c_rasterizer_profile_globals::frame_time_callback,
	//		((profile_element_index & 0x7FFF | start_flag) << 16) | frame_reference);
	//}
}

void c_rasterizer_profile_globals::frame_time_callback(uns32 packed_value)
{
	bool start_flag = HIWORD(packed_value) & 0x8000;
	e_rasterizer_profile_elements profile_element_index = e_rasterizer_profile_elements(HIWORD(packed_value) & 0x7FFF);
	int32 frame_reference = packed_value & 0x7FFF;

	s_gpu_time_profile* element_profile = g_rasterizer_profile_globals.get_element_profile(profile_element_index, frame_reference);

	if (start_flag)
	{
		element_profile->stop_watch.start();
	}
	else
	{
		element_profile->cycles = element_profile->stop_watch.hold();
		*g_rasterizer_profile_globals.get_frame_valid(frame_reference) = true;
	}
}

//c_profile_render_section_scope::c_profile_render_section_scope(const char*, e_rasterizer_profile_elements)//

void rasterizer_profile_begin_event(e_rasterizer_profile_elements profile_element_index, const wchar_t* name)
{
	if (render_debug_pix_events)
		D3DPERF_BeginEvent(g_rasterizer_profile_pix_colors[profile_element_index], name);
}

void rasterizer_profile_end_event()
{
	if (render_debug_pix_events)
		D3DPERF_EndEvent();
}

c_rasterizer_profile_scope::c_rasterizer_profile_scope(e_rasterizer_profile_elements profile_element_index, const wchar_t* name) :
	m_profile_element_index(profile_element_index)
{
	rasterizer_profile_begin_event(m_profile_element_index, name);
	//rasterizer_profile_stop_element_timer(profile_element_index);
}

c_rasterizer_profile_scope::~c_rasterizer_profile_scope()
{
	//rasterizer_profile_stop_element_timer(m_profile_element_index);
	rasterizer_profile_end_event();
}

