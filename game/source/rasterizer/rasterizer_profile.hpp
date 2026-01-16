#pragma once

#include "profiler/profiler_stopwatch.hpp"

enum e_rasterizer_profile_modes
{
	_rasterizer_profile_mode_0,
	_rasterizer_profile_mode_1,
	_rasterizer_profile_mode_2,
	_rasterizer_profile_mode_3,
	_rasterizer_profile_mode_4,
	_rasterizer_profile_mode_5,


	k_rasterizer_profile_mode_count
};

enum e_rasterizer_profile_elements
{
	_rasterizer_profile_element_total = 0,
	_rasterizer_profile_element_texaccum,
	_rasterizer_profile_element_static_lighting,
	_rasterizer_profile_element_transparents,
	_rasterizer_profile_element_lightmap_shadow_generate,
	_rasterizer_profile_element_lightmap_shadow_apply,
	_rasterizer_profile_element_dynamic_light,
	_rasterizer_profile_element_dynamic_light_shadow_generate,
	_rasterizer_profile_element_effects,
	_rasterizer_profile_element_widgets,
	_rasterizer_profile_element_game_engine,
	_rasterizer_profile_element_interface_hud,
	_rasterizer_profile_element_occlusions,
	_rasterizer_profile_element_debug,
	_rasterizer_profile_element_distortions,
	_rasterizer_profile_element_patchy_fog,
	_rasterizer_profile_element_water,

	k_rasterizer_profile_element_count
};

enum e_rasterizer_profile_stall_elements
{
	_rasterizer_profile_stall_none = 0,
	_rasterizer_profile_stall_primary_overrun,
	_rasterizer_profile_stall_secondary_overrun,
	_rasterizer_profile_stall_swap_throttle,
	_rasterizer_profile_stall_block_until_idle,
	_rasterizer_profile_stall_block_until_not_busy,
	_rasterizer_profile_stall_block_on_fence,
	_rasterizer_profile_stall_vertex_shader_release,
	_rasterizer_profile_stall_pixel_shader_release,
	_rasterizer_profile_stall_vertex_buffer_release,
	_rasterizer_profile_stall_vertex_buffer_lock,
	_rasterizer_profile_stall_index_buffer_release,
	_rasterizer_profile_stall_index_buffer_lock,
	_rasterizer_profile_stall_texture_release,
	_rasterizer_profile_stall_texture_lock,
	_rasterizer_profile_stall_command_buffer_release,
	_rasterizer_profile_stall_command_buffer_lock,
	_rasterizer_profile_stall_constant_buffer_release,
	_rasterizer_profile_stall_constant_buffer_lock,

	k_rasterizer_profile_stall_count
};

enum
{
	k_maximum_number_of_block_time_buffers = 2,
	k_max_cpu_frames_ahead_of_gpu = 4,
};

struct s_gpu_time_profile
{
	c_stop_watch stop_watch;
	int64 cycles;
};
COMPILE_ASSERT(sizeof(s_gpu_time_profile) == 0x20);

class c_rasterizer_profile_globals
{
public:
	c_rasterizer_profile_globals();
	~c_rasterizer_profile_globals();

	void initialize();
	
	s_gpu_time_profile* get_element_profile(e_rasterizer_profile_elements profile_element_index, int32 frame_reference);

	real64* get_element_block_time(e_rasterizer_profile_stall_elements profile_element_index, int32 block_buffer_index);
	real64* get_block_time_total(int32 block_buffer_index);

	bool* get_frame_valid(int32 index);
	int32 get_last_frame_reference();
	int32 get_current_frame_reference();
	void increment_frame_reference();

	e_rasterizer_profile_modes get_mode();
	void set_mode(e_rasterizer_profile_modes mode);

	void toggle_element_timer(e_rasterizer_profile_elements profile_element_index, uns8 frame_reference, bool a3);
	static void frame_time_callback(uns32);

protected:
	s_gpu_time_profile m_element_profile[k_rasterizer_profile_element_count][k_max_cpu_frames_ahead_of_gpu];

	real64 m_element_block_time[k_rasterizer_profile_stall_count][k_maximum_number_of_block_time_buffers];
	real64 m_block_time_total[k_maximum_number_of_block_time_buffers];

	bool m_frame_valid[k_max_cpu_frames_ahead_of_gpu];
	int32 m_frame_reference;

	e_rasterizer_profile_modes m_mode;
};
COMPILE_ASSERT(sizeof(c_rasterizer_profile_globals) == 0x9D0);

class c_rasterizer_profile_scope
{
public:
	c_rasterizer_profile_scope(e_rasterizer_profile_elements profile_element_index, const wchar_t* name);
	~c_rasterizer_profile_scope();

protected:
	e_rasterizer_profile_elements m_profile_element_index;
};

extern c_rasterizer_profile_globals g_rasterizer_profile_globals;
extern uns32 g_rasterizer_profile_pix_colors[k_rasterizer_profile_element_count];

extern void rasterizer_profile_begin_event(e_rasterizer_profile_elements profile_element_index, const wchar_t* name);
extern void rasterizer_profile_end_event();

extern void rasterizer_profile_initialize();
extern void rasterizer_profile_dispose();
extern void rasterizer_profile_initialize_for_new_map();
extern void rasterizer_profile_dispose_from_old_map();
extern void rasterizer_profile_frame_begin();
extern void rasterizer_profile_frame_end();
extern void rasterizer_profile_update();
extern void rasterizer_profile_start_element_timer(e_rasterizer_profile_elements profile_element_index);
extern void rasterizer_profile_stop_element_timer(e_rasterizer_profile_elements profile_element_index);
extern void rasterizer_profile_set_mode(e_rasterizer_profile_modes mode);
extern real32 rasterizer_profile_get_element_elapsed_milliseconds(e_rasterizer_profile_elements profile_element_index);
extern real32 rasterizer_profile_get_element_elapsed_block_milliseconds(e_rasterizer_profile_stall_elements profile_element_index);
extern real32 rasterizer_profile_get_block_total_time();
extern const char* rasterizer_profile_get_stall_name(int32 profile_element_index);
extern const char* rasterizer_profile_get_element_name(e_rasterizer_profile_elements profile_element_index);

