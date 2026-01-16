#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_scripted_exposure
{
public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();

	uns32 m_flags;
	real32 m_light_intensity_scale;
	real32 m_exposure_bias;
	real32 m_exposure_blend;
	real32 m_exposure_stops;
	real32 g_original_exposure_stops;
	real32 g_target_exposure_stops;
	real32 g_original_exposure_blend;
	real32 g_target_exposure_blend;
	int32 g_exposure_animation_start_time;
	int32 g_exposure_animation_end_time;
	int32 g_exposure_adapt_instantly_frames;
	int32 g_disable_autoexposure_frames;
};
COMPILE_ASSERT(sizeof(s_scripted_exposure) == 0x34);

struct s_ssao_parameter
{
	// SSAO
	// SSAO parameters

	uns32 m_flags;

	real32 m_intensity;
	real32 m_radius;
	real32 m_sample_z_threshold;
};
COMPILE_ASSERT(sizeof(s_ssao_parameter) == 0x10);

struct s_color_grading_parameter
{
	void set_defaults(uns32 flags, real32 blend_time);

	// COLOR GRADING
	// Color grading parameters

	uns32 m_flags;

	real32 m_blend_time;

	s_tag_block m_curves_editor;
	s_tag_block m_brightness_contrast;
	s_tag_block m_hslv;
	s_tag_block m_colorize_effect;
	s_tag_block m_selective_color;
	s_tag_block m_color_balance;
};
COMPILE_ASSERT(sizeof(s_color_grading_parameter) == 0x50);

struct s_lightshafts
{
	// LIGHTSHAFTS
	// Lightshafts parameters

	uns32 m_flags;

	real32 m_pitch;           // [0...90]
	real32 m_heading;         // [0...360]
	real_rgb_color m_tint;
	real32 m_depth_clamp;
	real32 m_intensity_clamp; // [0...1]
	real32 m_falloff_radius;  // [0...2]
	real32 m_intensity;       // [0...50]
	real32 m_blur_radius;     // [0...20]
};
COMPILE_ASSERT(sizeof(s_lightshafts) == 0x2C);

class c_camera_fx_settings
{
public:
	void set_defaults(bool use_default_parameters);

	enum e_parameter_flags
	{
		_parameter_use_default_bit = 0,
		_parameter_blend_limit_relative_bit,
		_parameter_auto_bit,
		_parameter_double_sided_star_bit,
		_parameter_fixed_bit,

		k_num_parameter_flags
	};

	struct s_parameter
	{
		uns16 m_flags;
	};

	struct s_real_parameter :
		s_parameter
	{
		uns16 pad;
		real32 m_target;
		real32 m_blend_limit;
		real32 m_blend_speed;
	};

	struct s_real_instant_parameter :
		s_parameter
	{
		uns16 pad;
		real32 m_target;
	};

	struct s_word_parameter :
		s_parameter
	{
		uns16 m_target;
	};

	struct s_color_parameter :
		s_parameter
	{
		uns16 pad;
		real_rgb_color m_color;
	};

	struct s_real_exposure_parameter :
		s_real_parameter
	{
		real32 m_target_min;
		real32 m_target_max;
		real32 m_auto_exposure_target_brightness;
		real32 m_auto_exposure_delay;
	};

	s_real_exposure_parameter m_exposure;
	s_real_instant_parameter m_auto_exposure_sensitivity;
	s_real_instant_parameter m_exposure_anti_bloom;
	s_real_parameter m_bloom_point;
	s_real_parameter m_bloom_inherent;
	s_real_parameter m_bloom_intensity;
	s_color_parameter m_bloom_large_color;
	s_color_parameter m_bloom_medium_color;
	s_color_parameter m_bloom_small_color;
	s_real_parameter m_bling_intensity;
	s_real_parameter m_bling_size;
	s_real_parameter m_bling_angle;
	s_word_parameter m_bling_count;
	s_real_parameter m_self_illum_preferred;
	s_real_parameter m_self_illum_scale;
	s_ssao_parameter m_ssao;
	s_color_grading_parameter m_color_grading;
	s_lightshafts m_lightshafts;
};
COMPILE_ASSERT(sizeof(c_camera_fx_settings) == 0x170);

// $TODO figure out the difference
class c_exposure
{
public:
	int32 m_render_target_queue_size;
	int32 m_render_target_queue_index;
	int32 m_render_target_queue[1];

	real32 m_prev_exposure;
	real32 m_exposure;
	real32 m_exposure_velocity;
	real32 m_exposure_buffer[60];

	int32 m_buffer_index;
};
COMPILE_ASSERT(sizeof(c_exposure) == 0x10C);

class c_camera_fx_values
{
public:
	void set(const c_camera_fx_settings* settings);

public:
	static real32& g_HDR_target_stops;

//protected:
	c_exposure m_exposure;
	real32 m_exposure_boost;
	real32 m_auto_exposure_sensitivity;
	real32 m_exposure_anti_bloom;
	real32 m_bloom_point;
	real32 m_bloom_inherent;
	real32 m_bloom_intensity;
	real32 m_bling_intensity;
	real32 m_bling_size;
	real32 m_bling_angle;
	uns16 m_bling_count;
	real_rgb_color m_bloom_large_color;
	real_rgb_color m_bloom_medium_color;
	real_rgb_color m_bloom_small_color;
	real32 m_self_illum_preferred;
	real32 m_self_illum_scale;
	real32 m_self_illum_exposure;
	s_ssao_parameter ssao;
	s_color_grading_parameter color_grading;
	s_lightshafts lightshafts;
};
COMPILE_ASSERT(sizeof(c_camera_fx_values) == 0x1F0);

extern void __cdecl scripted_exposure_update(real32 game_seconds_elapsed);

